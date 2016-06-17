#include"../header.h"

int insidePoint( Point p, SDL_Rect* rect ) {
	if( p.x > rect->x ) {
		if( p.x < rect->x + rect->w ) {
			if( p.y > rect->y ) {
				if( p.y < rect->y + rect->h ) return 1;
			}
		}
	}
	return 0;
}
