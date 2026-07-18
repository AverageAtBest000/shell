#ifndef BUILTINS_H
#define BUILTINS_H

bool handleBuiltins(char** argv, int argc);
void cd(char** argv, int argc);
bool is_builtin(char* cmd);

#endif