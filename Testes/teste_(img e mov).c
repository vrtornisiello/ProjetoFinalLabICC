#include<SDL.h>
#include<SDL_image.h>
#include<stdio.h>
#include<stdlib.h>http:
#include<time.h>http:
#include<math.h>http:

#define INIT_OBJ_SIZE 21
#define WINDOW_TITLE  "The Gray Spaceship"
#define WINDOW_SIZE_X 1024
#define WINDOW_SIZE_Y 576

typedef struct _point {
	int x;
	int y;
} Point;

int initSDL(SDL_Window** window, SDL_Renderer** renderer) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,
			"Nao foi possivel inicializar a SDL: %s\n", SDL_GetError());
		return -1;
	}

	*window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);
	if( !*window ) {
		SDL_Quit();
		fprintf(stderr,
			"Nao foi possivel abrir a janela: %s\n", SDL_GetError());
		return -1;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if( !*renderer ) {
		SDL_DestroyWindow(*window);
		SDL_Quit();

		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[]) {
    SDL_Window* window     = NULL;
	SDL_Renderer* renderer = NULL;

	if( initSDL( &window, &renderer ) < 0 ) {
		fprintf(stderr, "ERRO: inicializacao SDL.\n");
	}

    SDL_SetRenderDrawColor( renderer, 10, 10, 10, 255 );

    SDL_RenderClear( renderer );

	if( !IMG_Init( IMG_INIT_PNG ) ) {
		fprintf( stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return -2;
	}

	SDL_Surface* naveSf = IMG_Load("nave.png");
	SDL_Surface* meteoroSf = IMG_Load("meteoro.png");
	SDL_Surface* laserSf = IMG_Load("laser.png");
	if( !naveSf ) {
		fprintf( stderr, "Failed in create surface: %s\n", IMG_GetError() );
		return -2;
	}
	if( !meteoroSf ) {
		fprintf( stderr, "Failed in create surface: %s\n", IMG_GetError() );
		return -2;
	}
	if( !laserSf ) {
		fprintf( stderr, "Failed in create surface: %s\n", IMG_GetError() );
		return -2;
	}
	SDL_Texture* nave = SDL_CreateTextureFromSurface(renderer, naveSf);
	SDL_Texture* meteoro = SDL_CreateTextureFromSurface(renderer, meteoroSf);
	SDL_Texture* laser = SDL_CreateTextureFromSurface(renderer, laserSf);
	if( !nave ) {
		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}
	if( !meteoro ) {
		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}
	if( !laser ) {
		fprintf(stderr,
			"Nao foi possivel renderizar: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -2;
	}
	SDL_FreeSurface(naveSf);
	SDL_FreeSurface(meteoroSf);
	SDL_FreeSurface(laserSf);

    SDL_Event event;

    float xNave = 100, xMeteoro = -100, xLaser;
    float yNave = 100, yMeteoro = -100, yLaser;
    int xMouse, yMouse, AngM, AngL;
    int w = 0, s = 0, a = 0, d = 0;
    int laserMov = 0;

    while (1) {
    	SDL_Delay(5);
        SDL_PollEvent(&event);

        switch(event.type)
        {
			case SDL_QUIT:
				return 0;
	        break;

	        case SDL_KEYDOWN:
	        	if(event.key.keysym.sym == SDLK_w){
	        		w = 1;
	        	}
	        	if(event.key.keysym.sym == SDLK_s){
	        		s = 1;
	        	}
	        	if(event.key.keysym.sym == SDLK_a){
	        		a = 1;
	        	}
	        	if(event.key.keysym.sym == SDLK_d){
	        		d = 1;
	        	}
	        break;

	        case SDL_KEYUP:
	        	if(event.key.keysym.sym == SDLK_w){
	        		w = 0;
	        	}
	        	if(event.key.keysym.sym == SDLK_s){
	        		s = 0;
	        	}
	        	if(event.key.keysym.sym == SDLK_a){
	        		a = 0;
	        	}
	        	if(event.key.keysym.sym == SDLK_d){
	        		d = 0;
	        	}
	        break;

	        case SDL_MOUSEMOTION:
	        	SDL_GetMouseState(&xMouse, &yMouse);
	        	fprintf(stderr, "X: %d Y: %d\n", xMouse, yMouse);
	        break;
	    }

	    AngM = -90 + atan2((xNave+32 - yMouse), (yNave+32 - xMouse))*180/M_PI;
	    fprintf(stderr, "AngM: %d", AngM);

	    SDL_Rect rectL;
        if(event.type == SDL_MOUSEBUTTONDOWN){
            if(laserMov == 0){
                xLaser = xNave + 48;
                yLaser = yNave + 32;
                rectL.x = xLaser;
                rectL.y = yLaser;
                rectL.w = 30;
                rectL.h = 4;
                laserMov = 1;
            }
        }
        if(laserMov == 1){
            xLaser += 17.5;
        }
        if(xLaser > WINDOW_SIZE_X || xLaser < 0){
            laserMov = 0;
        }

        if(w == 1){
            yNave -= 1.5;
        }
        if(s == 1){
        	yNave += 1.5;
        }
        if(a == 1){
        	xNave -= 1.5;
        }
        if(d == 1){
        	xNave += 1.5;
        }

        SDL_Rect rectN;
        rectN.x = xNave;
        rectN.y = yNave;
        rectN.w = 64;
        rectN.h = 64;

        SDL_Rect rectM;
        rectM.x = xMeteoro;
        rectM.y = yMeteoro;
        rectM.w = 32;
        rectM.h = 32;
        xMeteoro += 2;
        yMeteoro += 2;

        rectL.x = xLaser;
        rectL.y = yLaser;

        SDL_Point p1 = {32, 32};
        SDL_Point p2 = {16, 16};
        SDL_Point p3 = {2, 15};

        SDL_RenderClear(renderer);
        SDL_RenderCopyEx( renderer, nave, NULL, &rectN, AngM,  &p1, SDL_FLIP_NONE );
        SDL_RenderCopyEx( renderer, meteoro, NULL, &rectM, 0,  &p2, SDL_FLIP_NONE );
        SDL_RenderCopyEx( renderer, laser, NULL, &rectL, AngL,  &p3, SDL_FLIP_NONE );
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(nave);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

