#include <GS_ENGINE/EventManager.h>
#include <GS_ENGINE/Logger.h>
#include <GS_ENGINE/Window.h>

int main()
{
    gsEngineLogger = GS_LoggerCreate("Engine", stdout, true, true, 512);
    gsGameLogger = GS_LoggerCreate("Game", stdout, true, true, 512);
    GS_EventManagerCreate();
    GS_Window *window = GS_WindowCreate("GS_ENGINE", 800, 600);

    while (!GS_WindowShouldClose(window))
    {
        GS_WindowPollEvents(window);
    }
    GS_WindowDestroy(&window);
    GS_EventManagerDestroy();
    GS_LoggerDestroy(&gsEngineLogger);
    GS_LoggerDestroy(&gsGameLogger);
}
