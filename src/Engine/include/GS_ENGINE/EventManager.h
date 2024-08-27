#pragma once
#ifndef GS_EVENT_SYSTEM_H
#define GS_EVENT_SYSTEM_H

#include "Define.h"
#include <stdbool.h>

#define GS_TOTAL_EVENT_TYPES 1024

typedef enum{
    GS_NullEvent = 0,
    GS_WindowResizeEvent,
    GS_UserEvent0 = 256
}GS_EventType;

typedef union 
{
    unsigned int ui[4];
    int i[4];
}GS_EventContext;

typedef bool (*GS_EventCallback)(GS_EventType type,void* sender,void* listener, GS_EventContext ctx);
typedef struct GS_EventManager GS_EventManager;

extern GS_EventManager* gsEventManager;

GS_API void GS_EventManagerCreate();
GS_API void GS_EventManagerDestroy();

GS_API void GS_EventManagerRegisterCallback(GS_EventType type,void* listener, GS_EventCallback callback);
GS_API void GS_EventManagerUnregisterCallback(GS_EventType type,void* listener, GS_EventCallback callback);
GS_API void GS_EventManagerFireEvent(GS_EventType type,void* sender, GS_EventContext ctx);



#endif