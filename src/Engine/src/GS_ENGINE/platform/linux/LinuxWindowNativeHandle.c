#if defined(__linux__)
#include <GS_ENGINE/WindowNativeHandle.h>
#include <stdlib.h>
#include <string.h>

GS_WindowNativeHandle *GS_WindowNativeHandleCreate()
{
    GS_WindowNativeHandle *ret = malloc(sizeof(GS_WindowNativeHandle));
    memset(ret, 0, sizeof(GS_WindowNativeHandle));
    if (!ret)
        return NULL;
    return ret;
}

void GS_WindowNativeHandleDestroy(GS_WindowNativeHandle **p_handle)
{
    if (!p_handle)
        return;
    if (!*p_handle)
        return;
    if ((*p_handle)->window)
        XDestroyWindow((*p_handle)->display, (*p_handle)->window);
    if ((*p_handle)->colormap)
        XFreeColormap((*p_handle)->display, (*p_handle)->colormap);
    if ((*p_handle)->display)
        XCloseDisplay((*p_handle)->display);
    if ((*p_handle)->hints)
        XFree((*p_handle)->hints);
    free(*p_handle);
    *p_handle = NULL;
}

#endif
