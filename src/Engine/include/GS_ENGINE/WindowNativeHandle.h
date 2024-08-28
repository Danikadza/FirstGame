#pragma once
#ifndef GS_WINDOW_NATIVE_HANDLE_H
#define GS_WINDOW_NATIVE_HANDLE_H
#include "Window.h"

#if defined(_WIN32)
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam);

struct GS_WindowNativeHandle
{
    WNDCLASSEX wc;
    HWND hwnd;
    HINSTANCE hInstance;
};

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
};

#else
#error "PLATFORM NOT SUPPORTED"
#endif

GS_WindowNativeHandle *GS_WindowNativeHandleCreate();
void GS_WindowNativeHandleDestroy(GS_WindowNativeHandle **p_handle);

#endif
