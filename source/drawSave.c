#include"../header.h"

void drawSave(  SDL_Window* window,
				SDL_Renderer* renderer,
				List* texture,
				TTF_Font* font[],
				List* users,
				List* meteors,
				List* lasers,
				clock_t* runtime,
				int* screen ) {

	SDL_Color color = {255,255,255,255};
	SDL_Color red = {255,0,0,255};
	SDL_Texture* txt[4] = {NULL};
	txt[0] = LoadTxtTexture(renderer, font[FONT_INDEX_BIG], "Digite o caminho:", &color, texture);
	txt[1] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Voltar", &color, texture);
	txt[2] = LoadTxtTexture(renderer, font[FONT_INDEX_MED], "Salvar", &color, texture);
	txt[3] = LoadTxtTexture(renderer, font[FONT_INDEX_SML], "No Errors", &red, texture);

	Dimension txt_dim[4] = {0};
	int i;
	for( i = 0; i < 4; i++ ) {
		if(SDL_QueryTexture(txt[i], NULL, NULL, &(txt_dim[i].w), &(txt_dim[i].h)) < 0) {
			fprintf(stderr, "%s\n", SDL_GetError());
		}
	}

	SDL_Rect txt_rect[4], txt_background[2];
	txt_rect[0].h = txt_dim[0].h; // rect da posição
	txt_rect[0].w = txt_dim[0].w; // rect da posição

	txt_rect[3].h = txt_dim[3].h; // rect da posição
	txt_rect[3].w = txt_dim[3].w; // rect da posição
	for( i = 1; i < 3; i++ ) {
		txt_rect[i].h = txt_dim[i].h; // rect da posição
		txt_rect[i].w = txt_dim[i].w; // rect da posição
		txt_background[i-1].h = txt_dim[i].h; // rect do background
		txt_background[i-1].w = txt_dim[i].w + 30; // rect do background
	}
	txt_rect[0].x = (WINDOW_SIZE_X - txt_dim[0].w)/2;
	txt_rect[0].y = (WINDOW_SIZE_Y - txt_dim[0].h)/2 - 50;

	txt_rect[1].x = 30;
	txt_rect[1].y = WINDOW_SIZE_Y - txt_background[0].h - 30;

	txt_rect[2].x = WINDOW_SIZE_X - txt_background[1].w - 30;
	txt_rect[2].y = WINDOW_SIZE_Y - txt_background[1].h - 30;

	txt_rect[3].x = (WINDOW_SIZE_X - txt_dim[3].w)/2;
	txt_rect[3].y = (WINDOW_SIZE_Y - txt_dim[3].h)/2 + 50;

	txt_background[0].x = txt_rect[1].x-15; // rect do background
	txt_background[0].y = txt_rect[1].y; // rect do background

	txt_background[1].x = txt_rect[2].x - 15; // rect do background 
	txt_background[1].y = txt_rect[2].y; // rect do background

	int selectButton = -1;

	Dimension input_dim = {0};
	SDL_Rect input_rect, input_cut, input_background;

	SDL_Texture* inputTexture = LoadTxtTexture(renderer, font[FONT_INDEX_MED], ".", &color, texture);
	if(SDL_QueryTexture(inputTexture, NULL, NULL, &(input_rect.w), &(input_rect.h)) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}
	input_background.h = input_rect.h;
	input_background.w = SIZE_INPUT;
	input_background.x = (WINDOW_SIZE_X - input_background.w)/2;
	input_background.y = (WINDOW_SIZE_Y - input_background.h)/2;

	input_cut.x = 0;
	input_cut.y = 0;

	char input[MAX_CHAR_INPUT] = {'\0'};
	int inputLen = 0;
	int hasChangedInput = 0;
	int inputSelected = 0;
	int typing = 0;

	SDL_Event e;

	Point mouse;

	SDL_StopTextInput();

	char message[MAX_MSG_INPUT] = "No Errors.";

	runtime[1] = SDL_GetTicks();
	ctrlFramerate( runtime[1] - runtime[0] ); // delay para considerar a mudança de página
	while( *screen == SCREEN_SAVE ) {
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
					if( inputSelected ) {
						SDL_StartTextInput();
						typing = 1;
					} else {
						SDL_StopTextInput();
						typing = 0;
					}
					if( selectButton == 0 ) {
						SDL_StopTextInput();
						*screen = SCREEN_MENU;
					} else if( selectButton == 1 ) {
						if( inputLen > 0 ) {
							switch( saveUser(users, meteors, lasers, input) ) {
								case -1:
									strncpy(message, "Erro ao salvar o arquivo. Usuario nao encontrado.", MAX_MSG_INPUT);
									hasChangedInput = 1;
								break;
								case -2:
									strncpy(message, "Erro ao salvar o arquivo. Verifique o caminho digitado.", MAX_MSG_INPUT);
									hasChangedInput = 1;
								break;
								default:
									SDL_StopTextInput();
									*screen = SCREEN_GAME;
								break;
							}
						}
					}
				break;
				case SDL_KEYDOWN:
					if( e.key.keysym.sym == SDLK_BACKSPACE && inputLen > 0 ) {
						input[--inputLen] = '\0';
						hasChangedInput = 1;
					} else if( e.key.keysym.sym == SDLK_LEFT ) {
						if( typing ) {
							if(input_cut.x > 0) input_cut.x -= 5;
							else input_cut.x = 0;
						}
					} else if( e.key.keysym.sym == SDLK_RIGHT ) {
						if( typing ) {
							if(input_cut.x < input_dim.w - input_rect.w) input_cut.x += 5;
							else input_cut.x = input_dim.w - input_rect.w;
						}
					}
				break;
				case SDL_TEXTINPUT:
					if(inputLen < MAX_CHAR_INPUT-1) {
						strncpy(input + inputLen, e.text.text, MAX_CHAR_INPUT-inputLen-1);
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
			if(SDL_QueryTexture(inputTexture, NULL, NULL, &(input_dim.w), &(input_dim.h)) < 0) {
				fprintf(stderr, "%s\n", SDL_GetError());
			}
			input_cut.y = 0;
			input_cut.h = input_dim.h;
			if(input_dim.w > input_background.w) {
				input_cut.x = input_dim.w - input_background.w;
				input_cut.w = input_background.w;
				input_rect.w = input_background.w;
			} else {
				input_cut.x = 0;
				input_cut.w = input_dim.w;
				input_rect.w = input_dim.w;
			}
			input_rect.x = (WINDOW_SIZE_X - input_rect.w) / 2;
			input_rect.y = input_background.y;
			hasChangedInput = 0;

			txt[3] = LoadTxtTexture(renderer, font[FONT_INDEX_SML], message, &red, texture);
			if(SDL_QueryTexture(txt[3], NULL, NULL, &(txt_dim[3].w), &(txt_dim[3].h)) < 0) {
				fprintf(stderr, "%s\n", SDL_GetError());
			}
			txt_rect[3].h = txt_dim[3].h; // rect da posição
			txt_rect[3].w = txt_dim[3].w; // rect da posição
			txt_rect[3].x = (WINDOW_SIZE_X - txt_dim[3].w)/2;
			txt_rect[3].y = (WINDOW_SIZE_Y - txt_dim[3].h)/2 + 50;
		}
		SDL_RenderCopy(renderer, inputTexture, &input_cut, &input_rect);

		if( insidePoint( mouse, &input_background ) ) inputSelected = 1;
		else inputSelected = 0;

		selectButton = -1;
		SDL_RenderCopy(renderer, txt[0], NULL, &(txt_rect[0]));
		SDL_RenderCopy(renderer, txt[3], NULL, &(txt_rect[3]));
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
