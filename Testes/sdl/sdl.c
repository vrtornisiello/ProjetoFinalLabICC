#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>

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
} Nave;

SDL_Texture* LoadImageTexture(SDL_Renderer* renderer, char* path) {
	SDL_Surface* imgSurface = SDL_LoadBMP(path);
	if( !imgSurface ) {
		fprintf(stderr,
			"Nao foi possivel carregar a imagem %s: %s\n", path, SDL_GetError());
		return NULL;
	}

	SDL_Texture* img = SDL_CreateTextureFromSurface(renderer, imgSurface);
	SDL_FreeSurface(imgSurface);

	return img;
}

int initSDL(SDL_Window* window, SDL_Renderer* renderer) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,
			"Nao foi possivel inicializar a SDL: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
	if( !window ) {
		SDL_Quit();
		fprintf(stderr,
			"Nao foi possivel abrir a janela: %s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if( !renderer ) {
		SDL_DestroyWindow(window);
		SDL_Quit();

		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

int vectorModule( Vector* vector ) {
	int x = pow(vector->x, 2);
	int y = pow(vector->y, 2);
	return sqrt(x + y);
}

int vectorToAng( Vector* vector ) {
	return acos((vector->x)/vectorModule(vector))*180/M_PI;
}

int main() {
	SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* nave      = NULL;
	SDL_Texture* meteor    = NULL;
	SDL_Texture* laser     = NULL;

	unsigned int usersLen  = 1;
	unsigned int usersSize = 2;
	unsigned int userID  = 0; // posição do usuário na array
	Nave* users = (Nave*)malloc(sizeof(*users) * usersSize);
	if( !users ) {
		fprintf(stderr, "ERRO: malloc (user).\n");
		return -1;
	}

	unsigned int objsLen  = INIT_OBJ_SIZE;
	unsigned int objsSize = INIT_OBJ_SIZE + 1;
	Obj* objs = (Obj*)malloc(sizeof(*objs) * objsSize);
	if( !objs ) {
		free(users);

		fprintf(stderr, "ERRO: malloc (objs).\n");
		return -1;
	}

	if( initSDL( window, renderer ) < 0 ) {
		free(users);
		free(objs);
	}

	nave = LoadImageTexture(renderer, PATH_NAVE);
	if( !nave ) {
		free(users);
		free(objs);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}
	
	meteor = LoadImageTexture(renderer, PATH_METEOR);
	if( !meteor ) {
		free(users);
		free(objs);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}
	
	laser = LoadImageTexture(renderer, PATH_LASER);
	if( !laser ) {
		free(users);
		free(objs);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}

	unsigned int rectLen  = usersLen + objsLen;
	unsigned int rectSize = usersLen + objsLen + 1;
	SDL_Rect* rect = malloc(sizeof(*rect) * rectSize);

	SDL_Point img_center = {0,0}; // Struct igual á nossa Point....
	int start = 1;
	while( start ) {
		// < ----------------------------------------  inicia tudo
		int i;
		img_center.x = IMG_NAVE_W/2;
		img_center.y = IMG_NAVE_H/2;
		for( i = 0; i < usersLen; i++ ) {
			rect[i].x = users[i].position.x - img_center.x;
			rect[i].y = users[i].position.y - img_center.y;
			rect[i].h = IMG_NAVE_H;
			rect[i].w = IMG_NAVE_W;

			if( SDL_RenderCopyEx( renderer, nave, NULL, &(rect[i]),
				users[i].ang, &img_center, SDL_FLIP_NONE) ) {
				fprintf(stderr, "ERRO: rederização do usuario %d.\n", i);
			}
		}

		for( i = usersLen; i < usersLen + objsLen; i++ ) {
			if(objs[i-usersLen].type == 0) {
				img_center.x = IMG_METEOR_W/2;
				img_center.y = IMG_METEOR_H/2;

				rect[i].x = objs[i-usersLen].position.x - img_center.x;
				rect[i].y = objs[i-usersLen].position.y - img_center.y;
				rect[i].h = IMG_METEOR_H;
				rect[i].w = IMG_METEOR_W;

				if( SDL_RenderCopyEx( renderer, meteor, NULL, &(rect[i]),
					vectorToAng(&(objs[i-usersLen].dirVector)), &img_center, SDL_FLIP_NONE) ) {
					fprintf(stderr, "ERRO: rederização do usuario %d.\n", i);
				}
			} else if(objs[i-usersLen].type == 1) {
				img_center.x = IMG_LASER_W/2;
				img_center.y = IMG_LASER_H/2;

				rect[i].x = objs[i-usersLen].position.x - img_center.x;
				rect[i].y = objs[i-usersLen].position.y - img_center.y;
				rect[i].h = IMG_LASER_H;
				rect[i].w = IMG_LASER_W;

				if( SDL_RenderCopyEx( renderer, laser, NULL, &(rect[i]),
					vectorToAng(&(objs[i-usersLen].dirVector)), &img_center, SDL_FLIP_NONE) ) {
					fprintf(stderr, "ERRO: rederização do usuario %d.\n", i);
				}
			}
		}
		
		start = 0;
	}

	free(users);
	free(objs);
	SDL_DestroyRenderer(renderer);
 	SDL_DestroyWindow(window);
 	SDL_Quit();
	return 0;
}
