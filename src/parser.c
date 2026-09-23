#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int get_tokens(int* argc, char*** argv, char** cmd, char delim);
static int append_to_vector(char*** argv, int* argc, char* token);

void tokenize(int* argc, char*** argv, char** cmd, char delim)
{
    if(get_tokens(argc, argv, cmd, delim) == -1){
        perror("Failed to tokenize command");
    }
}

int get_tokens(int* argc, char*** argv, char** cmd, char delim)
{
    bool in_string = false;
    bool token_started = false;
    size_t i = 0;
    size_t j = 0;

    *argc = 0;
    *argv = NULL;

    while((*cmd)[i] != '\0'){
        if((*cmd)[i] == '"'){
            if(!token_started){
                if(append_to_vector(argv, argc, &(*cmd)[j]) == -1)
                    return -1;
                token_started = true;
            }

            in_string = !in_string;
            i++;
            continue;
        }

        if((*cmd)[i] == delim && !in_string){
            if(token_started){
                (*cmd)[j++] = '\0';
                token_started = false;
            }

            i++;
            continue;
        }

        if(!token_started){
            if(append_to_vector(argv, argc, &(*cmd)[j]) == -1)
                return -1;
            token_started = true;
        }

        (*cmd)[j++] = (*cmd)[i++];
    }

    if(token_started)
        (*cmd)[j] = '\0';

    return 0;
}


static int append_to_vector(char*** argv, int* argc, char* token)
{
    char** temp = realloc(*argv, (*argc + 2) * sizeof(char*));
    if(temp == NULL) return -1;

    *argv = temp;
    (*argv)[*argc] = token;
    (*argc)++;
    (*argv)[*argc] = NULL;

    return 0;
}
