#include"../header.h"
/*
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
*/

int addToList( List* list, void* elem, int jumpSize ) {
	if( jumpSize > 0 ) {
		if( list->len < list->size ) {
			memcpy((list->list + (list->len)*(list->elemSize)), elem, list->elemSize);
			(list->len)++;
			return 0;
		} else {
			list->size += jumpSize;
			void* aux = realloc(list->list, (list->size)*(list->elemSize));
			if( aux ) {
				list->list = aux;
				return addToList(list, elem, jumpSize);
			} else {
				fprintf(stderr, "ERRO: addToList, realocation of memory failed.\n");
				return -2;
			}
		}
	}

	fprintf(stderr, "ERRO: addToList(), invalid argument jumpSize (%d).\n", jumpSize);
	return -1;
}

int removeFromList( List* list, int elemId ) {
	if( list->len > elemId ) {
		list->len--;
		memcpy((list->list + (elemId)*(list->elemSize)), (list->list + (list->len)*(list->elemSize)), list->elemSize);
		memset((list->list + (list->len)*(list->elemSize)), 0, list->elemSize);
		return 0;
	}
	fprintf(stderr, "ERRO: removeFromList(), invalid argument elemId (%d).\n", elemId);
	return -1;
}
