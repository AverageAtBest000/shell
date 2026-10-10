#include "ds_common.h"
#include "ds_string.h"
#include <string.h>
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

  while ((ch = cmd->data[i++]) != '\0') {

    if (ch == '"') {
      // "" should be read as an empty argument
      token_started = true;
      in_string = !in_string;
      continue;
    }

    if (ch == delim && !in_string) {
      if (token_started) {
        if (append_to_vector(argv, argc, token.data) != 0) {
          ds_string_deinit(&token);
          return -1;
        }
      }

      token_started = false;
      ds_string_clear(&token);
      continue;
    }

    if (ds_string_append_char(&token, ch) != DS_STATUS_OK) {
      ds_string_deinit(&token);
      return -1;
    }
    token_started = true;
  }

  if (in_string || (token_started &&
                    append_to_vector(argv, argc, token.data) != 0)) {
    ds_string_deinit(&token);
    return -1;
  }

  ds_string_deinit(&token);

  return 0;
}

static int append_to_vector(char ***argv, int *argc, char *token) {
  char **temp = realloc(*argv, (*argc + 2) * sizeof(char *));
  if (temp == NULL)
    return -1;

  *argv = temp;
  (*argv)[*argc] = strdup(token);
  if ((*argv)[*argc] == NULL)
    return -1;
  (*argc)++;
  (*argv)[*argc] = NULL;

  return 0;
}
