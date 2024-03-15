#include "s21_grep.h"

int main(int argc, char** argv) {
  options flags = {0};
  read_args(&flags, argc, argv);
  return 0;
}

// Чтение содержимого файла и запись в массив
char** read_file(char* path, int* flag, options* flags) {
  FILE* file;
  file = fopen(path, "r");
  char* content = malloc(1 * sizeof(char));
  char** result = NULL;
  int count = 0;
  flags->str_count = 0;
  if (file) {
    while (!feof(file)) {
      content = realloc(content, count * sizeof(char) + 1);
      content[count] = '\0';
      fscanf(file, "%c", &content[count]);
      if (content[count] == '\n') {
        flags->str_count++;
      }
      count++;
    }
    fclose(file);
    flags->str_count++;
    result = sep_content(content, flags);
  } else {
    *flag = 0;
  }
  free(content);
  return result;
}
// Разбиение содержимого файла на строки
char** sep_content(char* content, options* flags) {
  int j = 0;
  char** res = malloc(flags->str_count * sizeof(char*));
  for (int i = 0; i < flags->str_count; i++) {
    res[i] = malloc(sizeof(char) * 2);
    int count = 0;
    for (; content[j] != '\0' && content[j] != '\n'; j++) {
      res[i] = realloc(res[i], (2 + count) * sizeof(char));
      res[i][count] = content[j];
      count++;
    }
    res[i][count] = '\0';
    j++;
  }
  return res;
}
// Чтение аргументов
void read_args(options* flags, int argc, char** argv) {
  int flag = 1;
  int templ_flag = 0;
  char** templates = malloc(sizeof(char*));
  char** file_name = malloc(sizeof(char*));
  for (int i = 1; i < argc && flag != 0; i++) {
    if (flag == 2) {
      flags->template_count++;
      templates = save_arg(argv[i], templates, 0, flags->template_count);
      flag = 1;
    } else if (flag == 3) {
      flag = 0;
      templates = flag_f(templates, argv[i], 0, &flag, flags);
    } else if (argv[i][0] == '-') {
      flags->flags_count++;
      templates = check_flags(templates, flags, argv[i], &flag);
    } else if (templ_flag == 0 && flags->template_count == 0) {
      flags->template_count++;
      templates = save_arg(argv[i], templates, 0, flags->template_count);
      templ_flag = 1;
    } else {
      flags->files_count++;
      file_name = save_arg(argv[i], file_name, 0, flags->files_count);
    }
  }
  if (flag == 1 && flags->files_count != 0) {
    for (int i = 0; i < flags->files_count; i++) {
      working_with_files(file_name[i], templates, flags);
    }
  } else {
    printf("Error! Something goes wrong! Bye!\n");
  }
  check_free(file_name, flags->files_count);
  check_free(templates, flags->template_count);
}
// Проверка флагов на валидность
char** check_flags(char** templates, options* flags, char* argv,
                   int* flag_tmp) {
  int stop = 0;
  for (int i = 1; i < (int)strlen(argv) && !stop; i++) {
    if (argv[i] == 'e') {
      stop = 1;
      if (strlen(argv) > 2 && i + 1 < (int)strlen(argv)) {
        flags->template_count++;
        templates = save_arg(argv, templates, i + 1, flags->template_count);
      } else
        *flag_tmp = 2;
    } else if (argv[i] == 'i') {
      flags->i = 1;
    } else if (argv[i] == 'v') {
      flags->v = 1;
    } else if (argv[i] == 'c') {
      flags->c = 1;
    } else if (argv[i] == 'l') {
      flags->l = 1;
    } else if (argv[i] == 'n') {
      flags->n = 1;
    } else if (argv[i] == 'h') {
      flags->h = 1;
    } else if (argv[i] == 's') {
      flags->s = 1;
    } else if (argv[i] == 'f') {
      if (strlen(argv) > 2 && i + 1 < (int)strlen(argv)) {
        templates = flag_f(templates, argv, i + 1, flag_tmp, flags);
        stop = 1;
      } else
        *flag_tmp = 3;
    } else if (argv[i] == 'o') {
      flags->o = 1;
    } else {
      printf("Option %s does not exist!\n", argv);
      *flag_tmp = 0;
    }
  }
  return templates;
}
// Записываем аргумент в массив
char** save_arg(char* arg, char** templates, int flag, int size) {
  templates = realloc(templates, size * sizeof(char*));
  templates[size - 1] = malloc((strlen(arg) - flag + 1) * sizeof(char));
  for (int i = flag; i < (int)strlen(arg); i++) {
    templates[size - 1][i - flag] = arg[i];
  }
  templates[size - 1][strlen(arg) - flag] = '\0';
  return templates;
}
// Манипуляции с флагом f
char** flag_f(char** templates, char* arg, int ind, int* flag, options* flags) {
  char* path = malloc((strlen(arg) - ind + 1) * sizeof(char));
  for (int i = ind; i < (int)strlen(arg); i++) {
    path[i - ind] = arg[i];
  }
  path[strlen(arg) - ind] = '\0';
  *flag = 1;
  char** tmp = read_file(path, flag, flags);
  if (flag) {
    for (int i = 0; i < flags->str_count; i++) {
      flags->template_count++;
      templates = save_arg(tmp[i], templates, 0, flags->template_count);
    }
  }
  check_free(tmp, flags->str_count);
  free(path);
  return templates;
}
// Обработка содержимого файлов
void working_with_files(char* file, char** templates, options* flags) {
  int flag = 1;
  char** content = read_file(file, &flag, flags);
  if (flag) {
    if ((flags->o != 1 || flags->v) || flags->l || flags->c) {
      grep_casual(content, templates, file, flags);
    } else {
      for (int i = 0; i < flags->str_count; i++) {
        flag_o(content[i], templates, file, i, flags);
      }
    }
  } else if (flags->s != 1) {
    printf("Files %s does not exist!\n", file);
  }
  check_free(content, flags->str_count);
}
// Работа обычного грепа со всеми флагами
void grep_casual(char** content, char** templates, char* file_name,
                 options* flags) {
  int flag_l = 0;
  int count = 0;
  for (int i = 0; i < flags->str_count && !flag_l; i++) {
    int find = 0;
    if (content[i][0] == '\0' && i + 1 == flags->str_count) {
      continue;
    }
    for (int j = 0; j < flags->template_count && !find; j++) {
      if ((strstr(content[i], templates[j]) != NULL && flags->i == 0) ||
          (str_ignore_case(content[i], templates[j]) && flags->i == 1)) {
        find = 1;
      }
    }
    if ((flags->v == 1 && !find) || (flags->v == 0 && find)) {
      print_casual_grep(content[i], file_name, flags, i, &flag_l, &count);
    }
  }
  if (flags->c) {
    if (flags->l && count != 0) {
      count = 1;
    }
    another_flags_check(flags, 0, file_name);
    printf("%d\n", count);
    if (flags->l && count != 0) {
      printf("%s\n", file_name);
    }
  }
}
// Вывод результата с учетом всех флагов
void print_casual_grep(char* content, char* file_name, options* flags, int ind,
                       int* f1, int* count) {
  if (flags->l != 1 && flags->c != 1) {
    another_flags_check(flags, ind, file_name);
    printf("%s\n", content);
  } else if (flags->c != 1) {
    printf("%s\n", file_name);
    *f1 = 1;
  } else {
    (*count)++;
  }
}
// Проверка кастомизирующих флагов
void another_flags_check(options* flags, int index, char* file_name) {
  if (flags->h == 0 && flags->files_count > 1) {
    printf("%s:", file_name);
  }
  if (flags->c == 0 && flags->n == 1) {
    printf("%d:", index + 1);
  }
}
// Флаг o
void flag_o(char* content, char** templates, char* file, int ind,
            options* flags) {
  int size_con = strlen(content);
  int count = 0;
  for (int i = 0; i < size_con; i++) {
    char* res = NULL;
    for (int k = 0; k < flags->template_count; k++) {
      if (content[i] == *(templates[k])) {
        int flag = 0;
        int size_tem = strlen(templates[k]);
        for (int j = 0;
             j < size_tem && i + size_tem <= size_con && (flag || j == 0);
             j++) {
          if (content[i + j] == templates[k][j]) {
            flag = 1;
          } else {
            flag = 0;
          }
        }
        if (flag) {
          if (res == NULL || (strlen(res) < strlen(templates[k]))) {
            res = templates[k];
            count++;
          }
        }
      }
    }
    if (res != NULL) {
      if (count <= 1) {
        another_flags_check(flags, ind, file);
      }
      printf("%s\n", res);
      i += strlen(res) - 1;
    }
  }
}
// Перевод всех букв в нижний регистр
char* str_low(char* string) {
  char* res = malloc((strlen(string) + 1) * sizeof(char));
  for (int i = 0; i < (int)strlen(string); i++) {
    if (string[i] >= 65 && string[i] <= 90) {
      res[i] = string[i] + 32;
    } else {
      res[i] = string[i];
    }
  }
  res[strlen(string)] = '\0';
  return res;
}
// Проверка на наличие подстроки в строке без учета регистра
int str_ignore_case(char* content, char* temp) {
  int flag = 0;
  char* cont_low = str_low(content);
  char* temp_low = str_low(temp);
  if (strstr(cont_low, temp_low) != NULL) {
    flag = 1;
  }
  if (cont_low) {
    free(cont_low);
  }
  if (temp_low) {
    free(temp_low);
  }
  return flag;
}
// Освобождение памяти из под массивов
void check_free(char** array, int size) {
  for (int i = 0; i < size; i++) {
    if (array[i] != NULL) {
      free(array[i]);
    }
  }
  if (array) {
    free(array);
  }
}
