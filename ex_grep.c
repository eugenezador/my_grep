#include "ex_grep.h"

int main(int argc, char **argv) {
  if (argc > 2){
      file_s files[argc];
      pattern_s patterns[argc];
      grep_flags flags = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      
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
        strcpy(patterns[i].file_as_pattern, "");
        patterns[i].is_pattren_a_file = false;
    }
}
void init_files(file_s* files, int size) {
    for (int i = 0; i < size; i++) {
        strcpy(files[i].file_name, "");
    }
}


void parse_input(int argc, char **argv, grep_flags* flags, file_s* files, pattern_s* patterns, int* pattern_counter, int* files_counter) {
    
    // get e flag patterns
    parse_e_flag_patterns(argc, argv, flags, patterns, pattern_counter);

    // get f flag patterns
    parse_f_flag_patterns(argc, argv, flags, patterns, pattern_counter);
    
    // get flags (except e f) patterns files
    parse_input_ignore_e_f_flags(argc, argv, flags, files, patterns, pattern_counter, files_counter);
    
    //Debug
//    for(int i = 0; i < *pattern_counter; i++) {
//        if(!patterns[i].is_pattren_a_file) printf("pat %s \n", patterns[i].pattern_name);
//    }

//    for(int i = 0; i < *pattern_counter; i++) {
//        if(patterns[i].is_pattren_a_file) printf("f pat %s \n", patterns[i].file_as_pattern);
//    }

//    for(int i = 0; i < *files_counter; i++) {
//        printf("file %s \n", files[i].file_name);
//    }

}

void parse_e_flag_patterns(int argc, char **argv, grep_flags* flags, pattern_s* patterns, int* pattern_counter) {
    
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-' && strchr(argv[i], 'e') && i != argc - 1 && !pattern_already_stored(patterns, pattern_counter, argv[i + 1])) {

            if(strchr(argv[i], 'e') == argv[i] + strlen(argv[i]) - 1)
            {
                strcpy(patterns[*pattern_counter].pattern_name, argv[i + 1]);
            } else {
                strcpy(patterns[*pattern_counter].pattern_name, strchr(argv[i], 'e') + 1);
            }
            patterns[*pattern_counter].is_pattren_a_file = false;
            (*pattern_counter)++;
            flags->e_template = true;
        }
    }
}

void parse_f_flag_patterns(int argc, char **argv, grep_flags* flags, pattern_s* patterns, int* pattern_counter) {
    
    for(int i = 1; i < argc; i++) {

        if(argv[i][0] == '-' && strchr(argv[i], 'f') && i != argc - 1 && 
           !pattern_already_stored(patterns, pattern_counter, argv[i + 1])) {

            if(strchr(argv[i], 'f') == argv[i] + strlen(argv[i]) - 1)
            {
                strcpy(patterns[*pattern_counter].file_as_pattern, argv[i + 1]);
            } else {
                strcpy(patterns[*pattern_counter].file_as_pattern, strchr(argv[i], 'f') + 1);
            }
            patterns[*pattern_counter].is_pattren_a_file = true;
            (*pattern_counter)++;
            flags->f_file_is_a_pattern = true;
        }
    }
}

void parse_input_ignore_e_f_flags(int argc, char **argv, grep_flags* flags, file_s* files, pattern_s* patterns, int* pattern_counter, int* files_counter) {
    for (int i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            parse_flags(flags, argv[i]);
        }
        
        if(argv[i][0] != '-' && *pattern_counter == 0 && 
          !flags->e_template && !flags->f_file_is_a_pattern &&
          !pattern_already_stored(patterns, pattern_counter, argv[i])) {
            
            strcpy(patterns[*pattern_counter].pattern_name, argv[i]);
            (*pattern_counter)++;
            continue;
        }

        if(argv[i][0] != '-' && *pattern_counter > 0 && 
           !((argv[i][0] == '-' && strchr(argv[i], 'f')) ||
             (i != 1 && argv[i - 1][0] == '-' && argv[i - 1][strlen(argv[i - 1]) - 1] == 'f' ) ||
             (argv[i][0] == '-' && strchr(argv[i], 'e')) ||
             (i != 1 && argv[i - 1][0] == '-' && argv[i - 1][strlen(argv[i - 1]) - 1] == 'e' ))) {
                
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
      flags->e_template= true; break;
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
            if(!flags.s_hide_file_error_message) printf("grep: %s: No such file or directory\n", files[i].file_name);
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
            change_end_line_symbol_to_null(line_buf);
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
    
    if(flags.f_file_is_a_pattern && pattern.is_pattren_a_file){
        search_file_as_a_pattern(line_buf, pattern, flags, match_lines_counter, line_number, file_name, files_counter);
    } else if (flags.o_pattern_inside_line){

    } else {
        search_pattern_in_line(line_buf, pattern, flags, match_lines_counter, line_number, file_name, files_counter);
    }
    
}

void search_file_as_a_pattern(char* line_buf, pattern_s pattern, grep_flags flags, int* match_lines_counter, int line_number, char* file_name, int files_counter) {
    
    // bool print_error_one_time = true;
    
    FILE *pattern_is_file = fopen(pattern.file_as_pattern, "r");
        if(pattern_is_file == NULL) {
            if(!flags.s_hide_file_error_message) {
                if(line_number == 1) {
                    printf("grep: %s: No such file or directory\n", pattern.file_as_pattern);
                    // print_error_one_time = false;
                }
            }
        } else {
            size_t line_buf_size = 0;
            char* pattern_line_buf = NULL;
            while(getline(&pattern_line_buf, &line_buf_size, pattern_is_file) != -1) {
                if(pattern_line_buf) {
                    change_end_line_symbol_to_null(pattern_line_buf);
                    strcpy(pattern.pattern_name, pattern_line_buf);
                    // search_file_pattern_in_line(line_buf, pattern_line_buf, flags, match_lines_counter, line_number, file_name, files_counter);
                    search_pattern_in_line(line_buf, pattern, flags, match_lines_counter, line_number, file_name, files_counter);
                }
            }
    free(pattern_line_buf);
    fclose(pattern_is_file);
    }
}

void search_file_pattern_in_line(char* line_buf, char* pattern_line_buf, grep_flags flags, int* match_lines_counter, int line_number, char* file_name, int files_counter) {
    regex_t regex;
    int compile_status = get_regex_compile_status(&regex, flags, pattern_line_buf);

    if(compile_status) {
        printf("Regex compile error\n");
    } else {
        int search_status = regexec(&regex, line_buf, 0, NULL, 0);
        printf("%d :  %s : %s\n", search_status, pattern_line_buf, line_buf);
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

void search_pattern_in_line(char* line_buf, pattern_s pattern, grep_flags flags, int* match_lines_counter, int line_number, char* file_name, int files_counter) {
    regex_t regex;
    int compile_status = get_regex_compile_status(&regex, flags, pattern.pattern_name);
    
    if(compile_status) {
        printf("Regex compile error\n");
    } else {
        int search_status = regexec(&regex, line_buf, 0, NULL, 0);
        // printf("%d :  %s : %s\n", search_status, pattern.pattern_name, line_buf);
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
        printf("%s\n", line_buf);
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

