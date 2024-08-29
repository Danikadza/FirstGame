#include <GS_ENGINE/EventManager.h>
#include <GS_ENGINE/Logger.h>
#include <GS_ENGINE/Renderer/Renderer.h>
#include <GS_ENGINE/Window.h>

int main()
{
    GS_LoggerInitialize();
    GS_EventManagerCreate();

    GS_Renderer *renderer = GS_RendererCreate();
    GS_RendererDestroy(&renderer);

    GS_Window *window = GS_WindowCreate("Oleg LOX", 1280, 720);
    while (!GS_WindowShouldClose(window))
    {
        GS_WindowPollEvents(window);
    }
    GS_WindowDestroy(&window);
    GS_EventManagerDestroy();
    GS_LoggerDeinitialize();
}
