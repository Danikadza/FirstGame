#include <GS_ENGINE/Window.h>
#include <stdio.h>

int main()
{
    GS_Window* window = GS_WindowCreate("GS_ENGINE",800,600);
    printf("%d\n",GS_WindowGetWidth(window));
    printf("%d\n",GS_WindowGetHeight(window));
    printf("%s\n",GS_WindowGetTitle(window));

   GS_WindowSetWidth(window,1280);

    while (!GS_WindowShouldClose(window))
    {
        GS_WindowPollEvents(window);
    }
    GS_WindowDestroy(&window);
}