#include"../header.h"

void drawInitUser(  SDL_Window* window,
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
	SDL_Color red = {255,0,0,255};
	SDL_Texture* txt[4] = {NULL};
	txt[0] = LoadTxtTexture(renderer, font[FONT_INDEX_BIG], "User ID", &color, texture);
	txt[1] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Voltar", &color, texture);
	txt[2] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Iniciar", &color, texture);
	txt[3] = LoadTxtTexture(renderer, font[FONT_INDEX_SML], "No Errors", &red, texture);

	SDL_Rect txt_rect[4], txt_background[2];

	int i;
	for( i = 0; i < 4; i++ ) {
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

	txt_rect[3].x = (WINDOW_SIZE_X - txt_rect[3].w)/2;
	txt_rect[3].y = (WINDOW_SIZE_Y - txt_rect[3].h)/2 + 50;

	txt_background[0].x = txt_rect[1].x-15; // rect do background
	txt_background[0].y = txt_rect[1].y; // rect do background

	txt_background[1].x = txt_rect[2].x - 15; // rect do background 
	txt_background[1].y = txt_rect[2].y; // rect do background

	int selectButton = -1;

	SDL_Rect input_rect, input_background;

	SDL_Texture* inputTexture = LoadTxtTexture(renderer, font[FONT_INDEX_MED], ".", &color, texture);
	if(SDL_QueryTexture(inputTexture, NULL, NULL, &(input_rect.w), &(input_rect.h)) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}
	input_background.h = input_rect.h;
	input_background.w = SIZE_INPUT;
	input_background.x = (WINDOW_SIZE_X - input_background.w)/2;
	input_background.y = (WINDOW_SIZE_Y - input_background.h)/2;

	char input[MAX_CHAR_NOME] = {'\0'};
	int inputLen = 0;
	int hasChangedInput = 0;
	int inputSelected = 0;

	SDL_Event e;

	Point mouse;

	SDL_StopTextInput();

	char message[MAX_MSG_INPUT] = "No Errors.";

	runtime[1] = SDL_GetTicks();
	ctrlFramerate( runtime[1] - runtime[0] ); // delay para considerar a mudança de página
	while( *screen == SCREEN_SINGLE ) {
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
					if( inputSelected )
						SDL_StartTextInput();
					else
						SDL_StopTextInput();

					if( selectButton == 0 ) {
						SDL_StopTextInput();
						*screen = SCREEN_MENU;
					} else if( selectButton == 1 ) {
						if( inputLen > 0 ) {
							for( i = users->len - 1; i >= 0; i-- ) {
								removeFromList( users, i );
							}
							for( i = meteors->len - 1; i >= 0; i-- ) {
								removeFromList( meteors, i );
							}
							for( i = lasers->len - 1; i >= 0; i-- ) {
								removeFromList( lasers, i );
							}
							User user;
							strncpy(user.nome, input, MAX_CHAR_NOME);
							user.score = 0;
							user.active = 0;
							if( addToList(users, &user, 1) < 0 ) {
								strncpy(message, "Erro ao criar o usuario.", MAX_MSG_INPUT);
								hasChangedInput = 1;
							} else {
								SDL_StopTextInput();
								*game_type = GAME_TYPE_SINGLE;
								*screen = SCREEN_GAME;
							}
						}
					}
				break;
				case SDL_KEYDOWN:
					if( e.key.keysym.sym == SDLK_BACKSPACE && inputLen > 0 ) {
						input[--inputLen] = '\0';
						hasChangedInput = 1;
						strncpy(message, "No Errors.", MAX_MSG_INPUT);
					}
				break;
				case SDL_TEXTINPUT:
					if(inputLen < MAX_CHAR_NOME - 1) {
						strncpy(input + inputLen, e.text.text, MAX_CHAR_NOME-inputLen-1);
						inputLen = strlen(input);
						strncpy(message, "No Errors.", MAX_MSG_INPUT);
					} else {
						strncpy(message, "Maximo de caracteres atingido", MAX_MSG_INPUT);
					}
					hasChangedInput = 1;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 50,50,50,255);
		SDL_RenderFillRect(renderer, &input_background);

		if( hasChangedInput ) {
			destroyLastTexture(texture); // input
			destroyLastTexture(texture); // message
			inputTexture = LoadTxtTexture(renderer, font[FONT_INDEX_MED], input, &color, texture);
			if(!inputTexture) addToList(texture, &inputTexture, 3);
			if(SDL_QueryTexture(inputTexture, NULL, NULL, &(input_rect.w), &(input_rect.h)) < 0) {
				fprintf(stderr, "%s\n", SDL_GetError());
			}
			input_rect.x = (WINDOW_SIZE_X - input_rect.w) / 2;
			input_rect.y = input_background.y;

			txt[3] = LoadTxtTexture(renderer, font[FONT_INDEX_SML], message, &red, texture);
			if(SDL_QueryTexture(txt[3], NULL, NULL, &(txt_rect[3].w), &(txt_rect[3].h)) < 0) {
				fprintf(stderr, "%s\n", SDL_GetError());
			}
			txt_rect[3].x = (WINDOW_SIZE_X - txt_rect[3].w)/2;
			txt_rect[3].y = (WINDOW_SIZE_Y - txt_rect	[3].h)/2 + 50;
			hasChangedInput = 0;
		}
		SDL_RenderCopy(renderer, inputTexture, NULL, &input_rect);
		SDL_RenderCopy(renderer, txt[3], NULL, &(txt_rect[3]));

		if( insidePoint( mouse, &input_background ) ) inputSelected = 1;
		else inputSelected = 0;

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
