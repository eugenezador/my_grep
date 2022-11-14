#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char patterns[10][40];
    char files[10][40];
} grep_data;

typedef struct {
    bool e;
    bool i;
    bool v;
    bool c;
    bool l;
    bool n;
} grep_flags;

void parse_input(int argc, char** argv, grep_flags* flags, grep_data* data);
void parse_flags(grep_flags* flags, char* flag_array);

