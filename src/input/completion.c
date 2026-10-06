#include "input/completion.h"
#include "ds_array.h"
#include "ds_common.h"
#include "ds_string.h"
#include <dirent.h>
#include <parser.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DS_DEFINE_ARRAY(int, ds_int_array, NULL, NULL)

static int get_dir_names(ds_array *dir_names);
static int get_max_indeces(ds_int_array scores, ds_int_array *max_indeces);
static void cleanup_string(void *element);
static int score(ds_array *dir_names, char *to_auto, ds_int_array *scores);

int get_autocomplete_filepath(ds_string cmd, char **filepath,
                              size_t *to_delete) {

  int argc;
  char **argv;

  tokenize(&argc, &argv, cmd, ' ');

  char *to_auto = argv[argc - 1];
  *to_delete = strlen(to_auto);

  ds_array dir_names;
  if (ds_array_init(&dir_names, 8, sizeof(ds_string), cleanup_string, NULL) !=
      DS_STATUS_OK) {
    return -1;
  }

  get_dir_names(&dir_names);

  ds_int_array scores;
  if (ds_int_array_init(&scores, 8) != DS_STATUS_OK) {
    ds_array_deinit(&dir_names);
    return -1;
  }

  if (score(&dir_names, to_auto, &scores) != 0) {
    return -1;
  }

  // just pick the first filepath for now
  ds_int_array max_indeces;
  if (ds_int_array_init(&max_indeces, 8) != DS_STATUS_OK) {
    return -1;
  }

  if (get_max_indeces(scores, &max_indeces) != 0) {
    return -1;
  }

  *filepath = NULL;
  int score = ds_int_array_init(&max_indeces, 0);
  *filepath = names[max_indeces[0]];

  return 0;
}

static int get_dir_names(ds_array *dir_names) {

  DIR *current_dir;

  if ((current_dir = opendir(".")) == NULL) {
    perror("Could not read current directory for autocomple");
    return -1;
  }

  struct dirent *entry;

  while ((entry = readdir(current_dir)) != NULL) {
    ds_string name;
    ds_string_init(&name, entry->d_name);
    ds_array_push(dir_names, &name);
  }

  closedir(current_dir);

  return 0;
}

static int get_max_indeces(ds_int_array scores, ds_int_array *max_indeces) {
  int max_score = 0;

  for (size_t i = 0; i < scores.length; i++) {
    int score = ds_int_array_get(&scores, i);
    if (score > max_score)
      max_score = score;
  }

  for (size_t i = 0; i < scores.length; i++) {
    int score = ds_int_array_get(&scores, i);
    if (max_score == score) {
      ds_int_array_push(max_indeces, i);
    }
  }

  return 0;
}

static int score(ds_array *dir_names, char *to_auto, ds_int_array *scores) {

  for (size_t i = 0; i < dir_names->length; i++) {
    int num = 0;
    ds_string *dir_name = ds_array_get(dir_names, i);
    for (size_t j = 0; to_auto[j] && dir_name->data[j]; j++) {

      if (to_auto[j] != dir_name->data[j])
        break;

      num++;
    }
    if (ds_int_array_push(scores, num) != DS_STATUS_OK) {
      return -1;
    }
  }

  return 0;
}

static void cleanup_string(void *element) { ds_string_deinit(element); }
