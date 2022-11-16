#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


// typedef struct{
//     char name[100];
// }file_s;

// typedef struct {
//     char name[100];
// }pattern_s;



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


// void count_parse_data(int argc, char** argv, int* file_counter, int* pattern_counter);
void parse_input(int argc, char** argv, grep_flags* flags, grep_data* data);
void parse_flags(grep_flags* flags, char* flag_array);



int main(int argc, char** argv) {

    // int file_counter = 0;
    // int pattern_counter = 0;

    // count_parse_data(argc, argv, &file_counter, &pattern_counter);

    grep_flags flags = {0, 0, 0, 0, 0, 0};
    
    // file_s input_files[file_counter];

    // pattern_s input_patterns[pattern_counter];

    grep_data data = {{""}, {""}};
    
    parse_input(argc, argv, &flags, &data);

    return 0;
}



void parse_input(int argc, char**argv, grep_flags* flags, grep_data* data) {
 
    int pattern_counter = 0;
    int e_flag_counter = 0;
    int files_counter = 0;
    
    for(int i = 1; i < argc; i++) {

        // is e pattern
        if(argv[i][0] == '-' && argv[i + 1][0] != '-' && strchr(argv[i], 'e') && i != argc - 1) {
            strcpy(data->patterns[pattern_counter], argv[i + 1]);
            pattern_counter++;
            e_flag_counter++;
            flags->e = true;
        }

        // is file
        if(argv[i][0] != '-' && strspn(argv[i - 1], "-e") < 2 && pattern_counter != 0) {
            strcpy(data->files[files_counter], argv[i]);
            files_counter++;
            printf("%d - %s\n", i, data->files[files_counter]);
        }
    }
    
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-' && !strchr(argv[i],'e')) {
            parse_flags(flags, argv[i]);
        }
        if(argv[i][0] != '-' && pattern_counter == 0 && !flags->e) {
            strcpy(data->patterns[pattern_counter], argv[i]);
            pattern_counter++;
            continue;
        }
        if(pattern_counter != 0 && argv[i][0] != '-' && !flags->e) {
            strcpy(data->files[files_counter], argv[i]);
            files_counter++;
            printf("%d\n", files_counter);
        }
    }

    for(int i = 0; i < pattern_counter; i++) {
        printf("%s : ", data->patterns[i]);
    }
    printf("\n");

    for(int i = 0; i < files_counter; i++){
        printf("%s : ", data->files[i]);
    }
    printf("\n");
}

void parse_flags(grep_flags* flags, char* flag_array) {
    flag_array++;// пропускаем первое тире
        for(; *flag_array != '\0'; flag_array++) {
            switch(*flag_array) {
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


