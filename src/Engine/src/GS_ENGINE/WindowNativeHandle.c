#include <GS_ENGINE/WindowNativeHandle.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)

LRESULT CALLBACK WndProc(HWND hwnd,UINT wm,WPARAM wParam,LPARAM lParam)
{
    return DefWindowProc(hwnd,wm,wParam,lParam);
}

#elif defined(__linux__)
#error "PLATFORM NOT SUPPORTED"
#else
#error "PLATFORM NOT SUPPORTED"
#endif

GS_WindowNativeHandle* GS_WindowNativeHandleCreate()
{
    GS_WindowNativeHandle* ret = malloc(sizeof(GS_WindowNativeHandle));
    memset(ret,0,sizeof(GS_WindowNativeHandle));
    if(!ret)
        return NULL;
    return ret;
}

void GS_WindowNativeHandleDestroy(GS_WindowNativeHandle** p_handle)
{
    if(!p_handle)
        return;
    if(!*p_handle)
        return;
    free(*p_handle);
    *p_handle = NULL;
}