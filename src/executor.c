#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#include "input.h"


void execute(char*** argv, char** cmd, int* argc)
{
    pid_t pid = fork();
                        

    if(pid == -1 ) perror("Fork function fail");
    
    if(pid == 0 ){
        int val = execvp((*argv)[0], *argv);
        if(val == -1) perror("Command fail");
    }else{  
        wait(NULL);
    }
    
    reset(argc, cmd, argv);

}
 