#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define INIT_OBJ_SIZE 21

#define WINDOW_TITLE  "The Gray Spaceship"
#define WINDOW_SIZE_X 640
#define WINDOW_SIZE_Y 480

#define PATH_NAVE   "img/nave.bmp"
#define PATH_METEOR "img/meteor.bmp"
#define PATH_LASER  "img/laser.bmp"

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
	float ang;
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

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		free(users);
		free(objs);

        fprintf(stderr,
			"Nao foi possivel inicializar a SDL: %s\n", SDL_GetError());
        return -2;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
    if( !window ) {
		free(users);
		free(objs);
	    SDL_Quit();

        fprintf(stderr,
			"Nao foi possivel abrir a janela: %s\n", SDL_GetError());
        return -2;
    }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if( !renderer ) {
		free(users);
		free(objs);
	    SDL_DestroyWindow(window);
	    SDL_Quit();

        fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
        return -2;
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

	int start = 1;
	while( start ) {
		// < ----------------------------------------  inicia tudo
		int i;
		for( i = 0; i < usersLen; i++ ) {
			rect[i].x = users[i].position.x;
			rect[i].y = users[i].position.y;
			rect[i].h = 1000; // < -------------------------------
			rect[i].w = 1000; // < -------------------------------
			// < --------------------------- fazer link com imagem
		}

		for( i = usersLen; i < usersLen + objsLen; i++ ) {
			rect[i].x = objs[i-usersLen].position.x;
			rect[i].y = objs[i-usersLen].position.y;
			rect[i].h = 1000; // < -------------------------------
			rect[i].w = 1000; // < -------------------------------
			if(objs[i-usersLen].type == 0) {
				// link com imagem
			} else if(objs[i-usersLen].type == 1) {
				// link com outra imagem
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
