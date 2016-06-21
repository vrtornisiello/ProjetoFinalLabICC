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

void drawGame(  SDL_Window* window,
				SDL_Renderer* renderer,
				List* texture, 	 	
				TTF_Font* font[],
				List* users,
				List* meteors,
				List* lasers,
				clock_t* runtime,
				int* screen,
				int* game_type,
				int socket_d,
				int user_id ) {

//	int game_type = GAME_TYPE_SINGLE;

	List rectlist_user;
	rectlist_user.len      = users->len;
	rectlist_user.size     = users->len;
	rectlist_user.elemSize = sizeof(SDL_Rect);
	rectlist_user.list     = malloc(rectlist_user.elemSize * rectlist_user.size);
	if( !(rectlist_user.list) ) {
		closeALL(window, renderer, texture, font, users, meteors, lasers);
		fprintf(stderr, "Erro ao inicializar lista (rect).\n");
		exit(ERROR_INIT_LIST);
	}

	List rectlist_meteor;
	rectlist_meteor.len      = meteors->len;
	rectlist_meteor.size     = meteors->len;
	rectlist_meteor.elemSize = sizeof(SDL_Rect);
	rectlist_meteor.list     = malloc(rectlist_meteor.elemSize * rectlist_meteor.size);
	if( !(rectlist_meteor.list) ) {
		free(rectlist_user.list);
		closeALL(window, renderer, texture, font, users, meteors, lasers);
		fprintf(stderr, "Erro ao inicializar lista (rect).\n");
		exit(ERROR_INIT_LIST);
	}

	List rectlist_laser;
	rectlist_laser.len      = lasers->len;
	rectlist_laser.size     = lasers->len;
	rectlist_laser.elemSize = sizeof(SDL_Rect);
	rectlist_laser.list     = malloc(rectlist_laser.elemSize * rectlist_laser.size);
	if( !(rectlist_laser.list) ) {
		free(rectlist_user.list);
		free(rectlist_meteor.list);
		closeALL(window, renderer, texture, font, users, meteors, lasers);
		fprintf(stderr, "Erro ao inicializar lista (rect).\n");
		exit(ERROR_INIT_LIST);
	}

	SDL_Rect* rect_user   = (SDL_Rect*)rectlist_user.list;
	SDL_Rect* rect_meteor = (SDL_Rect*)rectlist_meteor.list;
	SDL_Rect* rect_laser  = (SDL_Rect*)rectlist_laser.list;

	SDL_Texture* main_texture[TEXTURE_MAIN_NUM] = {NULL};
	int i;
	for( i = 0; i < TEXTURE_MAIN_NUM; i++ ) {
		getFromList(texture, i, &(main_texture[i]));
	}

	User user = {0}; // aux
	Obj obj   = {0}; // aux

	if( *game_type == GAME_TYPE_SINGLE ) {
		for( i = 0; i < users->len; i++ ) {
			getFromList(users, i, &user);

			if( !user.active ) {
				user.position.x = IMG_NAVE_W/2 + rand()%(WINDOW_SIZE_X - IMG_NAVE_W);
				user.position.y = IMG_NAVE_H/2 + rand()%(WINDOW_SIZE_Y - IMG_NAVE_H);
				user.active = 1;

				updateList(users, i, &user);
			}
		}
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
	int key[4] = {0}; // left right ....

	uint32_t shoot_time[2];
	uint32_t meteor_time[2];
	shoot_time[0] = 0;
	meteor_time[0] = 0;

	int scoreHasChanged = 0;

	getFromList(users, user_id, &user);

	SDL_Color color = {255,255,255,255};
	SDL_Texture* nome = LoadTxtTexture(renderer, font[FONT_INDEX_SML], user.nome, &color, texture);

	char char_score[MAX_DIGITS_SCORE];
	snprintf(char_score, MAX_DIGITS_SCORE, "%d", user.score);
	SDL_Texture* score = LoadTxtTexture(renderer, font[FONT_INDEX_MED], char_score, &color, texture);

	SDL_Rect rect_nome, rect_score;
	if(SDL_QueryTexture(nome, NULL, NULL, &(rect_nome.w), &(rect_nome.h)) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}

	if(SDL_QueryTexture(score, NULL, NULL, &(rect_score.w), &(rect_score.h)) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}
	rect_score.x = 20;
	rect_score.y = 20;

	runtime[1] = SDL_GetTicks();
	ctrlFramerate(runtime[1] - runtime[0]);

	while( *screen == SCREEN_GAME ) {
		runtime[0] = SDL_GetTicks();
		shoot_time[1] = SDL_GetTicks();
		meteor_time[1] = SDL_GetTicks();

		getFromList(users, user_id, &user);

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					free(rectlist_user.list);
					free(rectlist_meteor.list);
					free(rectlist_laser.list);
					closeALL(window, renderer, texture, font, users, meteors, lasers);
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
					if(event.key.keysym.sym == SDLK_ESCAPE) {
						if( *game_type == GAME_TYPE_SINGLE )	*screen = SCREEN_MENU;
					}
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

						if( *game_type == GAME_TYPE_SINGLE ) {
							addToList(lasers, &obj, OBJ_JUMPSIZE);
	
							SDL_Rect rect_aux = {0};
							addToList(&rectlist_laser, &rect_aux, 3);
							rect_laser = (SDL_Rect*)rectlist_laser.list;
						} else if( *game_type == GAME_TYPE_MULTI ) {
							sendServerInfo( &socket_d, NULL, &obj, user_id );
						}

						shoot_time[0] = shoot_time[1];
						shoot_time[1] = SDL_GetTicks();
				}
				break;
			}
		}

		if( *game_type == GAME_TYPE_SINGLE ) {
			if( (meteor_time[1] - meteor_time[0]) > CREATE_METEOR_INTERVAL ) {
				createMeteor(&obj, user.position);

	//			printf("%d %d %.2f\n", obj.position.x,obj.position.y, obj.ang);

				obj.initPoint.x = obj.position.x;
				obj.initPoint.y = obj.position.y;
				obj.dirVector.x = sin(obj.ang*M_PI/180) * MOVEMENT_INCREMENT_METEOR;
				obj.dirVector.y = -cos(obj.ang*M_PI/180) * MOVEMENT_INCREMENT_METEOR;
				obj.r = 0;
				obj.type = OBJ_TYPE_METEOR;
				addToList(meteors, &obj, OBJ_JUMPSIZE);

				SDL_Rect rect_aux = {0};
				addToList(&rectlist_meteor, &rect_aux, 3);
				rect_meteor = (SDL_Rect*)rectlist_meteor.list;

				meteor_time[0] = meteor_time[1];
				meteor_time[1] = SDL_GetTicks();
			}
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

		rect_nome.x = user.position.x - rect_nome.w/2;
		rect_nome.y = user.position.y + IMG_NAVE_H/2;

		updateList(users, user_id, &user);
		if( *game_type == GAME_TYPE_MULTI ) {
			sendServerInfo( &socket_d, &user, NULL, user_id );
			setServerInfo( &socket_d, users, meteors, lasers, &user_id );
		}

		i = lasers->len - 1;
		for( i = 0; i < lasers->len; i++ ) {
			if( getFromList(lasers, i, &obj) < 0 ) continue;

			obj.r++;

			obj.position.x = obj.initPoint.x + obj.dirVector.x * obj.r;
			obj.position.y = obj.initPoint.y + obj.dirVector.y * obj.r;

			if( insidePoint2( obj.position, -WINDOW_BORDER, -WINDOW_BORDER,
					WINDOW_SIZE_X + WINDOW_BORDER, WINDOW_SIZE_Y + WINDOW_BORDER ) ) {
				rect_laser[i].x = obj.position.x - laser_center.x;
				rect_laser[i].y = obj.position.y - laser_center.y;
				rect_laser[i].w = IMG_LASER_W;
				rect_laser[i].h = IMG_LASER_H;

				SDL_RenderCopyEx( renderer, main_texture[TEXTURE_LASER], NULL,
							&(rect_laser[i]), obj.ang, &laser_center, SDL_FLIP_NONE );

				updateList(lasers, i, &obj);

				// colisão com meteoro
				int j;
				for( j = 0; j < meteors->len; j++ ) {
					Obj meteor;
					if( getFromList(meteors, j, &meteor) < 0 ) continue;

					if( contact(obj.position, meteor.position, IMG_LASER_W/2, IMG_METEOR_W/2) ) {
						user.score += SCORE_DEST_METEOR;
						scoreHasChanged = 1;

						removeFromList( lasers, i );
						removeFromList( &rectlist_laser, i );
						removeFromList( meteors, j );
						removeFromList( &rectlist_meteor, j );
					}
				}
			} else {
				removeFromList( lasers, i );
				removeFromList( &rectlist_laser, i );
			}
		}

		updateList(users, user_id, &user);

		for( i = 0; i < meteors->len; i++ ) {
			if( getFromList(meteors, i, &obj) < 0 ) continue;

			obj.r++;

			obj.position.x = obj.initPoint.x + obj.dirVector.x * obj.r;
			obj.position.y = obj.initPoint.y + obj.dirVector.y * obj.r;

			if( insidePoint2( obj.position, -WINDOW_BORDER, -WINDOW_BORDER,
					WINDOW_SIZE_X + WINDOW_BORDER, WINDOW_SIZE_Y + WINDOW_BORDER ) ) {
				rect_meteor[i].x = obj.position.x - IMG_METEOR_W/2;
				rect_meteor[i].y = obj.position.y - IMG_METEOR_H/2;
				rect_meteor[i].w = IMG_METEOR_W;
				rect_meteor[i].h = IMG_METEOR_H;

				SDL_RenderCopy( renderer, main_texture[TEXTURE_METEOR], NULL, &(rect_meteor[i]) );

				updateList(meteors, i, &obj);
			} else {
				removeFromList( meteors, i );
				removeFromList( &rectlist_meteor, i );
			}
		}

		for( i = 0; i < users->len; i++ ) {
			if( getFromList(users, i, &user) < 0 ) continue;

			rect_user[i].x = user.position.x - nave_center.x;
			rect_user[i].y = user.position.y - nave_center.y;
			rect_user[i].w = IMG_NAVE_W;
			rect_user[i].h = IMG_NAVE_H;

			int j;
			for( j = 0; j < meteors->len; j++ ) {
				if( getFromList(meteors, j, &obj) < 0 ) continue;

				if( contact(user.position, obj.position, IMG_NAVE_W/2, IMG_METEOR_W/2) ) {
					removeFromList( meteors, j );
					removeFromList( &rectlist_meteor, j );
					removeFromList( users, i );
					removeFromList( &rectlist_user, i );
					*screen = SCREEN_GAMEOVER;
				}
			}
			if( *game_type == GAME_TYPE_MULTI ) {
				for( j = 0; j < lasers->len; j++ ) {
					if( getFromList(lasers, j, &obj) < 0 ) continue;

					if( contact(user.position, obj.position, IMG_NAVE_W/2, IMG_LASER_W/2) ) {
						removeFromList( lasers, j );
						removeFromList( &rectlist_laser, j );
						removeFromList( users, i );
						removeFromList( &rectlist_user, i );

						*screen = SCREEN_GAMEOVER;
					}
				}
			}

			SDL_RenderCopyEx( renderer, main_texture[TEXTURE_NAVE], NULL, &(rect_user[i]), user.ang, &nave_center, SDL_FLIP_NONE );
		}

		if( scoreHasChanged ) {
			SDL_DestroyTexture(score);
			getFromList(users, user_id, &user);
			snprintf(char_score, MAX_DIGITS_SCORE, "%d", user.score);
			score = LoadTxtTexture(renderer, font[FONT_INDEX_BIG], char_score, &color, texture);
			if(SDL_QueryTexture(score, NULL, NULL, &(rect_score.w), &(rect_score.h)) < 0) {
				fprintf(stderr, "%s\n", SDL_GetError());
			}
			scoreHasChanged = 0;
		}

		SDL_RenderCopy( renderer, score, NULL, &rect_score );
		SDL_RenderCopy( renderer, nome, NULL, &rect_nome );

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

		runtime[1] = SDL_GetTicks();
		ctrlFramerate((runtime[1] - runtime[0]));
	}

	free(rectlist_user.list);
	free(rectlist_meteor.list);
	free(rectlist_laser.list);

	destroyNonMainTexture(texture);
}
