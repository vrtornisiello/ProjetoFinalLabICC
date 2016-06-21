#include"../header.h"

void drawGameOver(  SDL_Window* window,
					SDL_Renderer* renderer,
					List* texture,
					TTF_Font* font[],
					List* users,
					List* meteors,
					List* lasers,
					clock_t* runtime,
					int* screen,
					int* game_type ) {

	SDL_Color color = {255,255,255,255};
	SDL_Texture* txt[4] = {NULL};
	txt[0] = LoadTxtTexture(renderer, font[FONT_INDEX_BIG], "GAME OVER", &color, texture);
	txt[1] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Menu", &color, texture);
	txt[2] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Close", &color, texture);

	SDL_Rect txt_rect[3], txt_background[2];

	int i;
	for( i = 0; i < 3; i++ ) {
		if(SDL_QueryTexture(txt[i], NULL, NULL, &(txt_rect[i].w), &(txt_rect[i].h)) < 0) {
			fprintf(stderr, "%s\n", SDL_GetError());
		}
	}

	for( i = 1; i < 3; i++ ) {
		txt_background[i-1].h = txt_rect[i].h; // rect do background
		txt_background[i-1].w = txt_rect[i].w + 30; // rect do background
	}
	txt_rect[0].x = (WINDOW_SIZE_X - txt_rect[0].w)/2;
	txt_rect[0].y = (WINDOW_SIZE_Y - txt_rect[0].h)/2 - 50;

	txt_rect[1].x = 30;
	txt_rect[1].y = WINDOW_SIZE_Y - txt_background[0].h - 30;

	txt_rect[2].x = WINDOW_SIZE_X - txt_background[1].w - 30;
	txt_rect[2].y = WINDOW_SIZE_Y - txt_background[1].h - 30;

	txt_background[0].x = txt_rect[1].x-15; // rect do background
	txt_background[0].y = txt_rect[1].y; // rect do background

	txt_background[1].x = txt_rect[2].x - 15; // rect do background 
	txt_background[1].y = txt_rect[2].y; // rect do background

	int selectButton = -1;

	SDL_Event e;

	Point mouse;

	runtime[1] = SDL_GetTicks();
	ctrlFramerate( runtime[1] - runtime[0] ); // delay para considerar a mudança de página
	while( *screen == SCREEN_GAMEOVER ) {
		runtime[0] = SDL_GetTicks();

	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela

		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					closeALL(window, renderer, texture, font, users, meteors, lasers);
					exit(0);
				break;
			    case SDL_MOUSEMOTION:
			    	SDL_GetMouseState(&(mouse.x), &(mouse.y));
			    break;
				case SDL_MOUSEBUTTONUP:
					if( selectButton == 0 ) {
						*screen = SCREEN_MENU;
					} else if( selectButton == 1 ) {
						closeALL(window, renderer, texture, font, users, meteors, lasers);
						exit(0);
					}
				break;
			}
		}

		selectButton = -1;
		SDL_RenderCopy(renderer, txt[0], NULL, &(txt_rect[0]));
		for( i = 1; i < 3; i++ ) {
			if(insidePoint( mouse, &(txt_background[i - 1]) )) {
				selectButton = i - 1;
				SDL_SetRenderDrawColor(renderer, 200,0,0,255);
			} else {
				SDL_SetRenderDrawColor(renderer, 50,50,50,255);
			}

			SDL_RenderFillRect(renderer, &(txt_background[i-1]));
			SDL_RenderCopy(renderer, txt[i], NULL, &(txt_rect[i]));
		}

		SDL_RenderPresent(renderer);

		runtime[1] = SDL_GetTicks();
		ctrlFramerate( runtime[1] - runtime[0] );
	}
	destroyNonMainTexture(texture);
}
