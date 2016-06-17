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

#define OBJ_JUMPSIZE 5

#define FRAMERATE 30

#define WINDOW_TITLE  "The Gray Spaceship"
#define WINDOW_SIZE_X 720
#define WINDOW_SIZE_Y 480

#define PATH_FONT   "fonts/Laksaman.ttf"
#define PATH_NAVE   "sprites/nave/nave.png"
#define PATH_METEOR "sprites/meteoro.png"
#define PATH_LASER  "sprites/laser.png"

#define ARRAY_NUM_FONT  2
#define FONT_SIZE_BIG   30
#define FONT_SIZE_MED   20
#define FONT_INDEX_BIG  0
#define FONT_INDEX_MED  1

#define IMG_NAVE_H   200
#define IMG_NAVE_W   200
#define IMG_METEOR_H 50
#define IMG_METEOR_W 50
#define IMG_LASER_H  100
#define IMG_LASER_W  20

#define MAX_CHAR_NOME 11
#define SIZE_INPUT    300

#define ARRAY_NUM_TEXTURE 3
#define TEXTURE_NAVE      0
#define TEXTURE_METEOR    1
#define TEXTURE_LASER     2

#define SCREEN_MENU   0
#define SCREEN_SINGLE 1
#define SCREEN_MULTI  2
#define SCREEN_GAME   3
#define SCREEN_SAVE   4
#define SCREEN_OPEN   5

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

#define MOVEMENT_INCREMENT 5

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

/*
struct texturePointers {
	SDL_Texture** pointers;
	int size;
	int len;
};
*/
typedef struct _list{
	void* list;
	int len;
	int size;
	unsigned int elemSize;
} List;

int initSDL(SDL_Window** window, SDL_Renderer** renderer);
SDL_Texture* LoadImageTexture(SDL_Renderer* renderer, char* path);
SDL_Texture* LoadTxtTexture(SDL_Renderer* renderer, TTF_Font* font, char* txt, SDL_Color* color);
void ctrlFramerate( float delta );
void closeALL( SDL_Window* window, SDL_Renderer* renderer, SDL_Texture** texture, TTF_Font** font );

void getMaxDim( Dimension* dim, int len, Dimension* dest );
void drawMenu( SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture[], TTF_Font* font[], clock_t* runtime, int* screen );
void drawInitUser( SDL_Window* window, SDL_Renderer* renderer,
					SDL_Texture* texture[], TTF_Font* font[],
						List* users, clock_t* runtime, int* screen );
void drawMultiplayer();
void drawSave();
void drawReturn();

int addToList( List* list, void* elem, int jumpSize );
int removeFromList( List* list, int elemId );

void drawGame(SDL_Window* window, SDL_Renderer* renderer,
				SDL_Texture* texture[], TTF_Font* font[],
					List* users, List* objs, clock_t* runtime, int* screen);

void initGraphics( SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font );
int initList( List* users, List* objs );
