#include"../header.h"

void drawGame(SDL_Window* window, SDL_Renderer* renderer,
				SDL_Texture* texture[], TTF_Font* font[],
					List* users, List* objs, clock_t* runtime, int* screen) {
	List rect;
	rect.len = users->len + objs->len;
	rect.size = rect.len;
	rect.elemSize = sizeof(SDL_Rect);
	rect.list = malloc( rect.elemSize * rect.size );

	Dimension texture_dim[ARRAY_NUM_TEXTURE] = {0};
	int i;
	for( i = 0; i < ARRAY_NUM_TEXTURE; i++ ) {
		if(SDL_QueryTexture(texture[i], NULL, NULL, &(texture_dim[i].w), &(texture_dim[i].h)) < 0) {
			fprintf(stderr, "%s\n", SDL_GetError());
		}
	}

	for( i = 0; i < users->len; i++ ) {
		((Nave*)(users->list))[i].position.x = 50 + rand()%(WINDOW_SIZE_X-100);
		((Nave*)(users->list))[i].position.y = 50 + rand()%(WINDOW_SIZE_Y-100);
	}

    SDL_Event event;
    SDL_Point nave_center = {
		texture_dim[TEXTURE_NAVE].w/2,
		texture_dim[TEXTURE_NAVE].h/2
	};
	Point mouse = {0};
	int key[4] = {0};
	i = 0;
	while( *screen == SCREEN_GAME ) {
		runtime[1] = clock();
		ctrlFramerate((runtime[1] - runtime[0])*1000/CLOCKS_PER_SEC);
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					closeALL(window, renderer, texture, font);
					exit(0);
				break;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_UP) key[UP] = 1;
					if(event.key.keysym.sym == SDLK_DOWN) key[DOWN] = 1;
					if(event.key.keysym.sym == SDLK_LEFT) key[LEFT] = 1;
					if(event.key.keysym.sym == SDLK_RIGHT) key[RIGHT] = 1;
				break;

				case SDL_KEYUP:
					if(event.key.keysym.sym == SDLK_UP)	key[UP] = 0;
					if(event.key.keysym.sym == SDLK_DOWN) key[DOWN] = 0;
					if(event.key.keysym.sym == SDLK_LEFT) key[LEFT] = 0;
					if(event.key.keysym.sym == SDLK_RIGHT) key[RIGHT] = 0;
				break;

				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&(mouse.x), &(mouse.y));
				break;
			}

			((Nave*)(users->list))[i].ang = -90 + (atan2(
				(((Nave*)(users->list))[i].position.y + nave_center.y - mouse.y),
				(((Nave*)(users->list))[i].position.x + nave_center.x - mouse.x)))*180/M_PI;

			if( key[UP] ) ((Nave*)(users->list))[i].position.y -= MOVEMENT_INCREMENT;
			if( key[DOWN] ) ((Nave*)(users->list))[i].position.y += MOVEMENT_INCREMENT;
			if( key[LEFT] )	((Nave*)(users->list))[i].position.x -= MOVEMENT_INCREMENT;
			if( key[RIGHT] ) ((Nave*)(users->list))[i].position.x += MOVEMENT_INCREMENT;

			SDL_Rect dstrect = { ((Nave*)(users->list))[i].position.x,
									((Nave*)(users->list))[i].position.y, texture_dim[TEXTURE_NAVE].w, texture_dim[TEXTURE_NAVE].h };

			SDL_RenderClear(renderer);
			SDL_RenderCopyEx( renderer, texture[0]	, NULL, &dstrect, ((Nave*)(users->list))[i].ang, &nave_center , SDL_FLIP_NONE );
			SDL_RenderPresent(renderer);
		}
		runtime[1] = clock();
	}
}
