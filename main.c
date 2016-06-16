// Compilar com -lSDL2 -lSDL2_image -lSDL2_ttf
#include"header.h"

int main(char argc, char* argv[]) {
	SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Texture* texture[ARRAY_NUM_TEXTURE] = {NULL};
	TTF_Font* font[ARRAY_NUM_FONT]          = {NULL};

	initGraphics(&window, &renderer, texture, font);

	List users, objs;
	if( initList(&users, &objs) < 0 ) closeALL(window, renderer, texture, font);

	clock_t runtime[2] = {0, 0};

	SDL_Event e;
	int isRunning = 1;
	int screen = 0; // menu principal, single e multi, jogo, save, return
	while( isRunning ) {
		runtime[0] = clock();
	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela

		while( SDL_PollEvent(&e) ) {
			switch( e.type ) {
				case SDL_QUIT:
					isRunning = 0;

					closeALL(window, renderer, texture, font);
					return 0;
				break;
			}
		}

		if( screen == SCREEN_MENU ) drawMenu( window, renderer, texture, font, runtime, &screen );
		else if( screen == SCREEN_SINGLE ) drawInitUser( window, renderer, texture, font, runtime, &screen );
		else if( screen == SCREEN_MULTI ) drawMultiplayer();
		else if( screen == SCREEN_GAME ) drawGame();
		else if( screen == SCREEN_SAVE ) drawSave();
		else if( screen == SCREEN_OPEN ) drawReturn();

		SDL_RenderPresent(renderer);

		runtime[1] = clock();
		ctrlFramerate((runtime[1] - runtime[0])*1000/CLOCKS_PER_SEC);
	}

	closeALL(window, renderer, texture, font);
	return 0;
}

void initGraphics( SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font ) {
	srand( (unsigned)time(NULL) );

	initSDL(window, renderer); // inicializa o SDL

	texture[TEXTURE_NAVE] = LoadImageTexture( *renderer, PATH_NAVE );
	if( !texture[TEXTURE_NAVE] ) {
		closeALL(*window, *renderer, NULL, NULL);
		exit(-3);
	}

	texture[TEXTURE_METEOR] = LoadImageTexture( *renderer, PATH_METEOR );
	if( !texture[TEXTURE_METEOR] ) {
		closeALL(*window, *renderer, texture, NULL);
		exit(-3);
	}

	texture[TEXTURE_LASER] = LoadImageTexture( *renderer, PATH_LASER );
	if( !texture[TEXTURE_LASER] ) {
		closeALL(*window, *renderer, texture, NULL);
		exit(-3);
	}

	font[FONT_INDEX_BIG] = TTF_OpenFont(PATH_FONT, FONT_SIZE_BIG);
	if( !font[FONT_INDEX_BIG] ) {
		closeALL(*window, *renderer, texture, NULL);
		fprintf(stderr, "%s\n", TTF_GetError());
		exit(-4);
	}

	font[FONT_INDEX_MED] = TTF_OpenFont(PATH_FONT, FONT_SIZE_MED);
	if( !font[FONT_INDEX_MED] ) {
		closeALL(*window, *renderer, texture, font);
		fprintf(stderr, "%s\n", TTF_GetError());
		exit(-4);
	}
}

int initList( List* users, List* objs ) {
	users->len      = 0;
	users->size     = 1;
	users->elemSize = sizeof(Nave);
	users->list     = malloc(sizeof(Nave));
	if(!(users->list)) {
		fprintf(stderr, "Erro ao alocar memória (users).\n");
		return -1;
	}

	objs->len      = 0;
	objs->size     = 1;
	objs->elemSize = sizeof(Obj);
	objs->list     = malloc(sizeof(Obj));
	if(!(objs->list)) {
		fprintf(stderr, "Erro ao alocar memória (objs).\n");
		return -1;
	}

	return 0;
}
