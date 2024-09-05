#pragma once
#ifndef GS_WINDOW_NATIVE_HANDLE_H
#define GS_WINDOW_NATIVE_HANDLE_H
#include "Window.h"

#if defined(_WIN32)
#include <Windows.h>

struct GS_WindowNativeHandle
{
    WNDCLASSEX wc;
    HWND hwnd;
    HINSTANCE hInstance;
};

#if defined(GS_OPENGL_BACKEND)
#include <glad/wgl.h>

struct GS_WindowGraphicsContext
{
    HDC hdc;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC gl_context;
};

#elif definded(GS_VULKAN_BACKEND)

struct GS_WindowGraphicsContext
{
};

#else
#error "UNSUPPORTED PLATFORM"
#endif

#elif defined(__linux__)

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct GS_WindowNativeHandle
{
    Display *display;
    int screen;
    Window root;
    Visual *visual;
    Colormap colormap;
    XSetWindowAttributes attribs;
    Window window;
    XSizeHints *hints;
    Atom wmDeleteMessage;
};

#if defined(GS_OPENGL_BACKEND)
#elif definded(GS_VULKAN_BACKEND)
#else
#error "UNSUPPORTED PLATFORM"
#endif

#else
#error "PLATFORM NOT SUPPORTED"
#endif

GS_WindowNativeHandle *GS_WindowNativeHandleCreate();
void GS_WindowNativeHandleDestroy(GS_WindowNativeHandle **p_handle);

GS_WindowGraphicsContext *GS_WindowGraphicsContextCreate();
void GS_WindowGraphicsContextDestroy(GS_WindowGraphicsContext **p_handle, GS_WindowNativeHandle *handle);

#endif
