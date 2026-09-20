#ifndef PARSER_H
#define PARSER_H

void tokenize(int* argc, char*** argv, char** cmd, char delim);
int get_tokens(int* argc, char*** argv, char** cmd, char delim);

#endif
