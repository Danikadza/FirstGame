#ifdef __linux__
#include <GS_ENGINE/WindowNativeHandle.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
    window->handle->hints->min_width = window->handle->hints->max_width =
        window->data.width;

    XSetWMNormalHints(window->handle->display, window->handle->window,
                      window->handle->hints);
}

GS_API void GS_WindowSetHeight(GS_Window *window, unsigned int height)
{
    if (!window)
        return;
    if (!height)
        return;
    window->data.height = height;
    window->handle->hints->min_height = window->handle->hints->max_height =
        window->data.height;

    XSetWMNormalHints(window->handle->display, window->handle->window,
                      window->handle->hints);
}

GS_API void GS_WindowSetTitle(GS_Window *window, const char *title)
{
    if (!window)
        return;
    if (!title)
        return;
    window->data.title = title;
    XStoreName(window->handle->display, window->handle->window,
               window->data.title);
}
#endif
