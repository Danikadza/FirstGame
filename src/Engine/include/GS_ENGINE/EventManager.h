#pragma once
#ifndef GS_EVENT_SYSTEM_H
#define GS_EVENT_SYSTEM_H

#include "Define.h"
#include <stdbool.h>
#include <stdint.h>

#define GS_TOTAL_EVENT_TYPES 2048

typedef enum
{
    GS_NullEvent = 0,
    GS_WindowCloseEvent,
    GS_WindowResizeEvent,
    GS_MouseMovedEvent,
    GS_MouseScrolledEvent,
    GS_MouseButtonPressedEvent,
    GS_MouseButtonReleasedEvent,
    GS_KeyPressedEvent,
    GS_KeyReleasedEvent,
    GS_RendererEvent0 = 256,
    GS_UserEvent0 = 512,
} GS_EventType;

typedef union
{
    uint64_t ui64[4];
    int64_t i64[4];
    uint32_t ui32[8];
    int32_t i32[8];
    uint16_t ui16[16];
    int16_t i16[16];
    uint8_t ui8[32];
    int8_t i8[32];
    float f32[8];
    double f64[4];
    char c[32];
} GS_EventContext;

typedef bool (*GS_EventCallback)(unsigned int type, void *sender,
                                 void *listener, GS_EventContext ctx);
typedef struct GS_EventManager GS_EventManager;

extern GS_EventManager *gsEventManager;

GS_API void GS_EventManagerCreate();
GS_API void GS_EventManagerDestroy();

GS_API void GS_EventManagerRegisterCallback(unsigned int type, void *listener,
                                            GS_EventCallback callback);
GS_API void GS_EventManagerUnregisterCallback(unsigned int type, void *listener,
                                              GS_EventCallback callback);
GS_API void GS_EventManagerFireEvent(unsigned int type, void *sender,
                                     GS_EventContext ctx);

#endif
