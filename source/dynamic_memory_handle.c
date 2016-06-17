#include"../header.h"

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
