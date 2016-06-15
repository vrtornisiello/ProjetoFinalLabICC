#include<SDL2/SDL.h>
#include<stdio.h>

int main() {
    SDL_Window *window = NULL;
    SDL_Surface *screen = NULL;
    SDL_Surface *nave = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Nao foi possivel inicializar a SDL: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    
    if(window == NULL) {
        fprintf(stderr, "Nao foi possivel abrir a janela: %s\n", SDL_GetError());
        return -2;
    }
    
    screen = SDL_GetWindowSurface(window);
    SDL_Rect stretchRect;
    nave = SDL_LoadBMP("nave_espacial.bmp");
    
    int i;
    for(i = 0; i < 500; i++) {
      stretchRect.x = 100 + i;
      stretchRect.y = 200 + i;
      stretchRect.w = 100;
      stretchRect.h = 100;

    
      if(nave == NULL) {
          fprintf(stderr, "Nao foi possivel abrir a imagem 'nave_espacial': %s\n", SDL_GetError());
          return -3;
      }
      
      SDL_BlitSurface(nave, NULL, screen, &stretchRect);
      SDL_UpdateWindowSurface(window);
      SDL_Delay(20);
    }
    SDL_FreeSurface(nave);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
