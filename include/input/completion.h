#pragma once
#include "ds_string.h"
#include <sys/types.h>

struct ds_string;

int get_autocomplete_filepath(ds_string cmd, char **filepath,
                              size_t *to_delete);
