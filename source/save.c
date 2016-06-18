#include"../header.h"

int saveUser( List* users, List* objs, char* path ) {
	User user;
	if( getFromList( users, 0, &user ) < 0 ) {
		fprintf(stderr, "Erro ao resgatar usuario.\n");
		return -1;
	}
	FILE* fd = fopen(path, "w");
	if(!fd)	{
		fprintf(stderr, "Erro ao abrir o arquivo");
		return -2;
	}

	if( fwrite(&user, sizeof(user), 1, fd) != 1 ) {
		fprintf(stderr, "Houve algum problema ao escrever o arquivo (user).\n");
		fclose(fd);
		return 1;
	}

	int i;
	if( fwrite( objs->list, objs->elemSize, objs->len, fd ) != objs->len )
		fprintf(stderr, "Houve algum problema ao escrever o arquivo (obj).\n");

	fclose(fd);
	return 0;
}

int openUser( List* users, List* objs, char* path, int flags ) {
	FILE* fd = fopen(path, "r");
	if(!fd)	{
		fprintf(stderr, "Erro ao abrir o arquivo");
		return -2;
	}
	if( ((users->len == 0) && (objs->len == 0)) || (OPEN_OVERWRITE & flags) ) {
		if(users->len > 0) { // remove os usuários
			int i;
			for( i = 0; i < users->len; i++ ) {
				removeFromList(users, i);
			}
		}

		if(objs->len > 0) { // remove os usuários
			int i;
			for( i = 0; i < objs->len; i++ ) {
				removeFromList(objs, i);
			}
		}

		User user = {0};
		if( fread(&user, sizeof(user), 1, fd) != 1 ) {
			fprintf(stderr, "Houve algum problema ao ler o arquivo.\n");
			fclose(fd);
			return -2;
		}
		if( addToList(users, &user, 1) < 0) {
			fprintf(stderr, "Houve um erro ao adicionar o usuario.");
			return -3;
		}

		Obj obj;
		while( fread( &obj, sizeof(obj), 1, fd ) == 1 ) {
			addToList(objs, &obj, OBJ_JUMPSIZE);
		}
		fclose(fd);
		return 0;
	}

	fclose(fd);
	fprintf(stderr, "Há um jogo ativo.\n");
	return -1;
}
