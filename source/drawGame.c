#include"../header.h"

void drawGame ( SDL_Window* window,
				SDL_Renderer* renderer,
				List* textures,
				TTF_Font* font[],
				List* users,
				List* objs,
				clock_t* runtime,
				int* screen ) {
	List rect;
	rect.len = users->len + objs->len;
	rect.size = rect.len;
	rect.elemSize = sizeof(SDL_Rect);
	rect.list = malloc( rect.elemSize * rect.size );

	SDL_Texture* texture[TEXTURE_MAIN_NUM] = {NULL};
	Dimension texture_dim[TEXTURE_MAIN_NUM] = {0};
	int i;
	for( i = 0; i < TEXTURE_MAIN_NUM; i++ ) {
		getFromList(textures, i, &(texture[i]));
		if(SDL_QueryTexture(texture[i], NULL, NULL, &(texture_dim[i].w), &(texture_dim[i].h)) < 0) {
			fprintf(stderr, "%s\n", SDL_GetError());
		}
	}

	for( i = 0; i < users->len; i++ ) {
		User user_aux;
		getFromList(users, i, &user_aux);

		user_aux.position.x = 50 + rand()%(WINDOW_SIZE_X-100);
		user_aux.position.y = 50 + rand()%(WINDOW_SIZE_Y-100);

		updateList(users, i, &user_aux);
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

		User user = {0};
		getFromList(users, i, &user);

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					closeALL(window, renderer, textures, font);
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
					aux.position.x = user.position.x;
					aux.position.y = user.position.y;
					aux.initPoint.x = aux.position.x;
					aux.initPoint.y = aux.position.y;
					aux.dirVector.x = sin(user.ang*M_PI/180) * MOVEMENT_INCREMENT;
					aux.dirVector.y = -cos(user.ang*M_PI/180) * MOVEMENT_INCREMENT;
					aux.r = 0;
					aux.type = 1;
					addToList(objs, &aux, OBJ_JUMPSIZE);

					SDL_Rect rect_aux = {0};
					addToList(&rect, &rect_aux, OBJ_JUMPSIZE);
				break;
			}
		}

		user.ang = -90 + (atan2(user.position.y + nave_center.y - mouse.y,
			user.position.x + nave_center.x - mouse.x))*180/M_PI;

		if( key[UP] ) user.position.y -= MOVEMENT_INCREMENT;
		if( key[DOWN] ) user.position.y += MOVEMENT_INCREMENT;
		if( key[LEFT] )	user.position.x -= MOVEMENT_INCREMENT;
		if( key[RIGHT] ) user.position.x += MOVEMENT_INCREMENT;

		SDL_Rect _rect = { user.position.x, user.position.y,
			texture_dim[TEXTURE_NAVE].w, texture_dim[TEXTURE_NAVE].h };
	
		SDL_RenderCopyEx( renderer, texture[0], NULL, &(_rect), user.ang, &nave_center , SDL_FLIP_NONE );

		updateList(users, i, &user);

		int e;
		for( e = users->len; e < rect.len; e++ ) {
			Obj obj_aux = {0};
			getFromList(objs, e-users->len, &obj_aux);
			obj_aux.r++;
			obj_aux.position.x = obj_aux.initPoint.x + obj_aux.dirVector.x * obj_aux.r;
			obj_aux.position.y = obj_aux.initPoint.y + obj_aux.dirVector.y * obj_aux.r;

			SDL_Rect rect_aux;
			getFromList( &rect, i, &rect_aux );
			rect_aux.x = obj_aux.position.x;
			rect_aux.y = obj_aux.position.y;
			if( obj_aux.type == 1 ) {
				rect_aux.h = texture_dim[TEXTURE_LASER].h;
				rect_aux.w = texture_dim[TEXTURE_LASER].w;
				SDL_RenderCopy( renderer, texture[TEXTURE_LASER] , NULL, &rect_aux );
			}
			if(!( obj_aux.position.x < WINDOW_SIZE_X + 100 
				&& obj_aux.position.y < WINDOW_SIZE_Y + 100
				&& obj_aux.position.x > -100 
				&& obj_aux.position.y > -100) ) removeFromList(&rect, e-users->len);
			else updateList(&rect, e-users->len, &rect_aux);

			updateList(objs, e-users->len, &obj_aux);
		}

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
		runtime[1] = clock();
	}
}
