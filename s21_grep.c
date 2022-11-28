#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    file_s files[argc];
    pattern_s patterns[argc];
    grep_flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    init_patterns(patterns, argc);
    init_files(files, argc);

    int pattern_counter = 0;
    int files_counter = 0;

    parse_input(argc, argv, &flags, files, patterns, &pattern_counter, &files_counter);
    
    char all_templates[BUFSIZ] = {0};

    get_all_templates(patterns, pattern_counter, all_templates);

    // printf("all t = %s\n", all_templates);

    grep_parsed_data(flags, files, patterns, pattern_counter, files_counter, all_templates);

  } else {
    printf("Bad try to use grep!");
  }
  return 0;
}

void init_patterns(pattern_s *patterns, int size) {
  for (int i = 0; i < size; i++) {
    strcpy(patterns[i].pattern_name, "");
    strcpy(patterns[i].file_as_pattern, "");
    patterns[i].is_pattren_a_file = false;
  }
}
void init_files(file_s *files, int size) {
  for (int i = 0; i < size; i++) {
    strcpy(files[i].file_name, "");
  }
}

void parse_input(int argc, char **argv, grep_flags *flags, file_s *files,
                 pattern_s *patterns, int *pattern_counter,
                 int *files_counter) {

  // get e flag patterns
  parse_e_flag_patterns(argc, argv, flags, patterns, pattern_counter);

  // get f flag patterns
  parse_f_flag_patterns(argc, argv, flags, patterns, pattern_counter);

  // get flags (except e f) patterns files
  parse_input_ignore_e_f_flags(argc, argv, flags, files, patterns,
                               pattern_counter, files_counter);

    // for(int i = 0; i < *pattern_counter; i++) {
    //     if(!patterns[i].is_pattren_a_file) printf("pat = %s\n", patterns[i].pattern_name);
    //     else printf("f pat = %s\n", patterns[i].file_as_pattern);
    // }

    // for(int i = 0; i < *files_counter; i++) {
    //     printf("file = %s\n", files[i].file_name);
    // }
}

void parse_e_flag_patterns(int argc, char **argv, grep_flags *flags,
                           pattern_s *patterns, int *pattern_counter) {

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strchr(argv[i], 'e') && i != argc - 1) {

      if (strchr(argv[i], 'e') == argv[i] + strlen(argv[i]) - 1) {
        if (!pattern_already_stored(patterns, pattern_counter, argv[i + 1])) {
          strcpy(patterns[*pattern_counter].pattern_name, argv[i + 1]);
          patterns[*pattern_counter].is_pattren_a_file = false;
          (*pattern_counter)++;
          flags->e_template = true;
        }
      } else {
        if (!pattern_already_stored(patterns, pattern_counter,
                                    strchr(argv[i], 'e') + 1)) {
          strcpy(patterns[*pattern_counter].pattern_name,
                 strchr(argv[i], 'e') + 1);
          patterns[*pattern_counter].is_pattren_a_file = false;
          (*pattern_counter)++;
          flags->e_template = true;
        }
      }
    }
  }
}

void parse_f_flag_patterns(int argc, char **argv, grep_flags *flags,
                           pattern_s *patterns, int *pattern_counter) {

  for (int i = 1; i < argc; i++) {

    if (argv[i][0] == '-' && strchr(argv[i], 'f') && i != argc - 1) {

      if (strchr(argv[i], 'f') == argv[i] + strlen(argv[i]) - 1) {
        if (!file_as_pattern_already_stored(patterns, pattern_counter,
                                            argv[i + 1])) {
          strcpy(patterns[*pattern_counter].file_as_pattern, argv[i + 1]);
          patterns[*pattern_counter].is_pattren_a_file = true;
          (*pattern_counter)++;
          flags->f_file_is_a_pattern = true;
        }
      } else {
        if (!file_as_pattern_already_stored(patterns, pattern_counter,
                                            strchr(argv[i], 'f') + 1)) {
          strcpy(patterns[*pattern_counter].file_as_pattern,
                 strchr(argv[i], 'f') + 1);
          patterns[*pattern_counter].is_pattren_a_file = true;
          (*pattern_counter)++;
          flags->f_file_is_a_pattern = true;
        }
      }
    }
  }
}

void parse_input_ignore_e_f_flags(int argc, char **argv, grep_flags *flags,
                                  file_s *files, pattern_s *patterns,
                                  int *pattern_counter, int *files_counter) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      parse_flags(flags, argv[i]);
    }

    if (argv[i][0] != '-' && *pattern_counter == 0 && !flags->e_template &&
        !flags->f_file_is_a_pattern &&
        !pattern_already_stored(patterns, pattern_counter, argv[i])) {

      strcpy(patterns[*pattern_counter].pattern_name, argv[i]);
      (*pattern_counter)++;
      continue;
    }

    if (argv[i][0] != '-' && *pattern_counter > 0 &&
        !file_already_stored(files, files_counter, argv[i]) &&
        !((argv[i][0] == '-' && strchr(argv[i], 'f')) ||
          (i != 1 && argv[i - 1][0] == '-' &&
           argv[i - 1][strlen(argv[i - 1]) - 1] == 'f') ||
          (argv[i][0] == '-' && strchr(argv[i], 'e')) ||
          (i != 1 && argv[i - 1][0] == '-' &&
           argv[i - 1][strlen(argv[i - 1]) - 1] == 'e'))) {

      strcpy(files[*files_counter].file_name, argv[i]);
      (*files_counter)++;
    }
  }
}

void parse_flags(grep_flags *flags, char *flag_array) {
  flag_array++; // пропускаем тире
  for (; *flag_array != '\0'; flag_array++) {
    switch (*flag_array) {
    case 'e':
      flags->e_template = true; break;
    case 'i':
      flags->i_ignore_registr = true; break;
    case 'v':
      flags->v_invert_search = true; break;
    case 'c':
      flags->c_show_only_match_count = true; break;
    case 'l':
      flags->l_show_match_files = true; break;
    case 'n':
      flags->n_show_number_of_row = true; break;
    case 'h':
      flags->h_dont_show_file_name = true; break;
    case 's':
      flags->s_hide_file_error_message = true; break;
    case 'o':
      flags->o_pattern_inside_line = true; break;
    }
  }
}

bool file_already_stored(file_s *files, const int *files_counter,
                         char *new_file) {
  bool result = false;
  for (int i = 0; i < *files_counter; i++) {
    if (strcmp(files[i].file_name, new_file) == 0) {
      result = true;
    }
  }
  return result;
}

bool pattern_already_stored(pattern_s *patterns, const int *pattern_counter,
                            char *new_pattern) {
  bool result = false;
  for (int i = 0; i < *pattern_counter; i++) {
    if (strcmp(patterns[i].pattern_name, new_pattern) == 0) {
      result = true;
    }
  }
  return result;
}

bool file_as_pattern_already_stored(pattern_s *patterns,
                                    const int *pattern_counter,
                                    char *new_pattern) {
  bool result = false;
  for (int i = 0; i < *pattern_counter; i++) {
    if (strcmp(patterns[i].file_as_pattern, new_pattern) == 0) {
      result = true;
    }
  }
  return result;
}

void get_all_templates(pattern_s *patterns, int pattern_counter, char* all_templates) {
  strcat(all_templates, "|");
  for(int i = 0; i < pattern_counter; i++) {
    if(patterns[i].is_pattren_a_file) {
      patterns_from_file_to_array(patterns[i], all_templates);
    } else {
      strcat(all_templates, patterns[i].pattern_name);
      strcat(all_templates, "|");
    }
  }
}

void patterns_from_file_to_array(pattern_s pattern, char* all_templates) {
  FILE *file = fopen(pattern.file_as_pattern, "r");
  if (file == NULL) {
    // if (!flags.s_hide_file_error_message) {
      printf("grep: %s: No such file or directory\n", pattern.file_as_pattern);
    // }
  } else {
    size_t line_buf_size = 0;
    char *pattern_line_buf = NULL;
    while (getline(&pattern_line_buf, &line_buf_size, file) != -1) {
      if (pattern_line_buf) {
        change_end_line_symbol_to_null(pattern_line_buf);
        if(strlen(pattern_line_buf) != 0) {
          strcat(all_templates, pattern_line_buf);
          strcat(all_templates, "|");
        }
      }
    }
    free(pattern_line_buf);
    fclose(file);
    }
}

void grep_parsed_data(grep_flags flags, file_s *files, pattern_s *patterns,
                      int pattern_counter, int files_counter, char* all_templates) {
    
    // bool empty_line = is_empty_line_in_patterns(patterns, pattern_counter);

  int match_lines_counter = 0;
    int file_error_counter = 0;

  for (int i = 0; i < files_counter; i++) {
    FILE *file = fopen(files[i].file_name, "r");
    if (file == NULL) {
      if (!flags.s_hide_file_error_message) {
        printf("grep: %s: No such file or directory\n", files[i].file_name);
          file_error_counter++;
      }
    } else {

            match_lines_counter = search_in_file(file, patterns, pattern_counter, flags, files[i].file_name, files_counter, all_templates);

      fclose(file);
    }

    if (flags.c_show_only_match_count && file_error_counter == 0) {
      print_mach_lines_counter(flags, files[i].file_name, files_counter,
                               match_lines_counter);
    }

    if (flags.l_show_match_files && file_error_counter == 0) {
      print_files_with_match(files[i].file_name, match_lines_counter);
    }
  }
}

int search_in_file(FILE *file, pattern_s *patterns, int pattern_counter,
                   grep_flags flags, char *file_name, int files_counter, char* all_templates) {

  int match_lines_counter = 0;
  int line_number = 0;

  bool print_pattern_info = true;
  bool one_time_counter = true;

  size_t line_buf_size = 0;
  char *line_buf = NULL;

  while (getline(&line_buf, &line_buf_size, file) != -1) {
    if (line_buf) {
      one_time_counter = true;
      change_end_line_symbol_to_null(line_buf);
      line_number++;

      one_time_search(line_buf, flags, &match_lines_counter,
                             line_number, file_name, files_counter,
                             &one_time_counter, all_templates);

      // for (int i = 0; i < pattern_counter; i++) {

      //   print_pattern_info = true;
      //  // one_time_counter = true;

      //   search_selector(line_buf, patterns[i], flags, &match_lines_counter,
      //                   line_number, file_name, files_counter,
      //                   &print_pattern_info, &one_time_counter);
      // }
    }
  }
  free(line_buf);
  return match_lines_counter;
}

void one_time_search(char *line_buf, grep_flags flags,
                            int *match_lines_counter, int line_number,
                            char *file_name, int files_counter,
                            bool *one_time_counter, char* all_templates) {
  regex_t regex;
  int compile_status = get_regex_compile_status(&regex, flags, all_templates);

  if (compile_status) {
    printf("Regex compile error\n");
  } else {
    int search_status = regexec(&regex, line_buf, 0, NULL, 0);
    if (!flags.v_invert_search && search_status == 0) {
      print_found_matches(line_buf, flags, line_number, file_name,
                          files_counter, match_lines_counter, one_time_counter);
    } else if (flags.v_invert_search && search_status == REG_NOMATCH) {
        print_found_matches(line_buf, flags, line_number, file_name,
                            files_counter, match_lines_counter,
                            one_time_counter);
    }
   *one_time_counter = false;
  }
  regfree(&regex);
}


void change_end_line_symbol_to_null(char *line_buf) {
  if (line_buf[0] == '\n') {
    line_buf[0] = '\0';
  }
  if (line_buf[strlen(line_buf) - 1] == '\n') {
    line_buf[strlen(line_buf) - 1] = '\0';
  }
}

void search_selector(char *line_buf, pattern_s pattern, grep_flags flags,
                     int *match_lines_counter, int line_number, char *file_name,
                     int files_counter, bool *print_pattern_info,
                     bool *one_time_counter) {

  if (flags.o_pattern_inside_line && !flags.c_show_only_match_count && !flags.v_invert_search) {
    if (flags.f_file_is_a_pattern && pattern.is_pattren_a_file) {
      search_file_as_a_pattern(line_buf, pattern, flags, match_lines_counter,
                               line_number, file_name, files_counter,
                               print_pattern_info, one_time_counter);
    } else {
      search_o_pattern_in_line(line_buf, pattern.pattern_name, flags,
                               match_lines_counter, line_number, file_name,
                               files_counter, print_pattern_info);
    }
  } else {
    if (flags.f_file_is_a_pattern && pattern.is_pattren_a_file) {
      search_file_as_a_pattern(line_buf, pattern, flags, match_lines_counter,
                               line_number, file_name, files_counter,
                               print_pattern_info, one_time_counter);
    } else {
      search_pattern_in_line(line_buf, pattern, flags, match_lines_counter,
                             line_number, file_name, files_counter,
                             one_time_counter);
    }
  }
}

void search_o_pattern_in_line(char *line_buf, char *pattern_name,
                              grep_flags flags, int *match_lines_counter,
                              int line_number, char *file_name,
                              int files_counter, bool *print_pattern_info) {

  regex_t regex;
  int compile_status = get_regex_compile_status(&regex, flags, pattern_name);

  if (compile_status) {
    printf("Regex compile error\n");
  } else {
    regmatch_t match;

    size_t line_search_position = 0;
    size_t line_search_length = strlen(line_buf);

    while (regexec(&regex, line_buf + line_search_position, 1, &match, 0) ==
           0) {

//      if (*print_pattern_info) {
        (*match_lines_counter)++;
//      }

      print_o_pattern_match(line_buf, flags, line_number, file_name,
                            files_counter, match, line_search_position,
                            print_pattern_info);

      line_search_position += match.rm_eo;

      if (match.rm_so == match.rm_eo)
        line_search_position += 1;
      if (line_search_position > line_search_length)
        break;
    }
  }
  regfree(&regex);
}

void print_o_pattern_match(char *line_buf, grep_flags flags, int line_number,
                           char *file_name, int files_counter, regmatch_t match,
                           size_t line_search_position,
                           bool *print_pattern_info) {

  if (!flags.c_show_only_match_count && !flags.l_show_match_files) {
    if (*print_pattern_info) {
      if (!flags.h_dont_show_file_name && files_counter > 1) {
        printf("%s:", file_name);
      }
      if (flags.n_show_number_of_row) {
        printf("%d:", line_number);
      }
    }
    for (size_t i = line_search_position + match.rm_so;
         i < line_search_position + match.rm_eo; i++)
      printf("%c", line_buf[i]);
    printf("\n");
    *print_pattern_info = false;
  }
}

void search_file_as_a_pattern(char *line_buf, pattern_s pattern,
                              grep_flags flags, int *match_lines_counter,
                              int line_number, char *file_name,
                              int files_counter, bool *print_pattern_info,
                              bool *one_time_counter) {

  FILE *pattern_is_file = fopen(pattern.file_as_pattern, "r");
  if (pattern_is_file == NULL) {
    if (!flags.s_hide_file_error_message) {
      if (line_number == 1) {
        printf("grep: %s: No such file or directory\n",
               pattern.file_as_pattern);
      }
    }
  } else {
    size_t line_buf_size = 0;
    char *pattern_line_buf = NULL;
    while (getline(&pattern_line_buf, &line_buf_size, pattern_is_file) != -1) {
      if (pattern_line_buf) {
        change_end_line_symbol_to_null(pattern_line_buf);
        if (flags.o_pattern_inside_line) {
          search_o_pattern_in_line(line_buf, pattern_line_buf, flags,
                                   match_lines_counter, line_number, file_name,
                                   files_counter, print_pattern_info);
        } else {
          search_file_pattern_in_line(
              line_buf, pattern_line_buf, flags, match_lines_counter,
              line_number, file_name, files_counter, one_time_counter);
        }
      }
    }
    free(pattern_line_buf);
    fclose(pattern_is_file);
  }
}

void search_file_pattern_in_line(char *line_buf, char *pattern_line_buf,
                                 grep_flags flags, int *match_lines_counter,
                                 int line_number, char *file_name,
                                 int files_counter, bool *one_time_counter) {

  regex_t regex;
  int compile_status = get_regex_compile_status(&regex, flags, pattern_line_buf);

  if (compile_status) {
    printf("Regex compile error\n");
  } else {
    int search_status = regexec(&regex, line_buf, 0, NULL, 0);
    if (!flags.v_invert_search && search_status == 0) {
      print_found_matches(line_buf, flags, line_number, file_name,
                          files_counter, match_lines_counter, one_time_counter);
    } else if (flags.v_invert_search && search_status == REG_NOMATCH) {
        print_found_matches(line_buf, flags, line_number, file_name,
                            files_counter, match_lines_counter,
                            one_time_counter);
    }
//    *one_time_counter = false;
  }
  regfree(&regex);
}

void search_pattern_in_line(char *line_buf, pattern_s pattern, grep_flags flags,
                            int *match_lines_counter, int line_number,
                            char *file_name, int files_counter,
                            bool *one_time_counter) {

  regex_t regex;
  int compile_status = get_regex_compile_status(&regex, flags, pattern.pattern_name);

  if (compile_status) {
    printf("Regex compile error\n");
  } else {
    int search_status = regexec(&regex, line_buf, 0, NULL, 0);
    if (!flags.v_invert_search && search_status == 0) {
      print_found_matches(line_buf, flags, line_number, file_name,
                          files_counter, match_lines_counter, one_time_counter);
    } else if (flags.v_invert_search && search_status == REG_NOMATCH) {
        print_found_matches(line_buf, flags, line_number, file_name,
                            files_counter, match_lines_counter,
                            one_time_counter);
    }
   *one_time_counter = false;
  }
  regfree(&regex);
}

int get_regex_compile_status(regex_t *regex, grep_flags flags, char *pattern) {

  int compile_status = 1;
  if (flags.i_ignore_registr) {
    compile_status = regcomp(regex, pattern, REG_ICASE | REG_EXTENDED);
  } else {
    compile_status = regcomp(regex, pattern, REG_EXTENDED);
  }
  return compile_status;
}

void print_found_matches(char *line_buf, grep_flags flags, int line_number,
                         char *file_name, int files_counter,
                         int *match_lines_counter, bool *one_time_counter) {
  if (!flags.c_show_only_match_count && !flags.l_show_match_files &&
      *one_time_counter) {
    if (flags.n_show_number_of_row) {
      if (flags.h_dont_show_file_name == false && files_counter > 1) {
        printf("%s:", file_name);
      }
      printf("%d:", line_number);
    }
    printf("%s\n", line_buf);
  }
  if (*one_time_counter)
    (*match_lines_counter)++;
}

void print_mach_lines_counter(grep_flags flags, char *file_name,
                              int files_counter, int match_lines_counter) {
  if (flags.h_dont_show_file_name == false && files_counter > 1) {
    printf("%s:", file_name);
  }

  if (!flags.l_show_match_files) {
    printf("%d\n", match_lines_counter);
  } else {
    if (match_lines_counter) {
      printf("1\n");
    } else {
      printf("0\n");
    }
  }
}

void print_files_with_match(char *file_name, int match_lines_counter) {
  if (match_lines_counter)
    printf("%s\n", file_name);
}
