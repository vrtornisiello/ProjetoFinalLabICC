#include"../header.h"

void drawMenu( SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font[], clock_t* runtime ) {
	SDL_Color color = {255,255,255};
	SDL_Texture* menu_txt01 = LoadTxtTexture(renderer, font[0], "menu", &color, NULL);
	SDL_Texture* menu_txt02 = LoadTxtTexture(renderer, font[1], "SinglePlayer", &color, NULL);
	SDL_Texture* menu_txt03 = LoadTxtTexture(renderer, font[1], "MultiPlayer", &color, NULL);
	SDL_Texture* menu_txt04 = LoadTxtTexture(renderer, font[1], "Close", &color, NULL);

	int txtw = 100;
	int txth = 50;
	if(SDL_QueryTexture(menu_txt01, NULL, NULL, &txtw, &txth) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}
//	SDL_SetRenderDrawColor(renderer, 200,0,0,255);
//	SDL_RenderFillRect(renderer, &rect_txt01);
	SDL_Rect rect_txt01;
	rect_txt01.x = 100;
	rect_txt01.y = 200;
	rect_txt01.h = txth;
	rect_txt01.w = txtw;
	SDL_RenderCopy(renderer, menu_txt01, NULL, &rect_txt01);

	if(SDL_QueryTexture(menu_txt02, NULL, NULL, &txtw, &txth) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
	}
	SDL_Rect rect_txt02;
	rect_txt02.x = 150;
	rect_txt02.y = 250;
	rect_txt02.h = txth;
	rect_txt02.w = txtw;
	SDL_RenderCopy(renderer, menu_txt02, NULL, &rect_txt02);
	SDL_RenderPresent(renderer);

	if(menu_txt01) SDL_DestroyTexture(menu_txt01);
	if(menu_txt02) SDL_DestroyTexture(menu_txt02);
	if(menu_txt03) SDL_DestroyTexture(menu_txt03);
	if(menu_txt04) SDL_DestroyTexture(menu_txt04);
}
