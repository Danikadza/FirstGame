#include <GS_ENGINE/EventManager.h>
#include <GS_ENGINE/Logger.h>
#include <stb/stb_ds.h>

typedef struct
{
    GS_EventCallback callback;
    void *listener;
} Subscriber;

typedef struct
{
    GS_EventType type;
    Subscriber *subscribers;
} Subscription;

struct GS_EventManager
{
    Subscription subscriptions[GS_TOTAL_EVENT_TYPES];
};

void GS_EventManagerCreate()
{
    gsEventManager = malloc(sizeof(GS_EventManager));
    if (!gsEventManager)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Failed To Allocate Memory For Event Manager");
        return;
    }
    for (int i = 0; i < GS_TOTAL_EVENT_TYPES; ++i)
    {
        gsEventManager->subscriptions[i].type = GS_NullEvent;
        gsEventManager->subscriptions[i].subscribers = NULL;
    }
}

void GS_EventManagerDestroy()
{
    if (!gsEventManager)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid Event Manager Passed");
        return;
    }
    for (int i = 0; i < GS_TOTAL_EVENT_TYPES; ++i)
    {
        if (gsEventManager->subscriptions[i].subscribers)
            stbds_arrfree(gsEventManager->subscriptions->subscribers);
    }
    free(gsEventManager);
    gsEventManager = NULL;
}

void GS_EventManagerRegisterCallback(GS_EventType type, void *listener,
                                     GS_EventCallback callback)
{
    if (type < 0 || type >= GS_TOTAL_EVENT_TYPES)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid Type for Event Manager Passed");
        return;
    }
    if (!callback)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid Callback for Event Manager Passed");
        return;
    }
    gsEventManager->subscriptions[type].type = type;
    for (int i = 0;
         i < stbds_arrlen(gsEventManager->subscriptions[type].subscribers); ++i)
    {
        if (callback ==
                gsEventManager->subscriptions[type].subscribers[i].callback &&
            listener ==
                gsEventManager->subscriptions[type].subscribers[i].listener)
            return;
    }
    Subscriber sub = {.callback = callback, .listener = listener};
    stbds_arrput(gsEventManager->subscriptions[type].subscribers, sub);
}

void GS_EventManagerUnregisterCallback(GS_EventType type, void *listener,
                                       GS_EventCallback callback)
{
    if (type < 0 || type >= GS_TOTAL_EVENT_TYPES)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid Type for Event Manager Passed");
        return;
    }
    if (!callback)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid Callback for Event Manager Passed");
        return;
    }
    for (int i = 0;
         i < stbds_arrlen(gsEventManager->subscriptions[type].subscribers); ++i)
    {
        if (callback ==
                gsEventManager->subscriptions[type].subscribers[i].callback &&
            listener ==
                gsEventManager->subscriptions[type].subscribers[i].listener)
        {
            stbds_arrdelswap(gsEventManager->subscriptions[type].subscribers,
                             i);
            return;
        }
    }
}

void GS_EventManagerFireEvent(GS_EventType type, void *sender,
                              GS_EventContext ctx)
{
    if (type < 0 || type >= GS_TOTAL_EVENT_TYPES)
    {
        GS_LoggerLog(gsEngineLogger, GS_ErrorLevel, "%s",
                     "Invalid Type for Event Manager Passed");
        return;
    }
    for (int i = 0;
         i < stbds_arrlen(gsEventManager->subscriptions[type].subscribers); ++i)
    {
        if (gsEventManager->subscriptions[type].subscribers[i].callback(
                type, sender,
                gsEventManager->subscriptions[type].subscribers[i].listener,
                ctx))
            return;
    }
}

GS_EventManager *gsEventManager = NULL;
