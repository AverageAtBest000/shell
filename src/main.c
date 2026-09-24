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

static int set_raw_terminal( struct termios* old_attr, struct termios* new_attr);
static int set_cannonical_terminal(struct termios* old_attr);


int main(void)
{
      
    struct termios old_attr, new_attr;
  
    if(set_raw_terminal(&old_attr, &new_attr) != 0 ){
        return -1;
    }

    char* cmd = NULL;
    int argc = 0;                   
    char** argv = NULL;
    char delim = ' ';
    ssize_t cmdReadResult;
    
    while (1)
    {     
        cmdReadResult = get_cmd_noncannonical(&cmd);

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
    
    if(set_cannonical_terminal(&old_attr) != 0){    
      return -1;
    }
    
    return 0;
}


static int set_raw_terminal( struct termios* old_attr, struct termios*  new_attr){

  if(tcgetattr(STDIN_FILENO, old_attr) != 0){
    perror("Fail in tcgetattr(). Could not fetch current terminal attributes");
    return -1;
  }

  *new_attr = *old_attr;
  new_attr->c_lflag &= ~ICANON;

  if(tcsetattr(STDIN_FILENO, TCSANOW, new_attr) != 0){
    perror("Fail in tcsetattr(). Could not go to cannonical mode");
    return -1;
  }
  
  return 0; 

}

static int set_cannonical_terminal(struct termios* old_attr ){
 

  if(tcsetattr(STDIN_FILENO, TCSANOW, old_attr) != 0){
    perror("Fail in tcsetattr(). Could not return to cannonical mode");
    return -1;  
  }

 return 0;  

}
