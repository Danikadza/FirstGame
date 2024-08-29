#pragma once
#ifndef GS_RENDERER_VULKAN_HANDLE_H
#define GS_RENDERER_VULKAN_HANDLE_H

#include "../Define.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct GS_VulkanHandle GS_VulkanHandle;

GS_API GS_VulkanHandle *GS_RendererVulkanHandleCreate();
GS_API void GS_RendererVulkanHandleDestroy(GS_VulkanHandle **vk_handle);

#endif
