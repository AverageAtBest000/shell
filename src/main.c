#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "input.h"
#include "executor.h"
#include "builtins.h"



int main(void)
{

    char* cmd = NULL;
    int argc = 0;                   
    char** argv;
    char* delim = " ";

    while (1)
    {

        getcmd(&cmd);        
        tokenize(&argc, &argv, &cmd, delim);
        
        if(arc == 0 ){
            reset(&argc, &cmd, &argv);
            continue;
        }
        
        if(strcmp(argv[0], "exit") == 0) return 0;

        bool handled = handleBuiltins(&argv, &argc, &cmd);

       
        if(!handled){
            execute(argv)
        }
       
    
    }
    
    return 0;
}