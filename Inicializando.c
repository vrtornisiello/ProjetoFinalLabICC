#include<SDL.h>
#include<stdio.h>

int main(int argc, char *argv[]){

SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *nave = NULL;

if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("Nao foi possivel inicializar a SDL: %s\n", SDL_GetError());
}

window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

if(window == NULL){
    printf("Nao foi possivel abrir a janela: %s\n", SDL_GetError());
}

screen = SDL_GetWindowSurface(window);

nave = SDL_LoadBMP("nave_espacial.bmp");

if(nave == NULL){
    printf("Nao foi possivel abrir a imagem 'nave_espacial': %s\n", SDL_GetError());
}

SDL_BlitSurface(nave, NULL, screen, NULL);
SDL_UpdateWindowSurface(window);

SDL_Delay(2000);
SDL_FreeSurface(nave);
SDL_DestroyWindow(window);
SDL_Quit();

return 0;
}
