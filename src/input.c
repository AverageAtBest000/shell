#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "parser.h"

static int[] score( char** names, int entries);
static int* get_max_indeces(int[] scores);
static int get_autocomplete_filepath(char* current_command, char** filepath);


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


static int get_autocomplete_filepath(char* current_command, char** filepath  ){
 
  int argc;
  char** argv;

  // if( tokenize(&argc, &argv, &current_command, ' ') != 0){
  //   perror("Failed to tokenize");
  //   return -1;
  // }
  //
  tokenize(&argc, &argv, &current_command, ' ');
 
  char* to_auto = argv[ argc - 1];
  
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
        perror("realloc fail during autocomple");
        return -1;
      }

      names[num_entries] = entry->d_name;
    }

  }


  closedir(current_dir); 
  
  int entries = sizeof(names)/ sizeof(char*); 
  score(names, entries);
 
  // just pick the first filepath for now 
  int* max_indeces = get_max_indeces(scores); 
  
  *filepath = NULL;
  
  *filepath = names[max_indeces[0]];

  

}

static int* get_max_indeces(int[] scores){
  int max_score = 0;
  int* max_indeces; 

  for(int i = 0; i < entries; i++)
  {
    if(scores[i] > max_score || max_score == scores[i]){
      
      max_score = scores[i];
      int* temp = realloc(max_indeces, sizeof(max_indeces) + sizeof(int*));
      
      if(temp == NULL){
        perror("realloc() faliure in get_max_indeces() function"
        return NULL;
      }

      temp[sizeof(temp) / sizeof(int*)] = i;
    }
  }

  return max_indeces;

}


static int[] score(char** names, int entries){
  
  int scores[entries];

  for(int i = 0; i < entries; i++){

    for(int j = 0; j < strlen(to_auto); j++){
      
      if(to_auto[j] == names[i][j]) scores[i]++;
      else continue;
    } 
  
  }

  return scores;
}


void reset(int* argc, char** cmd, char*** argv)
{
    *argc = 0;

    free(*cmd);
    *cmd = NULL;
    
    free(*argv);
    *argv = NULL;
}
