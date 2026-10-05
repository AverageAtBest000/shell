#pragma once
#include <sys/types.h>

int get_autocomplete_filepath(char *current_command, char **filepath,
                              size_t *to_delete);
