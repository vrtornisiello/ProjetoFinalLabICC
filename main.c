/* Projeto Final Lab ICC 016
 * The Gray Spaceship
 * Carlos Roberto dos Santos Junior
 * Henry S. Suzukawa
 * Victor Rozzatti Tornisiello
 */

// https://wiki.libsdl.org/FrontPage
// http://lazyfoo.net/SDL_tutorials/
 
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

typedef struct nave {
	int x, y; // posição
 float ang; // angulo radiano
} Nave;

typedef struct objeto { // meteoro ou laser
 int x, y; // posição
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

void draw(){
 // desenha o cenário
}

int main()
{
 Nave nave;
 Obj meteoro[1];
 Obj laser;
 SDL_Event e; // Evento do SDL
 char init = 'r'; // Variável para manter o prog rodando
 while( init == 'r' ) {
  draw();
  
  while( SDL_PollEvent(&e) )
  {
   switch( e.type )
   {
    case SDL_QUIT:
     init = 'p';
     break;
   }
  }
  
 }
 close();
}
