#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REP_TAB "^I"

#define GNU_B "--number-nonblank"
#define GNU_E "-E"
#define GNU_N "--number"
#define GNU_S "--squeeze-blank"
#define GNU_T "-T"

typedef struct flags {
  int b;
  int e;
  int n;
  int t;
  int s;
  int v;
  int files_count;
  int flags_count;
} options;

char* line_numbering(char* content, char flag);
char* flag_e(char* content);
char* flag_s(char* content);
char* flag_t(char* content);
char* flag_v(char* content);

void read_arguments(char** args, int num);
void cat_solution(char** argv, int argc, options* flags);
void check_flags(char* arg, options* flags, int* flag);
void check_gnu(char* arg, options* flags, int* flag);
void go_to_options(char** content, options* flags);
char* read_file(char* path, int* flag);

char* convert_to_string(int numb);

#endif
