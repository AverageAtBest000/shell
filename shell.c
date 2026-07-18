#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "cd.h"
#include "utils.h"


int main(void)
{

    char* cmd = NULL;
    int argc = 0;                   
    char** argv;
    char* delim = " ";

    while (1)
    {
        printf(">>> ");
        getcmd(&cmd);        
        tokenize(&argc, &argv, &cmd, delim);
        
        if(argc == 0 ){
            reset(&argc, &cmd, &argv);
            continue;
        }

        if(strcmp(argv[0], "exit") == 0) return 0;
        
        if(strcmp(argv[0], "cd") == 0 )
        {
            cd(argv, argc);
            continue;
        } 

        pid_t pid = fork();
                        

        if(pid == -1 ) perror("Fork function fail");

        if(pid == 0 ){

            int val = execvp(argv[0], argv);
            if(val == -1) perror("Command fail");

        }else{  
            wait(NULL);
        }


        reset(&argc, &cmd, &argv);
    
    }
    
    return 0;
}