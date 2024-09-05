#pragma once
#ifndef GS_WINDOW_H
#define GS_WINDOW_H

#include "Define.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct GS_Window GS_Window;
typedef struct GS_WindowNativeHandle GS_WindowNativeHandle;
typedef struct GS_WindowGraphicsContext GS_WindowGraphicsContext;

GS_API GS_Window *GS_WindowCreate(const char *title, unsigned int width,
                                  unsigned int height);
GS_API void GS_WindowDestroy(GS_Window **window);

GS_API void GS_WindowSetShouldClose(GS_Window *window, bool should);
GS_API bool GS_WindowShouldClose(GS_Window *window);
GS_API void GS_WindowPollEvents(GS_Window *window);

GS_API const unsigned int GS_WindowGetWidth(GS_Window *window);
GS_API const unsigned int GS_WindowGetHeight(GS_Window *window);
GS_API const char *GS_WindowGetTitle(GS_Window *window);

GS_API void GS_WindowSetWidth(GS_Window *window, unsigned int width);
GS_API void GS_WindowSetHeight(GS_Window *window, unsigned int height);
GS_API void GS_WindowSetTitle(GS_Window *window, const char *title);
GS_API const uint64_t GS_WindowGetTime(GS_Window *window);

GS_API void GS_WindowSwapBuffers(GS_Window *window);
#endif
