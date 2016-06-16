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

void drawMenu( SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font[], clock_t* runtime, int* screen, struct texturePointers* textures ) {
	SDL_Color color = {255,255,255};
	SDL_Texture* txt[5] = {NULL};
	txt[0] = LoadTxtTexture(renderer, font[0], "Menu", &color, textures);
	txt[1] = LoadTxtTexture(renderer, font[1], "SinglePlayer", &color, textures);
	txt[2] = LoadTxtTexture(renderer, font[1], "MultiPlayer", &color, textures);
	txt[3] = LoadTxtTexture(renderer, font[1], "Return", &color, textures);
	txt[4] = LoadTxtTexture(renderer, font[1], "Close", &color, textures);

	Dimension dim[5] = {0};
	int i;
	for( i = 0; i < 5; i++ ) {
		if(SDL_QueryTexture(txt[i], NULL, NULL, &(dim[i].w), &(dim[i].h)) < 0) {
			fprintf(stderr, "%s\n", SDL_GetError());
		}
	}

	SDL_Rect rect[5] = {0};
	SDL_Rect background_rect[4] = {0};

	Dimension button_dim;
	getMaxDim(dim, 5, &button_dim);
	button_dim.w += 20;
	button_dim.h -= 20	;
	int selectedButton[4] = {0};

	// título
	rect[0].x = (WINDOW_SIZE_X - dim[0].w)/2;
	rect[0].y = (WINDOW_SIZE_Y - dim[0].h)/2 - dim[0].h*2;
	rect[0].h = dim[0].h;
	rect[0].w = dim[0].w;

	for( i = 1; i < 5; i++ ) { // botões
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
	while( (*screen) == 0 ) {
		runtime[1] = clock();
		ctrlFramerate((runtime[1] - runtime[0])*1000/CLOCKS_PER_SEC);

	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela
		while( SDL_PollEvent(&e) ) {
			switch( e.type ) {
				case SDL_QUIT:
					closeALL(window, renderer, textures, font);
					exit(0);
				break;
			    case SDL_MOUSEMOTION:
			    	SDL_GetMouseState(&(mouse.x), &(mouse.y));
			    break;
		        case SDL_MOUSEBUTTONUP:
					if( selectedButton[0] ) {
						printf("SinglePlayer\n");
						*screen = 1;
					} else if( selectedButton[1] ) {
						printf("MultiPlayer\n");
						*screen = 2;
					} else if( selectedButton[2] ) {
						printf("Return\n");
						*screen = 6;
					} else if( selectedButton[3] ) {
						closeALL(window, renderer, textures, font);
						exit(0);
					}
				break;
			}
		}

		SDL_RenderCopy(renderer, txt[0], NULL, &(rect[0]));

		for( i = 1; i < 5; i++ ) { // botões
			if((mouse.x > background_rect[i-1].x) && (mouse.x < background_rect[i-1].x + background_rect[i-1].w)
			&& (mouse.y > background_rect[i-1].y) && (mouse.y < background_rect[i-1].y + background_rect[i-1].h)) {
				selectedButton[i-1] = 1;
				SDL_SetRenderDrawColor(renderer, 200,0,0,255);
				SDL_RenderFillRect(renderer, &(background_rect[i-1]));
			} else {
				selectedButton[i-1] = 0;
				SDL_SetRenderDrawColor(renderer, 50,50,50,255);
				SDL_RenderFillRect(renderer, &(background_rect[i-1]));
			}
			SDL_RenderCopy(renderer, txt[i], NULL, &(rect[i]));
		}
		SDL_RenderPresent(renderer);

		runtime[0] = clock();
	}

	if(txt[0]) SDL_DestroyTexture(txt[0]);
	if(txt[1]) SDL_DestroyTexture(txt[1]);
	if(txt[2]) SDL_DestroyTexture(txt[2]);
	if(txt[3]) SDL_DestroyTexture(txt[3]);
	if(txt[4]) SDL_DestroyTexture(txt[4]);
}

void drawInitUser( SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font[],
					clock_t* runtime, int* screen, struct texturePointers* textures ) {
	SDL_Color color = {255,255,255};
	SDL_Texture* txt[3] = {NULL};
	txt[0] = LoadTxtTexture(renderer, font[1], "User ID", &color, textures);
	txt[1] = LoadTxtTexture(renderer, font[1], "Voltar", &color, textures);
	txt[2] = LoadTxtTexture(renderer, font[1], "Iniciar", &color, textures);

	Dimension dim[3] = {0};
	if(SDL_QueryTexture(txt[0], NULL, NULL, &(dim[0].w), &(dim[0].h)) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}
	if(SDL_QueryTexture(txt[1], NULL, NULL, &(dim[1].w), &(dim[1].h)) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}
	if(SDL_QueryTexture(txt[2], NULL, NULL, &(dim[2].w), &(dim[2].h)) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	SDL_Rect rect[3] = {0};
	rect[0].y = WINDOW_SIZE_Y/2 - dim[0].h*5/2;
	rect[0].x = (WINDOW_SIZE_X - dim[0].w)/2;
	rect[0].h = dim[0].h;
	rect[0].w = dim[0].w;

	rect[1].y = WINDOW_SIZE_Y - dim[1].h - 40;
	rect[1].x = 40;
	rect[1].h = dim[1].h;
	rect[1].w = dim[1].w;

	rect[2].y = WINDOW_SIZE_Y - dim[2].h - 40;
	rect[2].x = WINDOW_SIZE_X - dim[2].w - 40;
	rect[2].h = dim[2].h;
	rect[2].w = dim[2].w;

	SDL_Rect background_rect[2] = {0};
	int i;
	for( i = 1; i < 3; i++ ) { // botões
		background_rect[i-1].x = rect[i].x + ( rect[i].w - dim[i].w - 20)/2;
		background_rect[i-1].y = rect[i].y + ( rect[i].h - dim[i].h)/2;
		background_rect[i-1].h = dim[i].h;
		background_rect[i-1].w = dim[i].w + 20;
	}

	SDL_Rect rect_Input = {0};
	Dimension dim_Input = {0};

	SDL_Rect rectbg_Input = {0};
	rectbg_Input.w = SIZE_INPUT;
	rectbg_Input.h = dim[0].h + 20;
	rectbg_Input.x = (WINDOW_SIZE_X - rectbg_Input.w)/2;
	rectbg_Input.y = (WINDOW_SIZE_Y - rectbg_Input.h)/2;
	
	SDL_Texture* inputTexture = NULL;

	int hasChangedInput = 1;
	int selectInput = 0;
	int selectButton[2] = {0};

	Point mouse;

	int inputLen = 0;
	char input[MAX_CHAR_NOME] = {'\0'};
	SDL_Event e;
	while( *screen == 1 ) {
		runtime[1] = clock();
		ctrlFramerate((runtime[1] - runtime[0])*1000/CLOCKS_PER_SEC);

	    SDL_SetRenderDrawColor( renderer, 26, 26, 26, 255 ); // Fundo
	    SDL_RenderClear( renderer ); // Limpa a tela
		while( SDL_PollEvent(&e) ) {
			switch( e.type ) {
				case SDL_QUIT:
					closeALL(window, renderer, textures, font);
					exit(0);
				break;
				case SDL_KEYDOWN:
					if( e.key.keysym.sym == SDLK_BACKSPACE && inputLen > 0 ) {
						input[--inputLen] = '\0';
						hasChangedInput = 1;
					}
				break;
				case SDL_TEXTINPUT:
					if(inputLen < MAX_CHAR_NOME) {
						strncpy(input+inputLen, e.text.text, MAX_CHAR_NOME-inputLen);
						inputLen += strlen(e.text.text);
						input[inputLen] = '\0';
						hasChangedInput = 1;
						printf("%s %d %li\n", input, inputLen, strlen(e.text.text));
					}
				break;
				case SDL_MOUSEBUTTONUP:
					if(selectInput) SDL_StartTextInput();
					else SDL_StopTextInput();
					if(selectButton[0]) *screen = 0;
					else if(selectButton[1]) {
						if(inputLen) *screen = 3;
						else {
							strncpy(input, "Digite ID", MAX_CHAR_NOME);
							inputLen = 10;
							hasChangedInput = 1;
						}
					}
				break;
			    case SDL_MOUSEMOTION:
			    	SDL_GetMouseState(&(mouse.x), &(mouse.y));
			    break;
			}
		}

		if((mouse.x > rectbg_Input.x) && (mouse.x < rectbg_Input.x + rectbg_Input.w)
		&& (mouse.y > rectbg_Input.y) && (mouse.y < rectbg_Input.y + rectbg_Input.h)) {
			selectInput = 1;
			SDL_SetRenderDrawColor(renderer, 200,0,0,255);
			SDL_RenderFillRect(renderer, &rectbg_Input);
		} else {
			selectInput = 0;
			SDL_SetRenderDrawColor(renderer, 100,100,100,255);
			SDL_RenderFillRect(renderer, &rectbg_Input);
		}

		for( i = 0; i < 2; i++ ) {
			if((mouse.x > background_rect[i].x) && (mouse.x < background_rect[i].x + background_rect[i].w)
			&& (mouse.y > background_rect[i].y) && (mouse.y < background_rect[i].y + background_rect[i].h)) {
				selectButton[i] = 1;
				SDL_SetRenderDrawColor(renderer, 200,0,0,255);
				SDL_RenderFillRect(renderer, &(background_rect[i]));
			} else {
				selectButton[i] = 0;
				SDL_SetRenderDrawColor(renderer, 100,100,100,255);
				SDL_RenderFillRect(renderer, &(background_rect[i]));
			}
		}

		SDL_RenderCopy(renderer, txt[0], NULL, &(rect[0]));
		SDL_RenderCopy(renderer, txt[1], NULL, &(rect[1]));
		SDL_RenderCopy(renderer, txt[2], NULL, &(rect[2]));

		if(hasChangedInput) {
			if(inputTexture) SDL_DestroyTexture(inputTexture);

			inputTexture = LoadTxtTexture(renderer, font[1], input, &color, NULL);
			if(SDL_QueryTexture(inputTexture, NULL, NULL, &(dim_Input.w), &(dim_Input.h)) < 0) {
				fprintf(stderr, "%s\n", SDL_GetError());
			}

			rect_Input.x = (WINDOW_SIZE_X - dim_Input.w)/2;
			rect_Input.y = (WINDOW_SIZE_Y - dim_Input.h)/2;
			rect_Input.h = dim_Input.h;
			if(dim_Input.w > SIZE_INPUT) {
				rect_Input.w = SIZE_INPUT;
			} else {
				rect_Input.w = dim_Input.w;
			}

			hasChangedInput = 0;
		}

		SDL_RenderCopy(renderer, inputTexture, NULL, &rect_Input);

		
		SDL_RenderPresent(renderer);
		runtime[0] = clock();

	}
	if(txt[0]) SDL_DestroyTexture(txt[0]);
	if(txt[1]) SDL_DestroyTexture(txt[1]);
	if(txt[2]) SDL_DestroyTexture(txt[2]);
}

void drawMultiplayer() {

}
void drawSave() {

}
void drawReturn() {

}
