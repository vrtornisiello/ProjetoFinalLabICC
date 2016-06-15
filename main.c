// Compilar com -lSDL2 -lSDL2_image -lSDL2_ttf
#include"header.h"

int main(char argc, char* argv[]) {
	SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;

	initSDL(&window, &renderer); // inicializa o SDL
	
	SDL_Event e;
	int isRunning = 1;
	while( isRunning ) {
	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela

		while( SDL_PollEvent(&e) ) {
			switch( e.type ) {
				case SDL_QUIT:
					isRunning = 0;
					SDL_DestroyRenderer(renderer);
				 	SDL_DestroyWindow(window);
					IMG_Quit();
					TTF_Quit();
					SDL_Quit();

					exit(0);
				break;
			}
		}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
