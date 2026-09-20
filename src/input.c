#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include "parser.h"

ssize_t getcmd(char** cmd){
    
    printf("❯ ");
    size_t n = 10;
    ssize_t numchar = getline(cmd, &n, stdin);
    
    if(numchar == -1 ){
        
        if(feof(stdin)){
            return 0;
        }else{
            perror("Error in getline() function");          
            return -1;
        }
        
    } 

    if( numchar > 0 && (*cmd)[numchar - 1] == '\n') (*cmd)[numchar-1] = '\0';  
    
    return numchar;
}


static int get_autocomplete_filepath(char* current_command){
 
  int argc;
  char** argv

  if( tokenize(&arc, &argv, &current_command, '') != 0){
    perror("Failed to tokenize");
    return -1;
  }

  char* to_auto = argv[sizeof(argv)/sizeof(argv[0]) - 1);
  
  DIR* current_dir;

  if( (current_dir = opendir(".")) == NULL){
    perror("Could not read current directory for autocomple");
    return -1;
  }
   
  char** names = malloc(10*sizeof(char*));
  struct dirent* entry;
  int num_entries = 0;  

  while( (entry = readdir(current_dir)) != NULL )
  {
    if(num_entries > sizeof(names)/ sizeof(char*)){
      if(realloc(&num_entries, (sizeof(names)/ sizeof(char*)) * 2 ) == NULL){
        perror("realloc fail during autocomple")
        return -1;
      }

      names[num_entries] = entry->d_name;
    }

  }


  closedir(current_dir); 

  int entries = sizeof(names)/ sizeof(char*); 
  int scores[entries];

  for(int i = 0; i < entries; i++){

    for(int j = 0; j < strlen(to_auto); j++){
      if(to_auto[j] == names[i][j]) scores[i]++; 
    } 
  
  }

}

void reset(int* argc, char** cmd, char*** argv)
{
    *argc = 0;

    free(*cmd);
    *cmd = NULL;
    
    free(*argv);
    *argv = NULL;
}
