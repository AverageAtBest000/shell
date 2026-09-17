#include <string.h>
#include <stdlib.h>

void tokenize(int* argc, char*** argv, char** cmd, char delim)
{
    
    get_tokens(argc, argv, cmd, delim);

    (*argv)[*argc] = NULL;

    free(cmd_cpy);
}


void get_tokens(int* argc, char*** argv, char** cmd, char delim){

        *argv = malloc((*argc + 1) * sizeof(char*));

        char* cur_tok = malloc(1);
        cur_tok[0] = '\0';

        bool in_string = false;
        int j = 0; 

        for(int i = 0; i < strlen(cmd); i++){

            if(cmd[i] == '"') in_string = !in_string;

            if( i == strlen(cmd) && !(cmd[i] == delim) || (cmd[i] == delim && len(cur_tok) == 0 && !in_string) ){
                (*argv)[j++] = cur_tok;
                cur_tok[0] = '\0' ;
                continue;
            }

            if(cmd[i] != delim || in_string){
                
                char* temp = realloc( cur_tok, len(cur_tok) + 2 )
                temp[len(cur_tok)] = cmd[i];
                temp[len(cur_tok) + 1] = '\0';

                if(temp != NULL) cur_tok = temp;
            }

        }

        free(cur_tok);

}