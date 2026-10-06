#include "ds_common.h"
#include "ds_string.h"
#include <stdatomic.h>
#include <stdbool.h>

int get_tokens(int *argc, char ***argv, ds_string *cmd, char delim);
static int append_to_vector(char ***argv, int *argc, char *token);

int tokenize(int *argc, char ***argv, ds_string *cmd, char delim) {
  if (get_tokens(argc, argv, cmd, delim) != 0) {
    return -1;
  }

  return 0;
}

/*
 * @brief Splits a ds_string based on a delimeter
 *
 * @param argc Counter for the number of tokens
 * @param argv Argument vector containing pointers to the tokens
 * @param cmd Command to tokenize
 * @param delim The delimeter to split by
 *
 * @return 0 on succes
 */
int get_tokens(int *argc, char ***argv, ds_string *cmd, char delim) {

  int i = 0;
  char ch;
  bool in_string = false;
  bool token_started = false;
  ds_string token;
  if (ds_string_init(&token, "") != DS_STATUS_OK) {
    return -1;
  }

  ch = cmd->data[i++];
  while (ch != '\0') {

    ch = cmd->data[i++];

    if (ch == '"' || ch == '\'') {
      in_string = !in_string;
      i++;
      continue;
    }

    if (ch == delim && !in_string) {
      if (token_started) {
        append_to_vector(argv, argc, token.data);
      }

      token_started = false;
      ds_string_clear(&token);
      i++;
      continue;
    }

    if (ds_string_append_char(&token, ch) != DS_STATUS_OK) {
      return -1;
    }
    token_started = true;
  }

  ds_string_deinit(&token);

  return 0;
}

static int append_to_vector(char ***argv, int *argc, char *token) {
  char **temp = realloc(*argv, (*argc + 2) * sizeof(char *));
  if (temp == NULL)
    return -1;

  *argv = temp;
  (*argv)[*argc] = token;
  (*argc)++;
  (*argv)[*argc] = NULL;

  return 0;
}
