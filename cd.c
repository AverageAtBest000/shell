#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void cd( char** argv, int argc)
{
    if(argc > 2){
        printf("cd: too many arguments\n");
        return;
    }

    if(argc == 1){

        char* home = getenv("HOME");
        if(home == NULL) perror("Could not find HOME enviroment variable");

        if(chdir(home) != 0 ){

            perror("cd: Failed to change directory");
        
        }
    }

    if(chdir( argv[1] ) != 0 )
        perror("cd: Failed to change directory");
    
}