// Compilar com -lSDL2 -lSDL2_image -lSDL2_ttf
#ifdef linux // para linux
#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_ttf.h"
#include"SDL2/SDL_mixer.h"
#endif

#ifdef _WIN32 // para windows
#include"SDL.h"
#include"SDL_image.h"
#include"SDL_ttf.h"
#include"SDL_mixer.h"
#endif

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define WINDOW_TITLE  "The Gray Spaceship"
#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

#define PATH_FONT   "fonts/Lato-Light.ttf"
#define PATH_NAVE   "img/nave/nave.png"
#define PATH_METEOR "img/meteoro/meteoro.bmp"
#define PATH_LASER  "img/laser/laser.bmp"

#define IMG_NAVE_H   200
#define IMG_NAVE_W   200
#define IMG_METEOR_H 50
#define IMG_METEOR_W 50
#define IMG_LASER_H  100
#define IMG_LASER_W  20

typedef struct _point {
	int x;
	int y;
} Point;

typedef struct _vector {
	int x;
	int y;
} Vector;

typedef struct _obj {
	Point position;
	Point initPoint;
	Vector dirVector;
	int r; // parâmetro veja GA
	int type; // meteoro ou tiro
} Obj;

typedef struct _nave {
	Point position;
	int ang;
	int active;
	char nome[11]; // 10 letras
} Nave;


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
		fprintf( stderr, "Erro ao iniciar o SDL_image: %s\n", IMG_GetError() );
		return -2;
	}

	if( TTF_Init() < 0 ) {
		fprintf( stderr, "Erro ao iniciar o SDL_Init.\n");
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
