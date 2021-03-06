// Compilar com -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm -lpthread
#include"header.h"

int main(char argc, char* argv[]) {
	SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;

	List texture = {NULL, 0, 0, sizeof(SDL_Texture*)};
	TTF_Font* font[ARRAY_NUM_FONT]          = {NULL};

	initGraphics(&window, &renderer, &texture, font);
	List users, meteors, lasers;
	initList(&users, &meteors, &lasers);

	clock_t runtime[2] = {0, 0}; // início e fim de uma frame

	SDL_Event e; // guarda eventos
	int game_type;
	int socket_d;
	int user_id;
	int screen = 0; // menu principal, single e multi, jogo, save, return
	while( 1 ) {
		runtime[0] = SDL_GetTicks();
	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela

		while( SDL_PollEvent(&e) ) {
			switch( e.type ) {
				case SDL_QUIT:
					closeALL(window, renderer, &texture, font, &users, &meteors, &lasers);
					return 0;
				break;
			}
		}

		if( screen == SCREEN_MENU )
			drawMenu( window, renderer, &texture, font, &users, &meteors, &lasers, runtime, &screen, &game_type );
		else if( screen == SCREEN_SINGLE )
			drawInitUser( window, renderer, &texture, font, &users, &meteors, &lasers, runtime, &screen, &game_type );
		else if( screen == SCREEN_MULTI )
			drawMultiplayer( window, renderer, &texture, font, &users, &meteors, &lasers, runtime, &screen, &game_type, &socket_d, &user_id );
		else if( screen == SCREEN_GAME )
			drawGame( window, renderer, &texture, font, &users, &meteors, &lasers, runtime, &screen, &game_type, socket_d, user_id );
		else if( screen == SCREEN_SAVE )
			drawSave( window, renderer, &texture, font, &users, &meteors, &lasers, runtime, &screen );
		else if( screen == SCREEN_OPEN )
			drawOpen( window, renderer, &texture, font, &users, &meteors, &lasers, runtime, &screen, &game_type );
		else if( screen == SCREEN_GAMEOVER )
			drawGameOver( window, renderer, &texture, font, &users, &meteors, &lasers, runtime, &screen, &game_type );

		SDL_RenderPresent(renderer);

		runtime[1] = SDL_GetTicks();
		ctrlFramerate( runtime[1] - runtime[0] );
	}

	closeALL(window, renderer, &texture, font, &users, &meteors, &lasers);
	return 0;
}

void initGraphics( SDL_Window** window, SDL_Renderer** renderer, List* texture, TTF_Font** font ) {
	srand( (unsigned)time(NULL) );

	initSDL(window, renderer); // inicializa o SDL

	SDL_Texture* aux = LoadImageTexture( *renderer, PATH_NAVE, texture );
	if( !aux ) {
		closeALL(*window, *renderer, NULL, NULL, NULL, NULL, NULL);
		exit(-3);
	}

	aux = LoadImageTexture( *renderer, PATH_METEOR, texture );
	if( !aux ) {
		closeALL(*window, *renderer, texture, NULL, NULL, NULL, NULL);
		exit(-3);
	}

	aux = LoadImageTexture( *renderer, PATH_LASER, texture );
	if( !aux ) {
		closeALL(*window, *renderer, texture, NULL, NULL, NULL, NULL);
		exit(-3);
	}

	font[FONT_INDEX_BIG] = TTF_OpenFont(PATH_FONT, FONT_SIZE_BIG);
	if( !font[FONT_INDEX_BIG] ) {
		closeALL(*window, *renderer, texture, NULL, NULL, NULL, NULL);
		fprintf(stderr, "%s\n", TTF_GetError());
		exit(-4);
	}

	font[FONT_INDEX_MED] = TTF_OpenFont(PATH_FONT, FONT_SIZE_MED);
	if( !font[FONT_INDEX_MED] ) {
		closeALL(*window, *renderer, texture, font, NULL, NULL, NULL);
		fprintf(stderr, "%s\n", TTF_GetError());
		exit(-4);
	}

	font[FONT_INDEX_SML] = TTF_OpenFont(PATH_FONT, FONT_SIZE_SML);
	if( !font[FONT_INDEX_SML] ) {
		closeALL(*window, *renderer, texture, font, NULL, NULL, NULL);
		fprintf(stderr, "%s\n", TTF_GetError());
		exit(-4);
	}
}

void initList( List* users, List* meteors, List* lasers ) {
	users->len      = 0;
	users->size     = 0;
	users->elemSize = sizeof(User);
	users->list     = NULL;

	meteors->len      = 0;
	meteors->size     = 0;
	meteors->elemSize = sizeof(Obj);
	meteors->list     = NULL;

	lasers->len      = 0;
	lasers->size     = 0;
	lasers->elemSize = sizeof(Obj);
	lasers->list     = NULL;
}
