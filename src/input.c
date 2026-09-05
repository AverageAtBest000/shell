#include <stdio.h>
#include <stdlib.h>

void getcmd(char** cmd){
    
    printf("-> ");
    size_t n = 10;
    ssize_t numchar = getline(cmd, &n, stdin);
    
    if(numchar == -1){
        perror("Error in getline() function");  
        return;
    } 

    if( numchar > 0 && (*cmd)[numchar - 1] == "\n") (*cmd)[numchar-1] = '\0';  
     
}


void reset(int* argc, char** cmd, char*** argv)
{
    *argc = 0;

    free(*cmd);
    *cmd = NULL;
    
    free(*argv);
    *argv = NULL;
}
