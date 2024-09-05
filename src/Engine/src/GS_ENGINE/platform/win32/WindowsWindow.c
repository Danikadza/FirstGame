#ifdef _WIN32
#include <GS_ENGINE/WindowNativeHandle.h>
#include <GS_ENGINE/EventManager.h>
#include <GS_ENGINE/Logger.h>
#include <assert.h>
#include <string.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam);

typedef struct
{
    unsigned int width;
    unsigned int height;
    const char *title;
} GS_WindowData;

struct GS_Window
{
    GS_WindowNativeHandle *handle;
    GS_WindowData data;
    bool isRunning;
};

bool onWindowClose(GS_EventType type, void *sender,
                   void *listener, GS_EventContext ctx)
{
    if (type != GS_WindowCloseEvent)
        return false;
    if (!listener)
        return false;
    GS_ENGINE_LOG(GS_InfoLevel, "%s", "Window is Closing...");
    GS_Window *wind = (GS_Window *)listener;
    GS_WindowSetShouldClose(wind, true);
    return true;
}

bool onWindowResize(GS_EventType type, void *sender,
                    void *listener, GS_EventContext ctx)
{
    if (type != GS_WindowResizeEvent)
        return false;
    if (!listener)
        return NULL;
    GS_ENGINE_LOG(GS_InfoLevel, "%s", "Window is Resizing...");
    GS_Window *wind = (GS_Window *)listener;
    wind->data.width = ctx.ui16[0];
    wind->data.height = ctx.ui16[1];
    return true;
}

GS_API GS_Window *GS_WindowCreate(const char *title, unsigned int width,
                                  unsigned int height)
{
    assert(title);
    assert(width);
    assert(height);
    GS_Window *ret = malloc(sizeof(GS_Window));
    if (!ret)
        return NULL;
    memset(ret, 0, sizeof(GS_Window));
    ret->handle = GS_WindowNativeHandleCreate();
    if (!ret->handle)
    {
        free(ret);
        return NULL;
    }
    ret->data.width = width;
    ret->data.height = height;
    ret->data.title = title;

    ret->handle->wc.cbSize = sizeof(ret->handle->wc);
    ret->handle->wc.lpfnWndProc = WndProc;
    ret->handle->hInstance = GetModuleHandle(0);
    ret->handle->wc.hInstance = ret->handle->hInstance;
    ret->handle->wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    ret->handle->wc.hCursor = LoadCursor(NULL, IDC_UPARROW);
    ret->handle->wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    ret->handle->wc.lpszMenuName = NULL;
    ret->handle->wc.lpszClassName = TEXT("GS_ENGINE_CLASS");
    ret->handle->wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&ret->handle->wc))
    {
        MessageBox(NULL, TEXT("COULD NOT REGISTER WINDOW CLASS"), NULL,
                   MB_ICONERROR);
        GS_WindowNativeHandleDestroy(&ret->handle);
        free(ret);
        return NULL;
    }

    RECT windowRect = {0, 0, ret->data.width, ret->data.height};
    AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, 0);

    ret->handle->hwnd = CreateWindowEx(
        WS_EX_LEFT, TEXT("GS_ENGINE_CLASS"), TEXT(ret->data.title),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
        NULL, NULL, ret->handle->hInstance, NULL);

    if (!ret->handle->hwnd)
    {
        MessageBox(NULL, TEXT("COULD NOT CREATE WINDOW"), NULL, MB_ICONERROR);
        GS_WindowNativeHandleDestroy(&ret->handle);
        free(ret);
        return NULL;
    }

    ShowWindow(ret->handle->hwnd, SW_SHOW);
    UpdateWindow(ret->handle->hwnd);
    ret->isRunning = true;

    GS_EventManagerRegisterCallback(GS_WindowCloseEvent, ret, onWindowClose);
    GS_EventManagerRegisterCallback(GS_WindowResizeEvent, ret, onWindowResize);

    return ret;
}

GS_API void GS_WindowDestroy(GS_Window **window)
{
    if (!window)
        return;
    if (!*window)
        return;
    GS_WindowNativeHandleDestroy(&(*window)->handle);
    free(*window);
    *window = NULL;
}

GS_API void GS_WindowSetShouldClose(GS_Window *window, bool should)
{
    if (!window)
        return;
    window->isRunning = !should;
}

GS_API bool GS_WindowShouldClose(GS_Window *window)
{
    if (!window)
        return true;
    return !window->isRunning;
}

GS_API void GS_WindowPollEvents(GS_Window *window)
{
    if (!window)
        return;
    MSG msg;
    while (PeekMessage(&msg, window->handle->hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

GS_API const unsigned int GS_WindowGetWidth(GS_Window *window)
{
    if (!window)
        return 0;
    return window->data.width;
}

GS_API const unsigned int GS_WindowGetHeight(GS_Window *window)
{
    if (!window)
        return 0;
    return window->data.height;
}

GS_API const char *GS_WindowGetTitle(GS_Window *window)
{
    if (!window)
        return "";
    return window->data.title;
}

GS_API void GS_WindowSetWidth(GS_Window *window, unsigned int width)
{
    if (!window)
        return;
    if (!width)
        return;
    window->data.width = width;
    RECT windowRect = {0, 0, window->data.width, window->data.height};
    AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, 0);
    SetWindowPos(window->handle->hwnd, HWND_TOPMOST, 0, 0, windowRect.right - windowRect.left,
                 windowRect.bottom - windowRect.top,
                 SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_DRAWFRAME);
}

GS_API void GS_WindowSetHeight(GS_Window *window, unsigned int height)
{
    if (!window)
        return;
    if (!height)
        return;
    window->data.height = height;
    RECT windowRect = {0, 0, window->data.width, window->data.height};
    AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, FALSE, 0);
    SetWindowPos(window->handle->hwnd, HWND_TOPMOST, 0, 0, windowRect.right - windowRect.left,
                 windowRect.bottom - windowRect.top,
                 SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_DRAWFRAME);
}

GS_API void GS_WindowSetTitle(GS_Window *window, const char *title)
{
    if (!window)
        return;
    if (!title)
        return;
    window->data.title = title;
    SetWindowText(window->handle->hwnd, TEXT(window->data.title));
}
#endif
