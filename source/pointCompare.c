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

int insidePoint2( Point p, int x, int y, int w, int h ) {
	if( p.x > x ) {
		if( p.x < x + w ) {
			if( p.y > y ) {
				if( p.y < y + h ) return 1;
			}
		}
	}
	return 0;
}
