#include <GS_ENGINE/Logger.h>
#include <GS_ENGINE/Renderer/Renderer.h>
#include <stdlib.h>

struct GS_Renderer
{
    GS_VulkanHandle *handle;
};

GS_API GS_Renderer *GS_RendererCreate()
{
    GS_Renderer *ret = malloc(sizeof(GS_Renderer));
    if (!ret)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s",
                      "Failed To Allocate Memory For GS_Renderer");
        return NULL;
    }
    ret->handle = GS_RendererVulkanHandleCreate();
    if (!ret->handle)
    {
        free(ret);
        return NULL;
    }
    return ret;
}

GS_API void GS_RendererDestroy(GS_Renderer **renderer)
{
    if (!renderer)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s",
                      "Invalid Pointer to GS_Renderer passed");
        return;
    }
    if (!*renderer)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Invalid GS_Renderer passed");
        return;
    }
    GS_RendererVulkanHandleDestroy(&(*renderer)->handle);
    free(*renderer);
    *renderer = NULL;
}

GS_API GS_VulkanHandle *GS_RendererGetVulkanHandle(GS_Renderer *renderer)
{
    if (!renderer)
    {
        GS_ENGINE_LOG(GS_ErrorLevel, "%s", "Invalid GS_Renderer passed");
        return NULL;
    }
    return renderer->handle;
}
