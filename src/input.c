#include <stdio.h>
#include <stdlib.h>

ssize_t getcmd(char** cmd){
    
    printf("❯ ");
    size_t n = 10;
    ssize_t numchar = getline(cmd, &n, stdin);
    
    if(numchar == -1 && ferror(stdin)){
        perror("Error in getline() function");  
        return -1;
    } else if(feof(stdin)){
        *(cmd) = "";
        return 0;
    }

    if( numchar > 0 && (*cmd)[numchar - 1] == '\n') (*cmd)[numchar-1] = '\0';  
    
    return numchar;
}


void reset(int* argc, char** cmd, char*** argv)
{
    *argc = 0;

    free(*cmd);
    *cmd = NULL;
    
    free(*argv);
    *argv = NULL;
}
