#include "workWithFiles.h"
#include <stdlib.h>
#include <stdio.h>


void parse_command(command_t command) {
    if (command.condition){ //try to create file
        create_file(command.path);
    }
    else{
        delete_file(command.path);
    }
}

void create_file(char* path){
    FILE* pf = fopen(path, "w");
    printf("File created:%s\n", path);
    fclose(pf);
}

void delete_file(char* path){
    if( remove( path ) != 0 ) {
        printf("Error with delete file:%s\n",path);
    } else {
        printf("File was deleted:%s\n",path);
    }
}
