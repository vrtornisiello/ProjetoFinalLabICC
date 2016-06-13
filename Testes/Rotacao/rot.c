#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#define INIT_OBJ_SIZE 21

#define WINDOW_TITLE  "The Gray Spaceship"
#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

#define PATH_NAVE   "img/nave.bmp"
#define IMG_NAVE_H 200
#define IMG_NAVE_W 200
#define PATH_METEOR "img/meteor.bmp"
#define IMG_METEOR_H 50
#define IMG_METEOR_W 50
#define PATH_LASER  "img/laser.bmp"
#define IMG_LASER_H 100
#define IMG_LASER_W 20

typedef struct _point {
	int x;
	int y;
} Point;

SDL_Texture* LoadImageTexture(SDL_Renderer* renderer, char* path) {
	SDL_Surface* imgSurface = SDL_LoadBMP(path);
	if( !imgSurface ) {
		fprintf(stderr,
			"Nao foi possivel carregar a imagem %s: %s\n", path, SDL_GetError());
		return NULL;
	}

	SDL_Texture* img = SDL_CreateTextureFromSurface(renderer, imgSurface);
	SDL_FreeSurface(imgSurface);
	if( !img) {
		fprintf(stderr,
			"Nao foi possivel carregar a imagem %s: %s\n", path, SDL_GetError());
		return NULL;
	}
	return img;
}

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
	SDL_Texture* nave      = NULL;
	SDL_Texture* meteor    = NULL;
	SDL_Texture* laser     = NULL;

	if( initSDL( &window, &renderer ) < 0 ) {
		fprintf(stderr, "ERRO: inicializacao SDL.\n");
	}

	nave = LoadImageTexture(renderer, "img.bmp");
	if( !nave ) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		fprintf(stderr, "ERRO: inicializacao imagem.\n");
		return -2;
	}

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

    // Clear winow
    SDL_RenderClear( renderer );

 
	SDL_Rect rect;
	rect.x = 100;
	rect.y = 100;
	rect.h = 150;
	rect.w = 150;

	int i;
	Point p = {75,75};
	for( i = 0; i < 360; i++ ) {
	    SDL_RenderClear( renderer );
		SDL_RenderCopyEx( renderer, nave, NULL, &rect, i, (SDL_Point*)&p , SDL_FLIP_NONE );
SDL_RenderPresent(renderer);
		SDL_Delay(20);
	}

	SDL_DestroyRenderer(renderer);
 	SDL_DestroyWindow(window);
 	SDL_Quit();
	return 0;
}
