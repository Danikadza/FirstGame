#pragma once
#ifndef GS_RENDERER_H
#define GS_RENDERER_H

#include "../Define.h"
#include "VulkanHandle.h"

typedef struct GS_Renderer GS_Renderer;

GS_API GS_Renderer *GS_RendererCreate();
GS_API void GS_RendererDestroy(GS_Renderer **renderer);
GS_API GS_VulkanHandle *GS_RendererGetVulkanHandle(GS_Renderer *renderer);

#endif
