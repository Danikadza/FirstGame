#include <GS_ENGINE/EventManager.h>
#include <GS_ENGINE/Logger.h>
#include <GS_ENGINE/Window.h>

int main()
{
    GS_LoggerInitialize();
    GS_EventManagerCreate();
    GS_Window *window = GS_WindowCreate("GS_ENGINE", 800, 600);

    GS_ENGINE_LOG(GS_WarnLevel, "%s", "Hi!");
    GS_ENGINE_LOG(GS_InfoLevel, "%s", "Hi!");
    GS_ENGINE_LOG(GS_FatalLevel, "%s", "Hi!");

    while (!GS_WindowShouldClose(window))
    {
        GS_WindowPollEvents(window);
    }
    GS_WindowDestroy(&window);
    GS_EventManagerDestroy();
    GS_LoggerDeinitialize();
}
