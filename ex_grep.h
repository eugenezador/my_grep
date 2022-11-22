#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

 typedef struct{
     char file_name[100];
 }file_s;

 typedef struct {
     char pattern_name[100];
     char file_as_pattern[100];
    //  char file_name[100];
 }pattern_s;

typedef struct {
  bool e_template;
  bool i_ignore_registr;
  bool v_invert_search;
  bool c_show_only_match_count;
  bool l_show_match_files;
  bool n_show_number_of_row;
  bool h_dont_show_file_name;
  bool s_hide_file_error_message;
  bool f_file_is_a_pattern;
  bool o_pattern_inside_line;
} grep_flags;

void init_patterns(pattern_s* patterns,int size);

void init_files(file_s* files, int size);

void parse_input(int argc, char **argv, grep_flags* flags, file_s* files, pattern_s* patterns, int* pattern_counter, int* files_counter);

void parse_e_flag_patterns(int argc, char **argv, grep_flags* flags, pattern_s* patterns, int* pattern_counter);

void parse_f_flag_patterns(int argc, char **argv, grep_flags* flags, pattern_s* patterns, int* pattern_counter);

void parse_input_ignore_e_flag(int argc, char **argv, grep_flags* flags, file_s* files, pattern_s* patterns, int* pattern_counter, int* files_counter);

void parse_flags(grep_flags* flags, char* flag_array);

bool file_already_stored(file_s* files, const int* files_counter,  char* new_file);

bool pattern_already_stored(pattern_s* patterns, const int* pattern_counter,  char* new_pattern);

void grep_parsed_data(grep_flags flags, file_s* files, pattern_s* patterns, int pattern_counter, int files_counter);

int search_in_file(FILE *file, pattern_s* patterns, int patterns_counter, grep_flags flags, char* file_name, int files_counter);

void change_end_line_symbol_to_null(char* line_buf);

void search_selector(char* line_buf, pattern_s pattern, grep_flags flags, int* match_lines_counter, int line_number, char* file_name, int files_counter);

void search_file_as_a_pattern();

void search_pattern_in_line(char* line_buf, pattern_s pattern, grep_flags flags, int* match_lines_counter, int line_number, char* file_name, int files_counter);

int get_regex_compile_status(regex_t* regex, grep_flags flags, char* pattern);

void print_found_matches(char* line_buf, grep_flags flags, int line_number, char* file_name, int files_counter);

void print_mach_lines_counter(grep_flags flags, char* file_name, int files_counter, int match_lines_counter);

void print_files_with_match(char* file_name, int match_lines_counter);

#endif  // SRC_GREP_S21_GREP_H_
