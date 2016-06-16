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
#include<time.h>
#include<string.h>

#define FRAMERATE 30

#define WINDOW_TITLE  "The Gray Spaceship"
#define WINDOW_SIZE_X 720
#define WINDOW_SIZE_Y 480

#define PATH_FONT   "fonts/Laksaman.ttf"
#define PATH_NAVE   "sprites/nave/nave.png"
#define PATH_METEOR "sprites/meteoro.png"
#define PATH_LASER  "sprites/laser.png"

#define ARRAY_NUM_FONT 2

#define IMG_NAVE_H   200
#define IMG_NAVE_W   200
#define IMG_METEOR_H 50
#define IMG_METEOR_W 50
#define IMG_LASER_H  100
#define IMG_LASER_W  20

#define MAX_CHAR_NOME 11
#define SIZE_INPUT    300

typedef struct _point {
	int x;
	int y;
} Point;

typedef struct _vector {
	int x;
	int y;
} Vector;

typedef struct _dimension {
	int h;
	int w;
} Dimension;

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
	char nome[MAX_CHAR_NOME]; // 10 letras
} Nave;

struct texturePointers {
	SDL_Texture** pointers;
	int size;
	int len;
};

	typedef struct _list{
		void* list;
		int len;
		int size;
		unsigned int elemSize;
	} List;

SDL_Texture* LoadImageTexture(SDL_Renderer* renderer, char* path, struct texturePointers* textures);
SDL_Texture* LoadTxtTexture(SDL_Renderer* renderer, TTF_Font* font, char* txt, SDL_Color* color, struct texturePointers* textures);
int initSDL(SDL_Window** window, SDL_Renderer** renderer);
void ctrlFramerate( float delta );
void closeALL( SDL_Window* window, SDL_Renderer* renderer, struct texturePointers* texture, TTF_Font** font );

void drawMenu( SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font[], clock_t* runtime, int* screen, struct texturePointers* textures );
void drawInitUser( SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font[],
					clock_t* runtime, int* screen, struct texturePointers* textures );
void drawMultiplayer();
void drawSave();
void drawReturn();
void drawGame();


int addTexturePointer( struct texturePointers* texture, SDL_Texture* elem, int jumpSize );
