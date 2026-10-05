#include "input/completion.h"
#include <dirent.h>
#include <parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int get_dir_names(char ***names, int *entries);
static int score(char **names, char *to_auto, int entries, int **scores);
static int get_max_indeces(int *scores, int entries, int **max_indeces);

int get_autocomplete_filepath(char *current_command, char **filepath,
                              size_t *to_delete) {

  int argc;
  char **argv;

  char *current_command_copy = strdup(current_command);

  if (current_command_copy == NULL) {
    perror("faliure in strdup");
    return -1;
  }

  tokenize(&argc, &argv, &current_command_copy, ' ');

  char *to_auto = argv[argc - 1];
  *to_delete = strlen(to_auto);

  int entries;
  char **names;
  get_dir_names(&names, &entries);

  int *scores;

  if (score(names, to_auto, entries, &scores) != 0) {
    perror("fail in score() function");
    return -1;
  }

  // just pick the first filepath for now
  int *max_indeces = NULL;

  if (get_max_indeces(scores, entries, &max_indeces) != 0) {
    perror("fail in get_max_indeces() function");
    return -1;
  }

  *filepath = NULL;
  *filepath = names[max_indeces[0]];

  return 0;
}

static int get_dir_names(char ***names, int *entries) {

  DIR *current_dir;

  if ((current_dir = opendir(".")) == NULL) {
    perror("Could not read current directory for autocomple");
    return -1;
  }

  *names = malloc(sizeof(char *));

  struct dirent *entry;
  *entries = 0;

  while ((entry = readdir(current_dir)) != NULL) {
    char **temp = realloc(*names, (*entries + 1) * sizeof **names);
    if (temp == NULL)
      return -1;
    *names = temp;

    (*names)[*entries] = strdup(entry->d_name);
    if ((*names)[*entries] == NULL)
      return -1;
    (*entries)++;
  }

  closedir(current_dir);

  return 0;
}

static int get_max_indeces(int *scores, int entries, int **max_indeces) {
  int max_score = 0;
  int num_max_scores = 0;

  int capacity = 16;
  *max_indeces = malloc(capacity * sizeof(int));

  for (int i = 0; i < entries; i++) {
    if (scores[i] > max_score)
      max_score = scores[i];
  }

  for (int i = 0; i < entries; i++) {
    if (max_score == scores[i]) {
      if (capacity == num_max_scores) {
        int *temp =
            realloc(*max_indeces, sizeof(max_indeces[0]) * capacity * 2);
        capacity *= 2;
        if (temp == NULL)
          perror("realloc() fail for max_indeces");

        *max_indeces = temp;
      }

      (*max_indeces)[num_max_scores++] = i;
    }
  }

  return 0;
}

static int score(char **names, char *to_auto, int entries, int **scores) {
  *scores = calloc(entries, sizeof(int));

  if (*scores == NULL) {
    perror("Malloc fail in score()");
    return -1;
  }

  for (size_t i = 0; i < entries; i++) {

    for (size_t j = 0; to_auto[j] && names[i][j]; j++) {

      if (to_auto[j] != names[i][j])
        break;

      (*scores)[i]++;
    }
  }

  return 0;
}
