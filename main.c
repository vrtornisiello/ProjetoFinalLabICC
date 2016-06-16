// Compilar com -lSDL2 -lSDL2_image -lSDL2_ttf
#include"header.h"

int main(char argc, char* argv[]) {
	SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;

	struct texturePointers textures = {NULL, 1, 0};
	textures.pointers = malloc(sizeof(SDL_Texture*) * textures.size);
	if( !(textures.pointers) ) {
		fprintf(stderr, "Erro ao inicializar o coletor de texturas.\n");
		return -3;
	}

	initSDL(&window, &renderer); // inicializa o SDL

	SDL_Texture* nave = LoadImageTexture( renderer, PATH_NAVE, &textures );
	if( !nave ) {
		closeALL(window, renderer, &textures, NULL);
		return -2;
	}

	SDL_Texture* meteor = LoadImageTexture( renderer, PATH_METEOR, &textures );
	if( !meteor ) {
		closeALL(window, renderer, &textures, NULL);
		return -2;
	}

	SDL_Texture* laser = LoadImageTexture( renderer, PATH_LASER, &textures );
	if( !laser ) {
		closeALL(window, renderer, &textures, NULL);
		return -2;
	}

	TTF_Font* font[ARRAY_NUM_FONT];
	font[0] = TTF_OpenFont(PATH_FONT, 30);
	if( !font[0] ) {
		closeALL(window, renderer, &textures, NULL);
		fprintf(stderr, "%s\n", TTF_GetError());
		return -4;
	}

	font[1] = TTF_OpenFont(PATH_FONT, 20);
	if( !font[1] ) {
		closeALL(window, renderer, &textures, font);
		fprintf(stderr, "%s\n", TTF_GetError());
		return -4;
	}

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

					closeALL(window, renderer, &textures, font);
					return 0;
				break;
			}
		}

		if( screen == 0 ) drawMenu( window, renderer, font, runtime, &screen, &textures );
		else if( screen == 1 ) drawInitUser( window, renderer, font, runtime, &screen, &textures );

		SDL_RenderPresent(renderer);

		runtime[1] = clock();
		ctrlFramerate((runtime[1] - runtime[0])*1000/CLOCKS_PER_SEC);
	}

	closeALL(window, renderer, &textures, font);
	return 0;
}
