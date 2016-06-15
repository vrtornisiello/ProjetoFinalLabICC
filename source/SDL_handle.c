#include"../header.h"

SDL_Texture* LoadImageTexture(SDL_Renderer* renderer, char* path) {
	SDL_Surface* imgSurface = IMG_Load(path);
	if( !imgSurface ) {
		fprintf(stderr,
			"Nao foi possivel carregar a imagem %s: %s\n", path, IMG_GetError());
		return NULL;
	}

	SDL_Texture* img = SDL_CreateTextureFromSurface(renderer, imgSurface);
	SDL_FreeSurface(imgSurface);

	return img;
}

int initSDL(SDL_Window** window, SDL_Renderer** renderer) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,
			"Nao foi possivel inicializar o SDL: %s\n", SDL_GetError());
		return -1;
	}

	if( !IMG_Init( IMG_INIT_PNG ) ) {
		fprintf( stderr, "Erro ao iniciar o SDL_image: %s\n", IMG_GetError());
		return -2;
	}

	if( TTF_Init() < 0 ) {
		fprintf( stderr, "Erro ao iniciar o SDL_Init: %s\n", TTF_GetError());
		return -3;
	}

	*window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
	if( !(*window) ) {
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		fprintf(stderr,
			"Nao foi possivel abrir a janela: %s\n", SDL_GetError());
		return -1;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if( !(*renderer) ) {
		SDL_DestroyWindow(*window);
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();

		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}
