#include <stdio.h>
#include <stdlib.h>

void getcmd(char** cmd){
    
    printf("-> ");
    size_t n = 10;
    ssize_t numchar = getline(cmd, &n, stdin);
    (*cmd)[numchar-1] = '\0';  
     
}


void reset(int* argc, char** cmd, char*** argv)
{
    *argc = 0;

    free(*cmd);
    *cmd = NULL;
    
    free(*argv);
    *argv = NULL;
}
