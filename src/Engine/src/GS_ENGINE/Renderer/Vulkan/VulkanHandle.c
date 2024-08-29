#include <GS_ENGINE/Logger.h>
#include <GS_ENGINE/Renderer/VulkanHandle.h>
#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

#if defined(GS_DEBUG)
const char *GS_VKInstanceExtensions[] = {"VK_KHR_surface",
                                         "VK_EXT_debug_utils"};
#else
const char *GS_VKInstanceExtensions[] = {"VK_KHR_surface"};
#endif
const uint32_t GS_VKInstanceExtensionsCount =
    sizeof(GS_VKInstanceExtensions) / sizeof(GS_VKInstanceExtensions[0]);

const char *GS_VKValidationLayers[] = {"VK_LAYER_KHRONOS_validation"};

const uint32_t GS_VKValidationLayersCount =
    sizeof(GS_VKValidationLayers) / sizeof(GS_VKValidationLayers[0]);

#if defined(GS_DEBUG)
const bool GS_EnableValidationLayers = true;
#else
const bool GS_EnableValidationLayers = false;
#endif

struct GS_VulkanHandle
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL GS_DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        return VK_FALSE;

    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        GS_ENGINE_LOG(GS_TraceLevel, "%s", pCallbackData->pMessage);
    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        GS_ENGINE_LOG(GS_InfoLevel, "%s", pCallbackData->pMessage);
    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        GS_ENGINE_LOG(GS_WarnLevel, "%s", pCallbackData->pMessage);
    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", pCallbackData->pMessage);
    return VK_FALSE;
}

VkResult GS_CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugUtilsMessengerEXT");
    if (!func)
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void GS_DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                      VkDebugUtilsMessengerEXT messenger,
                                      const VkAllocationCallbacks *pAllocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT");
    if (!func)
        return;
    func(instance, messenger, pAllocator);
}

bool GS_SetupDebugMessenger(GS_VulkanHandle *handle)
{
    if (!handle)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Invalid VulkanHandle Passed");
        return false;
    }
    if (!GS_EnableValidationLayers)
        return true;
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    createInfo.pfnUserCallback = GS_DebugCallback;
    createInfo.pUserData = NULL;
    if (GS_CreateDebugUtilsMessengerEXT(handle->instance, &createInfo, NULL,
                                        &handle->debugMessenger))
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Failed To Set Up Debug Messenger");
        return false;
    }
    return true;
}

bool GS_CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    if (!layerCount)
        return false;
    VkLayerProperties *avaliableLayers =
        malloc(sizeof(VkLayerProperties) * layerCount);
    if (!avaliableLayers)
        return false;
    vkEnumerateInstanceLayerProperties(&layerCount, avaliableLayers);

    for (uint32_t i = 0; i < GS_VKValidationLayersCount; ++i)
    {
        bool layerFound = false;
        for (uint32_t j = 0; j < layerCount; ++j)
        {
            if (!strcmp(GS_VKValidationLayers[i], avaliableLayers[j].layerName))
            {
                layerFound = true;
                break;
            }
        }
        if (!layerFound)
        {
            free(avaliableLayers);
            return false;
        }
    }
    free(avaliableLayers);
    return true;
}

bool GS_VulkanCreateInstance(GS_VulkanHandle *handle)
{
    if (!handle)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Invalid VulkanHandle Passed");
        return false;
    }

    if (GS_EnableValidationLayers && !GS_CheckValidationLayerSupport())
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s",
                      "Validation Layer Requested, But not Avaliable");
        return false;
    }

    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "GS_Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo = {0};
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = GS_VKInstanceExtensionsCount;
    createInfo.ppEnabledExtensionNames = GS_VKInstanceExtensions;
    if (GS_EnableValidationLayers)
    {
        createInfo.enabledLayerCount = GS_VKValidationLayersCount;
        createInfo.ppEnabledLayerNames = GS_VKValidationLayers;

        debugCreateInfo.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        debugCreateInfo.pfnUserCallback = GS_DebugCallback;
        debugCreateInfo.pUserData = NULL;
        createInfo.pNext =
            (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
    }
    if (vkCreateInstance(&createInfo, NULL, &handle->instance) != VK_SUCCESS)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Failed To Create Vulkan Instance");
        return false;
    }
    return true;
}

bool GS_IsDeviceSuitable()
{
    return true;
}

bool GS_PickPhysicalDevice(GS_VulkanHandle *handle)
{
    if (!handle)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Invalid VulkanHandle Passed");
        return false;
    }
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(handle->instance, &deviceCount, NULL);
    if (!deviceCount)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s",
                      "Failed To Find GPU With Vulkan Support");
        return false;
    }
    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    if (!devices)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s",
                      "Failed To Allocate Memory For VkPhysicalDevice");
        return false;
    }
    vkEnumeratePhysicalDevices(handle->instance, &deviceCount, devices);

    return true;
}

bool GS_VulkanInit(GS_VulkanHandle *handle)
{
    if (!GS_VulkanCreateInstance(handle))
        return false;
    if (!GS_SetupDebugMessenger(handle))
        return false;
    if (!GS_PickPhysicalDevice(handle))
        return false;
    return true;
}

bool GS_VulkanCleanUp(GS_VulkanHandle *handle)
{
    if (!handle)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Invalid VulkanHandle Passed");
        return false;
    }
    if (GS_EnableValidationLayers)
        GS_DestroyDebugUtilsMessengerEXT(handle->instance,
                                         handle->debugMessenger, NULL);
    vkDestroyInstance(handle->instance, NULL);
    return true;
}

GS_API GS_VulkanHandle *GS_RendererVulkanHandleCreate()
{
    GS_VulkanHandle *ret = malloc(sizeof(GS_VulkanHandle));
    if (!ret)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s",
                      "Failed To Allocate Memory For GS_VulkanHandle");
        return NULL;
    }
    memset(ret, 0, sizeof(GS_VulkanHandle));

    if (!GS_VulkanInit(ret))
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Failed To Initialize Vulkan");
        free(ret);
        return NULL;
    }

    return ret;
}

GS_API void GS_RendererVulkanHandleDestroy(GS_VulkanHandle **vk_handle)
{
    if (!vk_handle)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s",
                      "Invalid Pointer To GS_VulkanHandle Passed");
        return;
    }
    if (!*vk_handle)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Invalid GS_VulkanHandle Passed");
        return;
    }
    if (!GS_VulkanCleanUp(*vk_handle))
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Failed To Clean Up Vulkan");
        return;
    }
    free(*vk_handle);
    *vk_handle = NULL;
}
