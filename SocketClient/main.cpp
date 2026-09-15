#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include "Network.h"
#include "Background.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

static SDL_Window* window;
static SDL_Renderer* renderer;
const bool* keyboard_state;
Network* net;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	window = SDL_CreateWindow("Game", 800, 600, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, 0);
	keyboard_state = SDL_GetKeyboardState(NULL);
	

	net = new Network(renderer);
	if (net->Initialize(window, keyboard_state, htons(65000)) != 0)
		return SDL_APP_FAILURE;

	return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void* appstate)
{
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);

	net->Update(SDL_GetTicks());
	
	SDL_RenderPresent(renderer);
	SDL_Delay(6);
	return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
		break;
	}
	net->LocalPlayer_handleEvents(event);
	return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	net->closeNetwork();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
