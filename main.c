// Compilar com -lSDL2 -lSDL2_image -lSDL2_ttf
#include"header.h"

int main(char argc, char* argv[]) {
	SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;

	struct texturePointers textures = {NULL, 5, 0};
	textures.pointers = malloc(sizeof(SDL_Texture*) * textures.size);
	if( !(textures.pointers) ) {
		fprintf(stderr, "Erro ao inicializar o coletor de texturas.\n");
		return -3;
	}

	initSDL(&window, &renderer); // inicializa o SDL

	SDL_Texture* nave = LoadImageTexture( renderer, PATH_NAVE, &textures );
	if( !nave ) {
		closeALL(window, renderer, &textures);
		return -2;
	}

	SDL_Texture* meteor = LoadImageTexture( renderer, PATH_METEOR, &textures );
	if( !meteor ) {
		closeALL(window, renderer, &textures);
		return -2;
	}

	SDL_Texture* laser = LoadImageTexture( renderer, PATH_LASER, &textures );
	if( !laser ) {
		closeALL(window, renderer, &textures);
		return -2;
	}
	
	SDL_Event e;
	int isRunning = 1;
	while( isRunning ) {
	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela

		while( SDL_PollEvent(&e) ) {
			switch( e.type ) {
				case SDL_QUIT:
					isRunning = 0;
					closeALL(window, renderer, &textures);
//					SDL_DestroyRenderer(renderer);
//				 	SDL_DestroyWindow(window);
//					IMG_Quit();
//					TTF_Quit();
//					SDL_Quit();

					exit(0);
				break;
			}
		}

		SDL_Delay(20);
	}

	closeALL(window, renderer, &textures);
//	SDL_DestroyRenderer(renderer);
//	SDL_DestroyWindow(window);
//	IMG_Quit();
//	TTF_Quit();
//	SDL_Quit();
	return 0;
}
