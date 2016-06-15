#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

typedef struct _point {
	int x;
	int y;
} Point;

int main()
{
    // variables
    SDL_Event event;
    Point p;
    int x = 100;
    int y = 100;
    int xMouse, yMouse, Ang;
    int kup = 0, kdown = 0, kleft = 0, kright = 0;

    // init SDL

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("SDL2 Keyboard/Mouse events", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 576, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface * image = SDL_LoadBMP("img/nave.bmp");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    // handle events

    while (1)
    {
    	SDL_Delay(5);
        SDL_PollEvent(&event);

        switch(event.type)
        {
			case SDL_QUIT:
				return 0;
	        break;

	        case SDL_KEYDOWN:
	        	if(event.key.keysym.sym == SDLK_UP)
	        	{
	        		kup = 1;

	        	}
	        	if(event.key.keysym.sym == SDLK_DOWN)
	        	{
	        		kdown = 1;
	        	}
	        	if(event.key.keysym.sym == SDLK_LEFT)
	        	{
	        		kleft = 1;
	        	}
	        	if(event.key.keysym.sym == SDLK_RIGHT)
	        	{
	        		kright = 1;
	        	}
	        break;

	        case SDL_KEYUP:
	        	if(event.key.keysym.sym == SDLK_UP)
	        	{
	        		kup = 0;

	        	}
	        	if(event.key.keysym.sym == SDLK_DOWN)
	        	{
	        		kdown = 0;
	        	}
	        	if(event.key.keysym.sym == SDLK_LEFT)
	        	{
	        		kleft = 0;
	        	}
	        	if(event.key.keysym.sym == SDLK_RIGHT)
	        	{
	        		kright = 0;
	        	}
	        break;

	        case SDL_MOUSEMOTION:
	        	SDL_GetMouseState(&xMouse, &yMouse);
	        	fprintf(stderr, "X: %d Y: %d\n", xMouse, yMouse);
	        break;
	    }

	    Ang = -90 + atan2((y + 32 - yMouse), ((x + 32 - xMouse)))*180/M_PI;
	    fprintf(stderr, "%d\n", Ang);

        if(kup == 1)
        {
        	y--;
        }
        if(kdown == 1)
        {
        	y++;
        }
        if(kleft == 1)
        {
        	x--;
        }
        if(kright == 1)
        {
        	x++;
        }

        SDL_Rect dstrect = { x, y, 64, 64 };
        p.x = 32;
        p.y = 32;

        SDL_RenderClear(renderer);
        SDL_RenderCopyEx( renderer, texture, NULL, &dstrect, Ang, (SDL_Point*)&p , SDL_FLIP_NONE );
        SDL_RenderPresent(renderer);
    }

    // cleanup SDL

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
