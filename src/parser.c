#include <string.h>
#include <stdlib.h>

void tokenize(int* argc, char*** argv, char** cmd, char delim)
{
    
    if( get_tokens(argc, argv, cmd, delim) == -1){
        perror( "Fail in get_tokens() function")
    }

    free(cmd_cpy);
}


int get_tokens(int* argc, char*** argv, char** cmd, char delim){

        *argv = malloc(sizeof(char*));

        char* cur_tok = malloc(1);
        cur_tok[0] = '\0';

        bool in_string = false;
        int j = 0; 

        char** temp;

        for(int i = 0; i < strlen(cmd); i++){

            if(cmd[i] == '"') in_string = !in_string;

            if( i == strlen(cmd) && !(cmd[i] == delim) || (cmd[i] == delim && len(cur_tok) == 0 && !in_string) ){

                if(append_to_vector(argv, argc, temp) == -1 ) return -1
                
                continue;
            }
            

            if(cmd[i] != delim || in_string){
                
                if(append_to_cur_tok(&cur_tok, cmd[i]) == -1) return -1;
            }
            
        }
        
        
        argv[];

        free(temp);
        free(cur_tok);

}


int append_to_vector(char*** argv, char** cur_tok, char** temp){

    temp = realloc( argv, sizeof(argv) + sizeof(char*) );
    if(temp == NULL) return -1; 

    temp[sizeof(temp)/sizeof(char*) - 1] = cur_tok;

    (*argc)++;
    cur_tok[0] = '\0';


}


int append_to_cur_tok(char** cur_tok, char to_append){

    char* temp = realloc( *cur_tok, len(cur_tok) + 2 )

    temp[len(cur_tok)] = to_append;
    temp[len(cur_tok) + 1] = '\0';
    
    if(temp != NULL) cur_tok = temp;
    else return -1;

}