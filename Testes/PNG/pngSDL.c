#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#define INIT_OBJ_SIZE 21

#define WINDOW_TITLE  "The Gray Spaceship"
#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

typedef struct _point {
	int x;
	int y;
} Point;

int initSDL(SDL_Window** window, SDL_Renderer** renderer) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,
			"Nao foi possivel inicializar a SDL: %s\n", SDL_GetError());
		return -1;
	}

	*window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
	if( !*window ) {
		SDL_Quit();
		fprintf(stderr,
			"Nao foi possivel abrir a janela: %s\n", SDL_GetError());
		return -1;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if( !*renderer ) {
		SDL_DestroyWindow(*window);
		SDL_Quit();

		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

int main() {
	SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;

	if( initSDL( &window, &renderer ) < 0 ) {
		fprintf(stderr, "ERRO: inicializacao SDL.\n");
	}

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

    // Clear winow
    SDL_RenderClear( renderer );

	if( !IMG_Init( IMG_INIT_PNG ) ) {
		fprintf( stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return -2;
	}

	SDL_Surface* naveSf = IMG_Load("nave.png");
	if( !naveSf ) {
		fprintf( stderr, "Failed in create surface: %s\n", IMG_GetError() );
		return -2;
	}
	SDL_Texture* nave = SDL_CreateTextureFromSurface(renderer, naveSf);
	if( !nave ) {
		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}
	SDL_FreeSurface(naveSf);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = 120;
	rect.w = 120;

	SDL_Point p = {60,60};

    SDL_RenderClear( renderer );
	SDL_RenderCopyEx( renderer, nave, NULL, &rect, 0, &p , SDL_FLIP_NONE );
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);


	SDL_DestroyRenderer(renderer);
 	SDL_DestroyWindow(window);
 	SDL_Quit();
	return 0;
}
