#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input.h"

char* commands[] = {"cd"};

bool is_builtin(char* cmd);
void cd( char** argv, int argc);



bool handleBuiltins(char** argv, int argc)
{

    if( !is_builtin(argv[0]) ) return false;

    if(strcmp(argv[0], "cd") == 0 ) cd(argv, argc);
    
    return true;

}

void cd( char** argv, int argc)
{
    if(argc > 2){
        printf("cd: too many arguments\n");
        return;
    }

    if(argc == 1){

        char* home = getenv("HOME");
        
        if(home == NULL){
            perror("Could not find HOME enviroment variable");
            return;
        } 

        if(chdir(home) != 0 ) perror("cd: Failed to change directory");

        return;
    }

    if(chdir( argv[1] ) != 0 )
        perror("cd: Failed to change directory");
    
}


bool is_builtin(char* cmd)
{
    int size = sizeof(commands) / sizeof(commands[0]);

    for(int i = 0 ; i < size; i++)
        if(strcmp(commands[i], cmd) == 0 )
            return true;

    return false;
}