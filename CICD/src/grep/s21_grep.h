#ifndef S21_GRPEP_H
#define S21_GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int files_count;
  int str_count;
  int flags_count;
  int template_count;
} options;

char **read_file(char *path, int *flag, options *flags);
char **sep_content(char *content, options *flags);

void read_args(options *flags, int argc, char **argv);
char **check_flags(char **templates, options *flags, char *argv, int *flag_tmp);

char **save_arg(char *arg, char **templates, int flag, int size);
char **flag_f(char **templates, char *arg, int ind, int *flag, options *flags);

void working_with_files(char *file, char **templates, options *flags);
void grep_casual(char **content, char **templates, char *file_name,
                 options *flags);
void print_casual_grep(char *content, char *file_name, options *flags, int ind,
                       int *f1, int *count);
void another_flags_check(options *flags, int index, char *file_name);
void flag_o(char *content, char **templates, char *file, int ind,
            options *flags);

char *str_low(char *string);
int str_ignore_case(char *content, char *temp);
void check_free(char **array, int size);

#endif
