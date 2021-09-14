#include "workWithFiles.h"
#include <stdlib.h>
#include <stdio.h>

// Хотелось бы воспользоваться мьютексами, чтобы один поток полфайла не записал, а второй уже удалил, но пока не вижу вариантов
// Можно сет сделать, чтобы только один работал с Path уникальным.
// Но сет в C вроде бы не завозили, и на двусвязн. списках писать не хочется, поэтому имеем что имеем

void parse_command(command_t command) {
    if (command.condition){ //try to create file
        printf("\nCreating file:%s\n",command.path);
        create_file(command.path);
    }
    else{
        printf("\nDeleting file:%s\n",command.path);
        delete_file(command.path);
    }
}

void create_file(char* path){
    FILE* pf = fopen(path, "w");
    fclose(pf);
    printf("File created:%s\n", path);
}

void delete_file(char* path){
    if( remove( path ) != 0 ) {
        printf("Error with delete file:%s\n",path);
    } else {
        printf("File was deleted:%s\n",path);
    }
}
