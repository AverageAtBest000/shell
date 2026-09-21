#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include "parser.h"
#include "input.h"
#include "executor.h"
#include "builtins.h"

static int set_raw_terminal();

int main(void)
{
  
    if(set_raw_terminal() != 0 ){
        perror("could not set_raw_terminal mode");
        return -1;
    }


    char* cmd = NULL;
    int argc = 0;                   
    char** argv = NULL;
    char delim = ' ';
    ssize_t cmdReadResult;
    
    while (1)
    {     
        cmdReadResult = getcmd(&cmd);

        if( cmdReadResult == -1){
            reset(&argc, &cmd, &argv);
            continue;  
        }else if(cmdReadResult == 0){
            reset(&argc, &cmd, &argv);
            return 0;
        }
    
        tokenize(&argc, &argv, &cmd, delim);
        
        if(argc == 0 ){
            reset(&argc, &cmd, &argv);
            continue;
        }

        if(strcmp(argv[0], "exit") == 0){
            reset(&argc, &cmd, &argv);
            return 0;
        }

        bool handled = handleBuiltins(argv, argc);

       
        if(!handled){
            execute(&argv, &cmd, &argc);
        }

        reset(&argc, &cmd, &argv);
       
     
    }
    
    return 0;
}

 static int set_raw_terminal(){
  struct termios old_attr, new_attr;

  if(tcgetattr(STDIN_FILENO, &old_attr) != 0){
    perror("tcgetattr() fail");
    return -1;
  }

  new_attr = old_attr;
  new_attr.c_lflag &= ~ICANON;

  if(tcsetattr(STDIN_FILENO, TCSANOW, &new_attr) != 0){
    perror("tcgetattr() fail");
    return -1;
  }
  
  return 0; 

}
