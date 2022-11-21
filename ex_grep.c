#include "ex_grep.h"

int main(int argc, char **argv) {
  if (argc > 2){
      file_s files[argc];
      pattern_s patterns[argc];
      grep_flags flags = {0, 0, 0, 0, 0, 0, 0, 0};
      
      init_patterns(patterns, argc);
      init_files(files, argc);
      
      int pattern_counter = 0;
      int files_counter = 0;
      parse_input(argc, argv, &flags, files, patterns, &pattern_counter, &files_counter);
      
      grep_parsed_data(flags, files, patterns, pattern_counter, files_counter);
      
    }
  else {
    printf("Bad try to use grep!");
  }
  return 0;
}

void init_patterns(pattern_s* patterns,int size) {
    for (int i = 0; i < size; i++) {
        strcpy(patterns[i].pattern_name, "");
    }
}
void init_files(file_s* files, int size) {
    for (int i = 0; i < size; i++) {
        strcpy(files[i].file_name, "");
    }
}


void parse_input(int argc, char **argv, grep_flags* flags, file_s* files, pattern_s* patterns, int* pattern_counter, int* files_counter) {
    
    
    // get e flags and patterns and files
    parse_e_flag_patterns_files(argc, argv, flags, files, patterns, pattern_counter, files_counter);
    
    // get flags (except e) patterns files
    parse_input_ignore_e_flag(argc, argv, flags, files, patterns, pattern_counter, files_counter);
    
    //Debug
//    for(int i = 0; i < *pattern_counter; i++) {
//        printf("pat %s \n", patterns[i].pattern_name);
//    }
//    for(int i = 0; i < *files_counter; i++) {
//        printf("file %s \n", files[i].file_name);
//    }

}

void parse_e_flag_patterns_files(int argc, char **argv, grep_flags* flags, file_s* files, pattern_s* patterns, int* pattern_counter, int* files_counter) {
    
    for(int i = 1; i < argc; i++) {
        // is e pattern
        if(argv[i][0] == '-' && argv[i + 1][0] != '-' && strchr(argv[i], 'e') && i != argc - 1 && !pattern_already_stored(patterns, pattern_counter, argv[i + 1])) {
            
    //         char *res = NULL;
    // if (strchr(argv[i], 'e') == argv[i] + strlen(argv[i]) - 1) {
    //     if (i + 1 < argc) res = argv[i + 1]; // слитно
    //     printf("0 debug: %s\n", res);
    // } else {
    //     res = strchr(argv[i], 'e') + 1;
    //     printf("1 debug: %s\n", res); // раздельно
    // }
    // strcpy(patterns[*pattern_counter].pattern_name, res);
            
            if(strchr(argv[i], 'e') == argv[strlen(argv[i] - 1)]) 
            {
                strcpy(patterns[*pattern_counter].pattern_name, argv[i + 1]);
                printf("0deb %s\n", patterns[*pattern_counter].pattern_name);
            } else {
                strcpy(patterns[*pattern_counter].pattern_name, strchr(argv[i], 'e') + 1);
                printf("1deb %s\n", patterns[*pattern_counter].pattern_name);
            }
            (*pattern_counter)++;
            flags->e_template = true;
            continue;
        }

        // is file
        if(argv[i][0] != '-' && strspn(argv[i - 1], "-e") != 2 && *pattern_counter != 0 && !file_already_stored(files, files_counter, argv[i])) {
            strcpy(files[*files_counter].file_name, argv[i]);
            (*files_counter)++;
        }
    }
}

void parse_input_ignore_e_flag(int argc, char **argv, grep_flags* flags, file_s* files, pattern_s* patterns, int* pattern_counter, int* files_counter) {
    for (int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            parse_flags(flags, argv[i]);
        }
        if(argv[i][0] != '-' && *pattern_counter == 0 && !flags->e_template && !pattern_already_stored(patterns, pattern_counter, argv[i])) {
            strcpy(patterns[*pattern_counter].pattern_name, argv[i]);
            (*pattern_counter)++;
            continue;
        }
        if(*pattern_counter != 0 && argv[i][0] != '-' && !flags->e_template && !file_already_stored(files, files_counter, argv[i])) {
            strcpy(files[*files_counter].file_name, argv[i]);
            (*files_counter)++;
        }
    }
}

void parse_flags(grep_flags *flags, char *flag_array) {
  flag_array++; // пропускаем первое тире
  for (; *flag_array != '\0'; flag_array++) {
    switch (*flag_array) {
        case 'e':
          flags->e_template= true;
          break;
        case 'i':
      flags->i_ignore_registr = true;
      break;
    case 'v':
      flags->v_invert_search = true;
      break;
    case 'c':
      flags->c_show_only_match_count = true;
      break;
    case 'l':
      flags->l_show_match_files = true;
      break;
    case 'n':
      flags->n_show_number_of_row = true;
      break;
    case 'h':
      flags->h_dont_show_file_name = true;
      break;
    case 's':
      flags->s_hide_file_error_message = true;
      break;
    // default:
    //   printf("Invalid option!");
    }
  }
}


bool file_already_stored(file_s* files, const int* files_counter, char* new_file) {
    bool result = false;
    for(int i = 0; i < *files_counter; i++) {
        if(strcmp(files[i].file_name, new_file) == 0) {
            result = true;
        }
    }
    return result;
}

bool pattern_already_stored(pattern_s* patterns, const int* pattern_counter,  char* new_pattern) {
    bool result = false;
    for(int i = 0; i < *pattern_counter; i++) {
        if(strcmp(patterns[i].pattern_name, new_pattern) == 0) {
            result = true;
        }
    }
    return result;
}

void grep_parsed_data(grep_flags flags,file_s* files, pattern_s* patterns, int pattern_counter, int files_counter) {
    
    int match_lines_counter = 0;
    
    for(int i = 0; i < files_counter; i++) {
        FILE *file = fopen(files[i].file_name, "r");
        if(file == NULL) {
            if(!flags.s_hide_file_error_message) printf("grep: %s: No such file or directory", files[i].file_name);
        } else {
            match_lines_counter = search_in_file(file, patterns, pattern_counter, flags, files[i].file_name, files_counter);
            fclose(file);
        }
        
        if(flags.c_show_only_match_count) {
            print_mach_lines_counter(flags, files[i].file_name, files_counter, match_lines_counter);
        }
        
        if(flags.l_show_match_files) {
            print_files_with_match(files[i].file_name, match_lines_counter);
        }
    }
    
}

int search_in_file(FILE *file, pattern_s* patterns, int patterns_counter, grep_flags flags, char* file_name, int files_counter) {
    
    int match_lines_counter = 0;
    int line_number = 0;
    
    size_t line_buf_size = 0;
    char* line_buf = NULL;
    while(getline(&line_buf, &line_buf_size, file) != -1) {
        if(line_buf) {
            line_number++;
            for(int i = 0; i < patterns_counter; i++) {
                search_selector(line_buf, patterns[i], flags, &match_lines_counter, line_number, file_name, files_counter);
            }
        }
    }
    free(line_buf);
    return match_lines_counter;
}

void change_end_line_symbol_to_null(char* line_buf) {
    if(line_buf[0] == '\n') {
        line_buf[0] = '\0';
    }
    if(line_buf[strlen(line_buf) - 1] == '\n') {
        line_buf[strlen(line_buf) - 1] = '\0';
    }
}

void search_selector(char* line_buf, pattern_s pattern, grep_flags flags, int* match_lines_counter, int line_number, char* file_name, int files_counter) {
    
    search_pattern_in_line(line_buf, pattern, flags, match_lines_counter, line_number, file_name, files_counter);
    
}

void search_pattern_in_line(char* line_buf, pattern_s pattern, grep_flags flags, int* match_lines_counter, int line_number, char* file_name, int files_counter) {
    
    regex_t regex;
    int compile_status = get_regex_compile_status(&regex, flags, pattern.pattern_name);
    
    if(compile_status) {
        printf("Regex compile error\n");
    } else {
        int search_status = regexec(&regex, line_buf, 0, NULL, 0);
        if(!flags.v_invert_search && search_status == 0) {
            print_found_matches(line_buf, flags, line_number, file_name, files_counter);
            (*match_lines_counter)++;
        } else {
            // get invert search line
            if(flags.v_invert_search && search_status == REG_NOMATCH) {
                print_found_matches(line_buf, flags, line_number, file_name, files_counter);
            }
        }
    }
    regfree(&regex);
}

int get_regex_compile_status(regex_t* regex, grep_flags flags, char* pattern) {
    
    int compile_status = 1;
    if(flags.i_ignore_registr) {
        compile_status = regcomp(regex, pattern, REG_ICASE);
    } else {
        compile_status = regcomp(regex, pattern, REG_EXTENDED);
    }
    return compile_status;
}

void print_found_matches(char* line_buf, grep_flags flags, int line_number, char* file_name, int files_counter) {
    if(!flags.c_show_only_match_count && !flags.l_show_match_files) {
        if(flags.n_show_number_of_row) {
            if (!flags.h_dont_show_file_name && files_counter > 0) {
                printf("%s:", file_name);
            }
            printf("%d:", line_number);
        }
        printf("%s", line_buf);
    }
}

void print_mach_lines_counter(grep_flags flags, char* file_name, int files_counter, int match_lines_counter) {
    if(!flags.h_dont_show_file_name && files_counter > 0) {
        printf("%s:", file_name);
    }
    
    if(!flags.l_show_match_files) {
        printf("%d\n", match_lines_counter);
    } else {
        if(match_lines_counter) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

void print_files_with_match(char* file_name, int match_lines_counter) {
    if(match_lines_counter) printf("%s\n", file_name);
}

