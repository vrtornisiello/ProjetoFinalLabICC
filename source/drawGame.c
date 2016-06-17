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
					Obj aux;
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

				case SDL_MOUSEBUTTONDOWN:
					aux.position.x = ((Nave*)(users->list))[i].position.x;
					aux.position.y = ((Nave*)(users->list))[i].position.y;
					aux.initPoint.x = aux.position.x;
					aux.initPoint.y = aux.position.y;
					aux.dirVector.x = sin(((Nave*)(users->list))[i].ang*M_PI/180) * MOVEMENT_INCREMENT;
					aux.dirVector.y = -cos(((Nave*)(users->list))[i].ang*M_PI/180) * MOVEMENT_INCREMENT;
					aux.r = 0;
					aux.type = 1;
					addToList(objs, &aux, OBJ_JUMPSIZE);

					SDL_Rect rect_aux = {0};
					addToList(&rect, &rect_aux, OBJ_JUMPSIZE);
				break;
			}
		}
		((Nave*)(users->list))[i].ang = -90 + (atan2(
			(((Nave*)(users->list))[i].position.y + nave_center.y - mouse.y),
			(((Nave*)(users->list))[i].position.x + nave_center.x - mouse.x)))*180/M_PI;

		if( key[UP] ) ((Nave*)(users->list))[i].position.y -= MOVEMENT_INCREMENT;
		if( key[DOWN] ) ((Nave*)(users->list))[i].position.y += MOVEMENT_INCREMENT;
		if( key[LEFT] )	((Nave*)(users->list))[i].position.x -= MOVEMENT_INCREMENT;
		if( key[RIGHT] ) ((Nave*)(users->list))[i].position.x += MOVEMENT_INCREMENT;

		SDL_Rect _rect = { ((Nave*)(users->list))[i].position.x,
								((Nave*)(users->list))[i].position.y, texture_dim[TEXTURE_NAVE].w, texture_dim[TEXTURE_NAVE].h };

		SDL_RenderCopyEx( renderer, texture[0]	, NULL, &(_rect), ((Nave*)(users->list))[i].ang, &nave_center , SDL_FLIP_NONE );

		int e;
		for( e = users->len; e < rect.len; e++ ) {
			((Obj*)(objs->list))[e-users->len].r++;
			((Obj*)(objs->list))[e-users->len].position.x = ((Obj*)(objs->list))[e-users->len].initPoint.x + ((Obj*)(objs->list))[e-users->len].dirVector.x * ((Obj*)(objs->list))[e-users->len].r;
			((Obj*)(objs->list))[e-users->len].position.y = ((Obj*)(objs->list))[e-users->len].initPoint.y + ((Obj*)(objs->list))[e-users->len].dirVector.y * ((Obj*)(objs->list))[e-users->len].r;

			((SDL_Rect*)rect.list)[i].x = ((Obj*)(objs->list))[e-users->len].position.x;
			((SDL_Rect*)rect.list)[i].y = ((Obj*)(objs->list))[e-users->len].position.y;
			if( ((Obj*)(objs->list))[e-users->len].type == 1 ) {
				((SDL_Rect*)rect.list)[i].h = texture_dim[TEXTURE_LASER].h;
				((SDL_Rect*)rect.list)[i].w = texture_dim[TEXTURE_LASER].w;
		SDL_RenderCopy( renderer, texture[TEXTURE_LASER] , NULL, &(((SDL_Rect*)rect.list)[i]) );

			}
			if(!( ((Obj*)(objs->list))[e-users->len].position.x < WINDOW_SIZE_X + 100 
				&& ((Obj*)(objs->list))[e-users->len].position.y < WINDOW_SIZE_Y + 100
				&& ((Obj*)(objs->list))[e-users->len].position.x > -100 
				&& ((Obj*)(objs->list))[e-users->len].position.y > -100) ) removeFromList(&rect, e-users->len);
		}
 
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		runtime[1] = clock();
	}
}
