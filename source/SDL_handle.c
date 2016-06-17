#include"../header.h"

int initSDL(SDL_Window** window, SDL_Renderer** renderer) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,
			"Nao foi possivel inicializar o SDL: %s\n", SDL_GetError());
		return -1;
	}

	if( !IMG_Init( IMG_INIT_PNG ) ) {
		SDL_Quit();
		fprintf( stderr, "Erro ao iniciar o SDL_image: %s\n", IMG_GetError());
		return -1;
	}

	if( TTF_Init() < 0 ) {
		IMG_Quit();
		SDL_Quit();
		fprintf( stderr, "Erro ao iniciar o SDL_Init: %s\n", TTF_GetError());
		return -1;
	}

	*window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
	if( !(*window) ) {
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		fprintf(stderr,
			"Nao foi possivel abrir a janela: %s\n", SDL_GetError());
		return -2;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if( !(*renderer) ) {
		SDL_DestroyWindow(*window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();

		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		return -2;
	}

	return 0;
}
// ----------------------- Texture ------------------------------
SDL_Texture* LoadImageTexture(SDL_Renderer* renderer, char* path, List* texture) {
	SDL_Surface* imgSurface = IMG_Load(path);
	if( !imgSurface ) {
		fprintf(stderr, "Nao foi possivel carregar a imagem %s: %s\n", path, IMG_GetError());
		return NULL;
	}

	SDL_Texture* img = SDL_CreateTextureFromSurface(renderer, imgSurface);
	SDL_FreeSurface(imgSurface);
	if( img ) {
		if( addToList(texture, &img, 3) < 0 )
			fprintf(stderr, "Falha ao adicionar textura a lista.\n");
	} else
		fprintf(stderr, "Erro ao criar textura (img): %s\n", IMG_GetError());

	return img;
}

SDL_Texture* LoadTxtTexture(SDL_Renderer* renderer, TTF_Font* font, char* txt, SDL_Color* color, List* texture) {
	SDL_Surface* auxSurface  = TTF_RenderText_Blended(font, txt, *color);
	if( !auxSurface ) {
		fprintf(stderr, "Nao foi possivel renderizar o texto: %s\n", TTF_GetError());
		return NULL;
	}

	SDL_Texture* txt_texture = SDL_CreateTextureFromSurface( renderer, auxSurface );
	SDL_FreeSurface(auxSurface);
	if( txt_texture ) {
		if( addToList(texture, &txt_texture, 3) < 0 )
			fprintf(stderr, "Falha ao adicionar textura a lista.\n");
	} else
		fprintf(stderr, "Erro ao criar textura (font): %s\n", SDL_GetError());

	return txt_texture;
}

void destroyNonMainTexture( List* texture ) {
	int i;
	for( i = texture->len - 1; i >= TEXTURE_MAIN_NUM; i-- ) {
		destroyLastTexture( texture );
//		SDL_Texture* aux = NULL;
//		getFromList(texture, i, &aux);
//		if(aux) SDL_DestroyTexture(aux);
//		removeFromList(texture, i);
	}
}

void destroyLastTexture( List* texture ) {
	SDL_Texture* aux = NULL;
	if(texture->len) {
		getFromList(texture, texture->len - 1, &aux);
		if(aux) SDL_DestroyTexture(aux);
		removeFromList(texture, texture->len - 1);
	}
}

// ---------------------- Other -----------------------------------
void ctrlFramerate( float delta ) {
//	printf("Delta: %.2f\n", delta);
	if( delta + 5 < 1000/FRAMERATE )
		SDL_Delay(1000/FRAMERATE - delta);
	else
		SDL_Delay(5);
}

void closeALL( SDL_Window* window, SDL_Renderer* renderer, List* texture, TTF_Font** font ) {
	int i;
	for( i = 0; i < texture->len; i++ ) {
		SDL_Texture* aux = NULL;
		getFromList(texture, i, &aux);
		if(aux) SDL_DestroyTexture(aux);
	}

	for( i = 0; i < ARRAY_NUM_FONT; i++) {
		if(font[i]) TTF_CloseFont(font[i]);
	}

	if(renderer) SDL_DestroyRenderer(renderer);
	if(window) SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
