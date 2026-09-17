#include <string.h>
#include <stdlib.h>

void tokenize(int* argc, char*** argv, char** cmd, char* delim)
{
    char* token;

    char* cmd_cpy = malloc(sizeof(char) * strlen(*cmd) + 1);

    strcpy(cmd_cpy , *cmd );
    
    token = strtok(*cmd, delim);

    for(int i = 0 ; token ; i++)
    {
        (*argv)[i] = token;
        token = strtok(NULL, delim);
    }

    (*argv)[*argc] = NULL;

    free(cmd_cpy);
}


int count_tokens(int* argc, char* cmd){


    bool in_string = false; 

    for(int i = 0; i < strlen(cmd); i++ ){

        if(strcmp(cmd[i], '"') && !in_string) in_string = !in_string;
        
        if(strcmp(cmd[i], ' ') && !in_string) num_tok++;
    }
    return num_tok;
}