#include"../header.h"

int addTexturePointer( struct texturePointers* texture, SDL_Texture* elem, int jumpSize ) {
	// adiciona um elemento à uma array ^ (ela aumenta de tamanho se necessário)
	if( jumpSize > 0 ) {
		if( texture->len < texture->size ) {
			texture->pointers[(texture->len)++] = elem;
			return 0;
		} else {
			texture->size += jumpSize;
			SDL_Texture** aux = realloc(texture->pointers, sizeof(SDL_Texture*)*(texture->size));
			if( aux ) {
				texture->pointers = aux;
				return addTexturePointer( texture, elem, jumpSize );
			} else {
				fprintf(stderr, "ERRO: addTexturePointer, realocation of memory failed.\n");
				return -2;
			}
		}
	}

	fprintf(stderr, "ERRO: addTexturePointer(), invalid argument jumpSize (%d).\n", jumpSize);
	return -1;
}

