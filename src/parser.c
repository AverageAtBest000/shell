#include <string.h>
#include <stdlib.h>

void tokenize(int* argc, char*** argv, char** cmd, char* delim)
{
    char* token;

    char* cmd_cpy = malloc(sizeof(char) * strlen(*cmd) + 1);

    strcpy(cmd_cpy , *cmd );
    
    token = strtok(cmd_cpy, delim); 
    *argc = 0;

    while (token)
    {
        (*argc)++;
        token = strtok(NULL, delim);
    }

    *argv = malloc((*argc + 1) * sizeof(char*));
    token = strtok(*cmd, delim);

    for(int i = 0 ; token ; i++)
    {
        (*argv)[i] = token;
        token = strtok(NULL, delim);
    }

    (*argv)[*argc] = NULL;

    free(cmd_cpy);
}