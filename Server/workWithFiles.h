#ifndef WORKWITHFILES_H_
#define WORKWITHFILES_H_

typedef enum Bool {
    FALSE,
    TRUE
} Bool;

struct command/*struct, which use server to contain clients commands*/
{
    Bool condition;//1 for create, 0 for delete
    char * path;//contains dir+"/"+filename
};
typedef struct command command_t;


void parse_command(command_t command);
void create_file(char* path);
void delete_file(char* path);


#endif