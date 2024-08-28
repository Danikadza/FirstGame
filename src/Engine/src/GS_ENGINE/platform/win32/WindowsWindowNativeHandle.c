#if defined(_WIN32)
#include <GS_ENGINE/WindowNativeHandle.h>
#include <GS_ENGINE/EventManager.h>
#include <stdlib.h>
#include <string.h>
#include <windowsx.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{
    switch (wm)
    {
    case WM_QUIT:
    case WM_DESTROY:
    case WM_CLOSE:
    {
        GS_EventContext ctx = {0};
        GS_EventManagerFireEvent(GS_WindowCloseEvent, NULL, ctx);
        PostQuitMessage(0);
        break;
    }
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_LBUTTONUP:
    {
        GS_EventContext ctx;
        ctx.ui16[0] = GET_X_LPARAM(lParam);
        ctx.ui16[1] = GET_Y_LPARAM(lParam);
        ctx.ui16[2] = wParam;
        GS_EventManagerFireEvent(GS_MouseButtonReleasedEvent, NULL, ctx);
        break;
    }
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_LBUTTONDOWN:
    {
        GS_EventContext ctx;
        ctx.ui16[0] = GET_X_LPARAM(lParam);
        ctx.ui16[1] = GET_Y_LPARAM(lParam);
        ctx.ui32[1] = wParam;
        GS_EventManagerFireEvent(GS_MouseButtonPressedEvent, NULL, ctx);
        break;
    }
    case WM_MOUSEMOVE:
    {
        GS_EventContext ctx;
        ctx.ui16[0] = GET_X_LPARAM(lParam);
        ctx.ui16[1] = GET_Y_LPARAM(lParam);
        ctx.ui32[1] = wParam;
        GS_EventManagerFireEvent(GS_MouseMovedEvent, NULL, ctx);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        GS_EventContext ctx;
        ctx.ui16[0] = GET_X_LPARAM(lParam);
        ctx.ui16[1] = GET_Y_LPARAM(lParam);
        ctx.ui16[2] = GET_KEYSTATE_WPARAM(wParam);
        ctx.ui16[3] = GET_WHEEL_DELTA_WPARAM(wParam);
        GS_EventManagerFireEvent(GS_MouseScrolledEvent, NULL, ctx);
        break;
    }
    case WM_KEYDOWN:
    {
        GS_EventContext ctx;
        ctx.ui32[0] = wParam;
        ctx.ui32[1] = lParam;
        GS_EventManagerFireEvent(GS_KeyPressedEvent, NULL, ctx);
        break;
    }
    case WM_KEYUP:
    {
        GS_EventContext ctx;
        ctx.ui32[0] = wParam;
        ctx.ui32[1] = lParam;
        GS_EventManagerFireEvent(GS_KeyReleasedEvent, NULL, ctx);
        break;
    }
    case WM_SIZE:
    {
        GS_EventContext ctx;
        ctx.ui16[0] = GET_X_LPARAM(lParam);
        ctx.ui16[1] = GET_Y_LPARAM(lParam);
        ctx.ui32[1] = wParam;
        GS_EventManagerFireEvent(GS_WindowResizeEvent, NULL, ctx);
        break;
    }
    default:
        return DefWindowProc(hwnd, wm, wParam, lParam);
    }
    return 0;
}

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
    free(*p_handle);
    *p_handle = NULL;
}
#endif
