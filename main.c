/* Projeto Final Lab ICC 016
 * The Gray Spaceship
 * Carlos Roberto dos Santos Junior
 * Henry S. Suzukawa
 * Victor Rozzatti Tornisiello
 */

// https://wiki.libsdl.org/FrontPage
// http://lazyfoo.net/SDL_tutorials/

#define NAME "The Gray Spaceship"
#define WINDOWS_SIZE_X 800
#define WINDOWS_SIZE_Y 600
#define SIZE_X 800
#define SIZE_Y 600
#define LASER_SIZE_X 15
#define LASER_SIZE_Y 5

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

typedef struct coordinate {
	int x, y;
} Coordinate;

typedef struct nave {
	int x, y; // posição
	float ang; // angulo radiano
} Nave;

typedef struct objeto { // meteoro ou laser
	int x, y, x0, y0; // posição
	float coefA; // coeficientes da reta que o obj vai seguir.. y = ax + b radiano
	int coefL;
} Obj;

int reta(float coefA, int coefL, int x) {
	// https://www.codecogs.com/library/computing/c/math.h/round.php
	return round(coefA*x + coefL); // arredonda o resultado (0.5+ -> 1)
}

float calcTan(int x0, int y0, int x1, int y1) { // tangente dos pontos
	return (y1 - y0)/(x1 - x0);
}

float calcAng(int x0, int y0, int x1, int y1) { // angulo
	// http://www.cplusplus.com/reference/cmath/atan2/
	return atan2(x1 - x0, y1 - y0);
}

// Verifica a validade da posição
// char ocupa menos espaço do que int
char isValidPos( int x, int y ) {
	if( x < 0 ) {
		return 'n';
	}
	if( y < 0 ) {
		return 'n';
	}
	if( x > SIZE_X ) {
		return 'n';
	}
	if( y > SIZE_Y ) {
		return 'n';
	}
	return 'y';
}

void draw(){
	// desenha o cenário
}

void handleSDLEvents(Nave* nave, Obj* mouse, Obj* laser) {
	SDL_Event e; // Evento do SDL
	while( SDL_PollEvent(&e) ) {
		switch( e.type ) {
			case SDL_QUIT:
			init = 'p';
			break;
			
			case SDL_MOUSEMOTION:
			break;
			// https://wiki.libsdl.org/SDL_MouseButtonEvent
			case SDL_MOUSEBUTTONDOWN:
			break;
		}
	}
}

int main() {
	SDL_Window window;
	SDL_Renderer renderer;
	
	initSDL( &window, &renderer );
	
	Coordinate mouse;
	Nave nave;
	Obj meteoro[1];
	Obj laser;
	char init = 'r'; // Variável para manter o prog rodando
	while( init == 'r' ) {
		draw();
		handleSDLEvents(&nave, &mouse, &laser);
	}
	close();
}
