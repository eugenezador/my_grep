#include "ex_grep.h"

int main(int argc, char** argv) {
    grep_flags flags = {0, 0, 0, 0, 0, 0};
    grep_data data = {{""}, {""}};
    
    parse_input(argc, argv, &flags, &data);

    return 0;
}



void parse_input(int argc, char**argv, grep_flags* flags, grep_data* data) {
 
    int pattern_counter = 0;
    int e_flag_counter = 0;
    
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-' && argv[i + 1][0] != '-' && strchr(argv[i], 'e') && i != argc - 1) {
            strcpy(data->patterns[pattern_counter], argv[i + 1]);
            pattern_counter++;
            e_flag_counter++;
            flags->e = true;
        } 
    }
    
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-' && !strchr(argv[i],'e')) {
            parse_flags(&flags, argv[i]);
        }
        if(argv[i][0] == '-' && pattern_counter == 0 && !flags->e) {
            strcpy(data->patterns[pattern_counter], argv[i]);
        }
        if(pattern_counter > 1 ) {

            // parse files;
        }
    }
    
    for(int i = 1; i != argc; i++) {
        if(argv[i][0] != '-'){
            print_file(argv[i], &flags);
        }
        
    }
}

void parse_flags(grep_flags* flags, char* flag_array) {
    flag_array++;// пропускаем первое тире
        for(; *flag_array != '\0'; flag_array++) {
            switch(*flag_array) {
            case 'e':
                flags->e = true;
                break;
            case 'i':
                flags->i= true;
                break;
            case 'v':
                flags->v= true;
                break;
            case 'c':
                flags->c = true;
                break;
            case 'l':
                flags->l = true;
                break;
            default:
                printf("Invalid option!");
        }
        }
    }
}


