#include"../header.h"

int saveUser( List* users, List* meteors, List* lasers, char* path ) {
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

	struct save_struct save = {1, meteors->len, lasers->len};

	if( fwrite(&save, sizeof(save), 1, fd) != 1 ) {
		fprintf(stderr, "Houve algum problema ao escrever o arquivo (struct).\n");
		fclose(fd);
		return 1;
	}

	if( fwrite(&user, sizeof(user), 1, fd) != 1 ) {
		fprintf(stderr, "Houve algum problema ao escrever o arquivo (user).\n");
		fclose(fd);
		return 1;
	}

	if( fwrite( meteors->list, meteors->elemSize, meteors->len, fd ) != meteors->len )
		fprintf(stderr, "Houve algum problema ao escrever o arquivo (meteoros).\n");

	if( fwrite( lasers->list, lasers->elemSize, lasers->len, fd ) != lasers->len )
		fprintf(stderr, "Houve algum problema ao escrever o arquivo (lasers).\n");

	fclose(fd);
	return 0;
}

int openUser( List* users, List* meteors, List* lasers, char* path, int flags ) {
	FILE* fd = fopen(path, "r");
	if(!fd)	{
		fprintf(stderr, "Erro ao abrir o arquivo");
		return -2;
	}
	if( (users->len == 0) || (OPEN_OVERWRITE & flags) ) {
		if(users->len > 0) { // remove os usuários
			int i;
			for( i = 0; i < users->len; i++ ) {
				removeFromList(users, i);
			}
		}

		if(meteors->len > 0) { // remove os usuários
			int i;
			for( i = 0; i < meteors->len; i++ ) {
				removeFromList(meteors, i);
			}
		}

		if(lasers->len > 0) { // remove os usuários
			int i;
			for( i = 0; i < lasers->len; i++ ) {
				removeFromList(lasers, i);
			}
		}

		struct save_struct save = {0};

		if( fread(&save, sizeof(save), 1, fd) != 1 ) {
			fprintf(stderr, "Houve algum problema ao ler o arquivo (struct).\n");
			fclose(fd);
			return 1;
		}

		if( save.len_users > 0 ) {
			User user = {0};
			while( save.len_users && (fread(&user, sizeof(user), 1, fd) == 1) ) {
				if( addToList(users, &user, 1) < 0) {
					fprintf(stderr, "Houve um erro ao adicionar o usuario.\n");
					return -3;
				}
				save.len_users--;
			}
			if( save.len_users > 0 ) {
				fprintf(stderr, "Houve algum problema ao ler o arquivo.\n");
				fclose(fd);
				return -2;
			}

			Obj obj;
			while( save.len_meteors && (fread( &obj, sizeof(obj), 1, fd ) == 1) ) {
				addToList(meteors, &obj, OBJ_JUMPSIZE);
				save.len_meteors--;
			}
			while( save.len_lasers && (fread( &obj, sizeof(obj), 1, fd ) == 1) ) {
				addToList(lasers, &obj, OBJ_JUMPSIZE);
				save.len_lasers--;
			}
			fclose(fd);
			return 0;
		}

		fclose(fd);
		fprintf(stderr, "Há um erro no arquivo.\n");
		return -2;
	}

	fclose(fd);
	fprintf(stderr, "Há um jogo ativo.\n");
	return -1;
}
