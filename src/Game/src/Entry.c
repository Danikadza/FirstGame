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

    GS_EventManagerDestroy();
    GS_LoggerDeinitialize();
}
