#pragma once

ssize_t get_cmd_cannonical(char** cmd);
ssize_t get_cmd_noncannonical(char** cmd);
void reset(int* argc, char** cmd, char*** argv);

