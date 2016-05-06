/* Projeto Final Lab ICC 016
 * The Gray Spaceship
 * Carlos Roberto dos Santos Junior
 * Henry S. Suzukawa
 * Victor Rozzatti Tornisiello
 */

#include <stdio.h>
#include <math.h>

typedef struct nave {
 int x, y; // posição
 float ang; // angulo radiano
} Nave;

typedef struct objeto { // meteoro ou laser
 int x, y; // posição
 float ang; // coeficientes da reta que o obj vai seguir.. y = ax + b radiano
 int b;
} Obj;

int reta(a, b, x) {
 // https://www.codecogs.com/library/computing/c/math.h/round.php
 return round(a*x + b); // arredonda o resultado (0.5+ -> 1)
}

void draw(){
 // desenha o cenário
}

int main(){
 draw();
 
}
