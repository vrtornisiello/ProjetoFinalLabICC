#include"../header.h"

int contact( Point center1, Point center2, int r1, int r2 ) {
	float distCenter = sqrtf(pow(center1.x-center2.x, 2) + pow(center1.y-center2.y, 2));
	if( distCenter <= r1 + r2 ) return 1;
	return 0;
}

void createMeteor( Obj* obj, Point user ) {
	Point p;
	switch( rand()%4 ) {
		case 0:
			obj->position.x = -IMG_METEOR_W/2;
			obj->position.y = 50 + rand()%(WINDOW_SIZE_Y-100);
		break;
		case 1:
			obj->position.x = 50 + rand()%(WINDOW_SIZE_X-100);
			obj->position.y = -IMG_METEOR_H/2;
		break;
		case 2:
			obj->position.x = WINDOW_SIZE_X + IMG_METEOR_W/2;
			obj->position.y = 50 + rand()%(WINDOW_SIZE_Y-100);
		break;
		case 3:
			obj->position.x = 50 + rand()%(WINDOW_SIZE_X-100);
			obj->position.y = WINDOW_SIZE_Y + IMG_METEOR_H/2;
		break;
	}

	p.x = user.x - (WINDOW_SIZE_X/3) + rand()%(WINDOW_SIZE_X*2/3);
	p.y = user.y - (WINDOW_SIZE_Y/3) + rand()%(WINDOW_SIZE_Y*2/3);

	obj->ang = -90 + (atan2(obj->position.y - p.y, obj->position.x - p.x))*180/M_PI;

}

void drawGame ( SDL_Window* window,
				SDL_Renderer* renderer,
				List* texture,
				TTF_Font* font[],
				List* users,
				List* meteors,
				List* lasers,
				clock_t* runtime,
				int* screen ) {
	List rect_list;
	rect_list.len = users->len + meteors->len + lasers->len;
	rect_list.size = rect_list.len;
	rect_list.elemSize = sizeof(SDL_Rect);
	rect_list.list = malloc(rect_list.elemSize * rect_list.size);
	if( !rect_list.list ) {
		fprintf(stderr, "Erro ao inicializar lista (rect).\n");
		exit(ERROR_INIT_LIST);
	}

	SDL_Rect* rect = (SDL_Rect*)rect_list.list;

	SDL_Texture* main_texture[TEXTURE_MAIN_NUM] = {NULL};
	int i;
	for( i = 0; i < TEXTURE_MAIN_NUM; i++ ) {
		getFromList(texture, i, &(main_texture[i]));
	}

	User user = {0};
	Obj obj = {0};
	for( i = 0; i < users->len; i++ ) {
		getFromList(users, i, &user);

		user.position.x = IMG_NAVE_W/2 + rand()%(WINDOW_SIZE_X - IMG_NAVE_W);
		user.position.y = IMG_NAVE_H/2 + rand()%(WINDOW_SIZE_Y - IMG_NAVE_H);

		updateList(users, i, &user);
	}

    SDL_Event event;

    SDL_Point nave_center = {
		IMG_NAVE_W/2,
		IMG_NAVE_H/2
	};

    SDL_Point laser_center = {
		IMG_LASER_W/2,
		IMG_LASER_W/2 // não é um erro... centro está na ponta
	};

	Point mouse = {0};
	int key[4] = {0};

	int user_id = 0; //---------------------------------------
	int game_type = GAME_TYPE_SINGLE;// ----------------------
	uint32_t shoot_time[2];
	uint32_t meteor_time[2];
	shoot_time[0] = 0;
	meteor_time[0] = 0;

	runtime[1] = clock();
	ctrlFramerate((runtime[1] - runtime[0])*1000/CLOCKS_PER_SEC);

	while( *screen == SCREEN_GAME ) {
		runtime[0] = SDL_GetTicks();
		shoot_time[1] = SDL_GetTicks();
		meteor_time[1] = SDL_GetTicks();

		getFromList(users, user_id, &user);

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
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
					if(event.key.keysym.sym == SDLK_s) *screen = SCREEN_MENU;
				break;

				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&(mouse.x), &(mouse.y));
				break;
				case SDL_MOUSEBUTTONDOWN:
					if( (shoot_time[1] - shoot_time[0]) > SHOOT_INTERVAL ) {
						obj.position.x = user.position.x;
						obj.position.y = user.position.y;
						obj.initPoint.x = obj.position.x;
						obj.initPoint.y = obj.position.y;
						obj.ang = user.ang;
						obj.dirVector.x = sin(user.ang*M_PI/180) * MOVEMENT_INCREMENT_LASER;
						obj.dirVector.y = -cos(user.ang*M_PI/180) * MOVEMENT_INCREMENT_LASER;
						obj.r = 0;
						obj.type = OBJ_TYPE_LASER;
						addToList(lasers, &obj, OBJ_JUMPSIZE);
	
						SDL_Rect rect_aux = {0};
						addToList(&rect_list, &rect_aux, 3);
						rect = (SDL_Rect*)rect_list.list;

						shoot_time[0] = shoot_time[1];
						shoot_time[1] = SDL_GetTicks();
				}
				break;
			}
		}

		if( (meteor_time[1] - meteor_time[0]) > CREATE_METEOR_INTERVAL ) {
			createMeteor(&obj, user.position);
			printf("%d %d %.2f\n", obj.position.x,obj.position.y, obj.ang);
			obj.initPoint.x = obj.position.x;
			obj.initPoint.y = obj.position.y;
			obj.dirVector.x = sin(obj.ang*M_PI/180) * MOVEMENT_INCREMENT_METEOR;
			obj.dirVector.y = -cos(obj.ang*M_PI/180) * MOVEMENT_INCREMENT_METEOR;
			obj.r = 0;
			obj.type = OBJ_TYPE_METEOR;
			addToList(meteors, &obj, OBJ_JUMPSIZE);

			SDL_Rect rect_aux = {0};
			addToList(&rect_list, &rect_aux, 3);
			rect = (SDL_Rect*)rect_list.list;

			meteor_time[0] = meteor_time[1];
			meteor_time[1] = SDL_GetTicks();
		}

		user.ang = -90 + (atan2(user.position.y - mouse.y, user.position.x - mouse.x))*180/M_PI;

		if( key[UP] ) user.position.y -= MOVEMENT_INCREMENT_USER;
		if( key[DOWN] ) user.position.y += MOVEMENT_INCREMENT_USER;
		if( key[LEFT] )	user.position.x -= MOVEMENT_INCREMENT_USER;
		if( key[RIGHT] ) user.position.x += MOVEMENT_INCREMENT_USER;

		if( user.position.x < 0 ) user.position.x = 0;
		if( user.position.x > WINDOW_SIZE_X ) user.position.x = WINDOW_SIZE_X;
		if( user.position.y < 0 ) user.position.y = 0;
		if( user.position.y > WINDOW_SIZE_Y ) user.position.y = WINDOW_SIZE_Y;

		updateList(users, user_id, &user);

		i = users->len + meteors->len + lasers->len -1;
		while( i >= users->len + meteors->len ) {
			int err = getFromList(lasers, i - users->len - meteors->len, &obj);
			if( err < 0 ) memset(&obj, 0, sizeof(Obj)); // zera

			obj.r++;

			obj.position.x = obj.initPoint.x + obj.dirVector.x * obj.r;
			obj.position.y = obj.initPoint.y + obj.dirVector.y * obj.r;

			if( insidePoint2( obj.position, -WINDOW_BORDER, -WINDOW_BORDER, WINDOW_SIZE_X + WINDOW_BORDER, WINDOW_SIZE_Y + WINDOW_BORDER ) ) {
				rect[i].x = obj.position.x - laser_center.x;
				rect[i].y = obj.position.y - laser_center.y;
				rect[i].w = IMG_LASER_W;
				rect[i].h = IMG_LASER_H;
				SDL_RenderCopyEx( renderer, main_texture[TEXTURE_LASER], NULL, &(rect[i]), obj.ang, &laser_center, SDL_FLIP_NONE );
				if( err >= 0 ) updateList(lasers, i - users->len - meteors->len, &obj);

				int e;
				Obj meteor_aux;
				for( e = 0; e < meteors->len; e++ ) {
					int err = getFromList(meteors, e, &meteor_aux);
					if( err < 0 ) memset(&meteor_aux, 0, sizeof(Obj)); // zera
					if(contact(obj.position, meteor_aux.position, IMG_LASER_W/2, IMG_METEOR_W/2)) {
						printf("METEORO DESTRUIDO");
						removeFromList( meteors, e );
						removeFromList( &rect_list, i );
						removeFromList( &rect_list, i-1 );
					}
				}
			} else {
				removeFromList( lasers, i - users->len - meteors->len );
				removeFromList( &rect_list, i );
			}
			i--;
		}


		i = users->len + meteors->len -1;
		while( i >= users->len ) {
			int err = getFromList(meteors, i - users->len, &obj);
			if( err < 0 ) memset(&obj, 0, sizeof(Obj)); // zera

			obj.r++;

			obj.position.x = obj.initPoint.x + obj.dirVector.x * obj.r;
			obj.position.y = obj.initPoint.y + obj.dirVector.y * obj.r;

			if( insidePoint2( obj.position, -WINDOW_BORDER, -WINDOW_BORDER, WINDOW_SIZE_X + WINDOW_BORDER, WINDOW_SIZE_Y + WINDOW_BORDER ) ) {
				rect[i].x = obj.position.x - IMG_METEOR_W/2;
				rect[i].y = obj.position.y - IMG_METEOR_H/2;
				rect[i].w = IMG_METEOR_W;
				rect[i].h = IMG_METEOR_H;
				SDL_RenderCopy( renderer, main_texture[TEXTURE_METEOR], NULL, &(rect[i]) );
				if( err >= 0 ) updateList(meteors, i - users->len, &obj);
			} else {
				removeFromList( meteors, i - users->len );
				removeFromList( &rect_list, i );
			}
			i--;
		}


		for( i = 0; i < users->len; i++ ) {
			if( getFromList(users, i, &user) < 0 ) memset(&user, 0, sizeof(User)); // zera

			rect[i].x = user.position.x - nave_center.x;
			rect[i].y = user.position.y - nave_center.y;
			rect[i].w = IMG_NAVE_W;
			rect[i].h = IMG_NAVE_H;
//			printf("%d %d %d %d %d\n", i, rect[i].x, rect[i].y, rect[i].w, rect[i].h);

			int e;
			for( e = 0; e < meteors->len; e++ ) {
				int err = getFromList(meteors, e, &obj);
				if( err < 0 ) memset(&obj, 0, sizeof(Obj)); // zera
				if(contact(user.position, obj.position, IMG_NAVE_W/2, IMG_METEOR_W/2)) {
					printf("GAMEOVER");
				}
			}

			if( game_type == GAME_TYPE_MULTI ) {
				for( e = 0; e < lasers->len; e++ ) {
					int err = getFromList(lasers, e, &obj);
					if( err < 0 ) memset(&obj, 0, sizeof(Obj)); // zera
					if(contact(user.position, obj.position, IMG_NAVE_W/2, IMG_LASER_W/2)) {
						printf("GAMEOVER");
					}
				}
			}

			SDL_RenderCopyEx( renderer, main_texture[TEXTURE_NAVE], NULL, &(rect[i]), user.ang, &nave_center, SDL_FLIP_NONE );
		}

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

		runtime[1] = SDL_GetTicks();
		ctrlFramerate((runtime[1] - runtime[0]));
	}
	destroyNonMainTexture(texture);
}
