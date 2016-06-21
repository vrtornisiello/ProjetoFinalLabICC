#include"../header.h"

void getMaxDim( Dimension* dim, int len, Dimension* dest ) {
	dest->h = 0;
	dest->w = 0;
	int i;
	for( i = 0; i < len; i++ ) {
		if( dim[i].w > dest->w ) dest->w = dim[i].w;
		if( dim[i].h > dest->h ) dest->h = dim[i].h;
	}
}

void drawMenu(  SDL_Window* window,
				SDL_Renderer* renderer,
				List* texture,
				TTF_Font* font[],
				List* users,
				List* meteors,
				List* lasers,
				clock_t* runtime,
				int* screen,
				int* game_type ) {
	SDL_Color color = {255,255,255};
	SDL_Texture* txt[7] = {NULL};
	txt[0] = LoadTxtTexture(renderer, font[FONT_INDEX_BIG], "Menu", &color, texture);
	txt[1] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "SinglePlayer", &color, texture);
	txt[2] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "MultiPlayer", &color, texture);
	txt[3] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Abrir status", &color, texture);
	txt[4] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Salvar status", &color, texture);
	txt[5] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Continuar", &color, texture);
	txt[6] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Close", &color, texture);

	Dimension dim[7] = {0};
	int i;
	for( i = 0; i < 7; i++ ) {
		if(SDL_QueryTexture(txt[i], NULL, NULL, &(dim[i].w), &(dim[i].h)) < 0) {
			fprintf(stderr, "%s\n", SDL_GetError());
		}
	}

	SDL_Rect rect[7] = {0};
	SDL_Rect background_rect[6] = {0};

	Dimension button_dim;
	getMaxDim(dim, 7, &button_dim);
	button_dim.w += 40;
	button_dim.h -= 20	;
	int selectedButton = -1;

	// título
	rect[0].x = (WINDOW_SIZE_X - dim[0].w)/2;
	rect[0].y = (WINDOW_SIZE_Y - dim[0].h)/2 - dim[0].h*2;
	rect[0].h = dim[0].h;
	rect[0].w = dim[0].w;

	for( i = 1; i < 7; i++ ) { // botões
		rect[i].x = (WINDOW_SIZE_X - dim[i].w)/2;
		rect[i].y = (WINDOW_SIZE_Y - dim[i].h)/2 + dim[i].h*(i-2) + 5*i;
		rect[i].h = dim[i].h;
		rect[i].w = dim[i].w;

		background_rect[i-1].x = rect[i].x + ( rect[i].w - button_dim.w)/2;
		background_rect[i-1].y = rect[i].y + ( rect[i].h - button_dim.h)/2;
		background_rect[i-1].h = button_dim.h;
		background_rect[i-1].w = button_dim.w;
	}

	Point mouse;
	SDL_Event e;

	runtime[1] = SDL_GetTicks();
	ctrlFramerate( runtime[1] - runtime[0] ); // delay para considerar a mudança de página
	while( *screen == SCREEN_MENU ) {
		runtime[0] = SDL_GetTicks();

	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela

		while( SDL_PollEvent(&e) ) {
			switch( e.type ) {
				case SDL_QUIT:
					closeALL(window, renderer, texture, font, users, meteors, lasers);
					exit(0);
				break;
			    case SDL_MOUSEMOTION:
			    	SDL_GetMouseState(&(mouse.x), &(mouse.y));
			    break;
		        case SDL_MOUSEBUTTONUP:
					if( selectedButton == 0 ) {
						printf("SinglePlayer\n");
						*screen = SCREEN_SINGLE;
					} else if( selectedButton == 1 ) {
						printf("MultiPlayer\n");
						*screen = SCREEN_MULTI;
					} else if( selectedButton == 2 ) {
						printf("Return\n");
						*screen = SCREEN_OPEN;
					} else if( selectedButton == 3 ) {
						printf("Save\n");
						*screen = SCREEN_SAVE;
					} else if( selectedButton == 4 ) {
						printf("Continue\n");
						if(users->len > 0)
							*screen = SCREEN_GAME;
					} else if( selectedButton == 5 ) {
						closeALL(window, renderer, texture, font, users, meteors, lasers);
						exit(0);
					}
				break;
			}
		}

		SDL_RenderCopy(renderer, txt[0], NULL, &(rect[0]));
		if( users->len != 1 ) {
			SDL_SetRenderDrawColor(renderer, 30,30,30,255);
			SDL_RenderFillRect(renderer, &(background_rect[4]));
		}
		SDL_RenderCopy(renderer, txt[4], NULL, &(rect[4]));
		SDL_RenderCopy(renderer, txt[5], NULL, &(rect[5]));

		selectedButton = -1;
		for( i = 1; i < 7; i++ ) { // botões
			if( (i == 4) || (i == 5) ) {
				if( users->len <= 0 )  continue;
			}
			if( insidePoint(mouse, &(background_rect[i-1])) ) {
				selectedButton = i-1;
				SDL_SetRenderDrawColor(renderer, 200,0,0,255);
			} else {
				SDL_SetRenderDrawColor(renderer, 50,50,50,255);
			}
			SDL_RenderFillRect(renderer, &(background_rect[i-1]));
			SDL_RenderCopy(renderer, txt[i], NULL, &(rect[i]));
		}
		SDL_RenderPresent(renderer);

		runtime[1] = SDL_GetTicks();
		ctrlFramerate( runtime[1] - runtime[0] ); // delay
	}

	destroyNonMainTexture( texture ); // Destroi texturas criadas nessa função
}
