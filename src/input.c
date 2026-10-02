#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

static int get_max_indeces(int* scores, int entries,  int** max_indeces);
static int get_autocomplete_filepath(char* current_command, char** filepath, size_t* to_delete);
static int score(char** names, char* to_auto, int entries, int** scores);
static int get_dir_names( char*** names, int* entries);
static int delete_from_userin(int num_chars);


ssize_t get_cmd_cannonical(char** cmd){
    
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

ssize_t get_cmd_noncannonical(char** cmd){
  
  printf("❯ ");
  fflush(stdout);

  int capacity = 16;
  *cmd = malloc(sizeof(char*) * capacity);

  if( *cmd == NULL ){
    perror("Failiure in malloc(). Could not allocate space for cmd");
    return -1;
  }

  char ch;
  ssize_t num_char = 0;

  // read() will return a \r once the user hits Enter
  while(1){
   
    if(num_char >= capacity){
      if(realloc(*cmd, capacity*2) == NULL){
        perror("Failiure in realloc(). Could not allocate command buffer");
        return -1;
      } 
      capacity *= 2;
    }
     
    if(read(STDIN_FILENO, &ch, 1 ) == -1){
      perror("Error in read() operation"); 
      return -1;
    }

    if(ch == '\n' || ch == '\r'){
      (*cmd)[num_char] = '\0';
      break; 
    } 

    if(ch == '\b' || ch == 8 || ch == 127) {
      if(num_char > 0){
        num_char --;
        if(delete_from_userin(1) == -1){
          return -1; 
        }
      }
      continue;
    }
    
    if((ch == '\t' || ch == 9) && num_char != 0){

      char* filepath;
      size_t to_delete;

      (*cmd)[num_char] = '\0';

      if(get_autocomplete_filepath(*cmd, &filepath, &to_delete) != 0){
        return -1;
      }

      delete_from_userin(to_delete);
      num_char -= to_delete;
      (*cmd)[num_char] = '\0';
      
      size_t to_write = strlen(filepath);
      write(STDOUT_FILENO, filepath, to_write);
      continue;
    }


    write(STDOUT_FILENO, &ch, 1);
    (*cmd)[num_char] = ch;
    num_char++;
  }
  return num_char;
}



static int delete_from_userin( int num_chars){
  while(num_chars --> 0 ){
    if(write(STDOUT_FILENO, "\033[D \033[D", 7) == -1){
      perror("Error in write() operation"); 
      return -1;
    }
  }

  return 0;
}

static int get_autocomplete_filepath(char* current_command, char** filepath, size_t* to_delete ){
 
  int argc;
  char** argv;

  char* current_command_copy = strdup(current_command);

  if(  current_command_copy == NULL ){
    perror("faliure in strdup");
    return -1;
  }

  tokenize(&argc, &argv, &current_command_copy, ' ');
 
  char* to_auto = argv[argc - 1];
  *to_delete = strlen(to_auto);
  
  int entries;
  char** names;
  get_dir_names(&names, &entries);

  int* scores;

  if(  score(names, to_auto, entries, &scores) != 0){
    perror("fail in score() function");
    return - 1;
  }
 
  // just pick the first filepath for now 
  int* max_indeces = NULL;

  if(get_max_indeces(scores, entries, &max_indeces) != 0){
    perror("fail in get_max_indeces() function"); 
    return -1;  
  } 
  
  *filepath = NULL;
  *filepath = names[max_indeces[0]];

  return 0; 
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
      char **temp = realloc(*names, (*entries + 1) * sizeof **names);
      if (temp == NULL) return -1;
      *names = temp;

      (*names)[*entries] = strdup(entry->d_name);
      if ((*names)[*entries] == NULL) return -1;
      (*entries)++;
  }

  closedir(current_dir);  

  return 0; 
}


static int get_max_indeces(int* scores, int entries,  int** max_indeces){
  int max_score = 0;
  int num_max_scores = 0;

  int capacity = 16;  
  *max_indeces = malloc(capacity * sizeof(int));

  for(int i = 0; i < entries; i++){
    if(scores[i] > max_score)
      max_score = scores[i];
  }

  for(int i = 0 ; i < entries; i++){
    if(max_score == scores[i]){
      if(capacity == num_max_scores){
        int* temp = realloc(*max_indeces, sizeof(max_indeces[0]) * capacity * 2);
        capacity *= 2;
        if(temp == NULL)
          perror("realloc() fail for max_indeces");

        *max_indeces = temp;
      }

      (*max_indeces)[num_max_scores++] = i;
    }
  }

  return 0;
}


static int score(char** names, char* to_auto, int entries, int** scores){
  *scores = calloc(entries, sizeof(int));
  
  if(*scores == NULL){
    perror("Malloc fail in score()");
    return -1;
  }

  for(size_t i = 0; i < entries; i++){

    for( size_t j = 0; to_auto[j] && names[i][j]; j++){
      
      if(to_auto[j] != names[i][j]) break; 

      (*scores)[i]++;
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
