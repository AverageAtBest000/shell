#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "parser.h"

static int get_max_indeces(int* scores, int entries,  int** max_indeces);
static int get_autocomplete_filepath(char* current_command, char** filepath);
static int score(char** names, char* to_auto, int entries, int** scores);
static int get_dir_names( char*** names, int* entries);

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
  char* current_command_copy;
  if(strcpy(current_command_copy, current_command) != 0 ){
    perror("Failed to copy current_command");
    return -1;
  }

  tokenize(&argc, &argv, &current_command_copy, ' ');
 
  char* to_auto = argv[ argc - 1];

  int entries;
  char** names;
  get_dir_names(&names, &entries);

  int* scores;

  if(  score(names, to_auto, entries, &scores) != 0){
    perror("fail in score() function");
    return - 1;
  }
 
  // just pick the first filepath for now 
  int* max_indeces;

  if(get_max_indeces(scores, entries, &max_indeces) != 0){
    perror("fail in get_max_indeces() function"); 
    return -1;  
  } 
  
  *filepath = NULL;
  
  *filepath = names[max_indeces[0]];
  

}

static int get_dir_names( char*** names, int* entries){
  
  DIR* current_dir;

  if( (current_dir = opendir(".")) == NULL){
    perror("Could not read current directory for autocomple");
    return -1;
  }
   

  *names = malloc(sizeof(char*));

  struct dirent* entry;
  *entries = 0;  

  while( (entry = readdir(current_dir)) != NULL )
  {
      if(realloc(names, *entries + 1  ) == NULL){
        perror("realloc fail during autocomple");
        return -1;
      }

      *names[*entries] = entry->d_name;
      (*entries) ++; 
  }

  closedir(current_dir); 
  

}


static int get_max_indeces(int* scores, int entries,  int** max_indeces){
  int max_score = 0;

  for(int i = 0; i < entries; i++)
  {
    if(scores[i] > max_score || max_score == scores[i]){
      
      max_score = scores[i];
      int* temp = realloc(max_indeces, entries);
      entries++;
      
      if(temp == NULL){
        perror("realloc() faliure in get_max_indeces() function");
        return -1;
      }

      temp[sizeof(temp) / sizeof(int*)] = i;

     *max_indeces = temp; 
    }
  }

  return 0;
}


static int score(char** names, char* to_auto, int entries, int** scores){
  
  if(*scores = malloc(entries * sizeof(int))){
    perror("Malloc fail in score()");
    return -1;
  }

  for(int i = 0; i < entries; i++){

    for(int j = 0; j < strlen(to_auto); j++){
      
      if(to_auto[j] == names[i][j]) scores[i]++;
      else continue;
    }  
  }

  return 0;
}


void reset(int* argc, char** cmd, char*** argv)
{
    *argc = 0;

    free(*cmd);
    *cmd = NULL;
    
    free(*argv);
    *argv = NULL;
}
