#include "ds_string.h"
#include "input/completion.h"
#include "input/input.h"
#include "input/handlers.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int handle_backspace(ds_string *cmd) {
  if ((*cmd).length > 0) {
    if (delete_from_userin(1) == -1) {
      return -1;
    }
    cmd->data[--cmd->length] = '\0';
  }
  return 0;
}

int handle_tab(ds_string *cmd) {

  char *filepath;
  size_t to_delete;

  if (get_autocomplete_filepath(cmd, &filepath, &to_delete) != 0) {
    return -1;
  }

  delete_from_userin(to_delete);

  cmd->length -= to_delete;
  cmd->data[cmd->length] = '\0';
  if (ds_string_append_c_str(cmd, filepath) != DS_STATUS_OK) {
    free(filepath);
    return -1;
  }

  size_t to_write = strlen(filepath);
  write(STDOUT_FILENO, filepath, to_write);
  free(filepath);

  return 0;
}
