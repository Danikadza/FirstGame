#ifdef __linux__
#include <GS_ENGINE/Logger.h>
#include <GS_ENGINE/WindowNativeHandle.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    unsigned int width;
    unsigned int height;
    const char *title;
    struct timespec startTime;
} GS_WindowData;

struct GS_Window
{
    GS_WindowNativeHandle *handle;
    GS_WindowData data;
    bool isRunning;
};

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

    ret->handle->display = XOpenDisplay(NULL);
    if (!ret->handle->display)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Failed To Open X11 Display");
        GS_WindowNativeHandleDestroy(&ret->handle);
        free(ret);
        return NULL;
    }
    ret->handle->screen = DefaultScreen(ret->handle->display);
    ret->handle->root = RootWindow(ret->handle->display, ret->handle->screen);
    ret->handle->visual =
        DefaultVisual(ret->handle->display, ret->handle->screen);
    ret->handle->colormap =
        XCreateColormap(ret->handle->display, ret->handle->root,
                        ret->handle->visual, AllocNone);
    ret->handle->attribs.event_mask =
        ExposureMask | KeyPressMask | KeyReleaseMask;
    ret->handle->attribs.colormap = ret->handle->colormap;

    ret->handle->window = XCreateWindow(
        ret->handle->display, ret->handle->root, 0, 0, ret->data.width,
        ret->data.height, 0,
        DefaultDepth(ret->handle->display, ret->handle->screen), InputOutput,
        ret->handle->visual, CWColormap | CWEventMask, &ret->handle->attribs);

    ret->handle->wmDeleteMessage =
        XInternAtom(ret->handle->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(ret->handle->display, ret->handle->window,
                    &ret->handle->wmDeleteMessage, 1);

    XMapWindow(ret->handle->display, ret->handle->window);
    XStoreName(ret->handle->display, ret->handle->window, ret->data.title);

    ret->handle->hints = XAllocSizeHints();
    if (!ret->handle->hints)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Failed To Allocate X11 Hints");
        GS_WindowNativeHandleDestroy(&ret->handle);
        free(ret);
        return NULL;
    }

    ret->handle->hints->flags |= (PMinSize | PMaxSize);
    ret->handle->hints->min_width = ret->handle->hints->max_width =
        ret->data.width;
    ret->handle->hints->min_height = ret->handle->hints->max_height =
        ret->data.height;
    ret->handle->hints->win_gravity = StaticGravity;

    XSetWMNormalHints(ret->handle->display, ret->handle->window,
                      ret->handle->hints);

    ret->isRunning = true;
    if (clock_gettime(CLOCK_MONOTONIC, &ret->data.startTime) == -1)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Failed To Get X11 Time");
        GS_WindowNativeHandleDestroy(&ret->handle);
        free(ret);
        return NULL;
    }
    return ret;
}

GS_API void GS_WindowDestroy(GS_Window **window)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid pointer to window passed");
        return;
    }
    if (!*window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return;
    }
    GS_WindowNativeHandleDestroy(&(*window)->handle);
    free(*window);
    *window = NULL;
}

GS_API void GS_WindowSetShouldClose(GS_Window *window, bool should)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return;
    }
    window->isRunning = !should;
}

GS_API bool GS_WindowShouldClose(GS_Window *window)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return true;
    }
    return !window->isRunning;
}

GS_API void GS_WindowPollEvents(GS_Window *window)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return;
    }
    while (XPending(window->handle->display))
    {
        XEvent event;
        XNextEvent(window->handle->display, &event);
        switch (event.type)
        {
        case Expose:
            break;

        case ClientMessage:
            if (event.xclient.data.l[0] == window->handle->wmDeleteMessage)
                GS_WindowSetShouldClose(window, true);
            break;

        default:
            break;
        }
    }
}

GS_API const unsigned int GS_WindowGetWidth(GS_Window *window)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return 0;
    }
    return window->data.width;
}

GS_API const unsigned int GS_WindowGetHeight(GS_Window *window)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return 0;
    }
    return window->data.height;
}

GS_API const char *GS_WindowGetTitle(GS_Window *window)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return "";
    }
    return window->data.title;
}

GS_API void GS_WindowSetWidth(GS_Window *window, unsigned int width)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return;
    }
    if (!width)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid width passed");
        return;
    }
    window->data.width = width;
    window->handle->hints->min_width = window->handle->hints->max_width =
        window->data.width;

    XSetWMNormalHints(window->handle->display, window->handle->window,
                      window->handle->hints);
}

GS_API void GS_WindowSetHeight(GS_Window *window, unsigned int height)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return;
    }
    if (!height)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid height passed");
        return;
    }
    window->data.height = height;
    window->handle->hints->min_height = window->handle->hints->max_height =
        window->data.height;

    XSetWMNormalHints(window->handle->display, window->handle->window,
                      window->handle->hints);
}

GS_API void GS_WindowSetTitle(GS_Window *window, const char *title)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return;
    }
    if (!title)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid title passed");
        return;
    }
    window->data.title = title;
    XStoreName(window->handle->display, window->handle->window,
               window->data.title);
}

GS_API const uint64_t GS_WindowGetTime(GS_Window *window)
{
    if (!window)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid window passed");
        return 0;
    }
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time) == -1)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s", "Failed To Get Time");
        return 0;
    }
    return (time.tv_sec * 1000000 + time.tv_nsec / 1000) -
           (window->data.startTime.tv_sec * 1000000 +
            window->data.startTime.tv_nsec / 1000);
}

#endif
