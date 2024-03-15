#include "s21_cat.h"

int main(int argc, char **argv) {
  read_arguments(argv, argc);
  return 0;
}

void cat_solution(char **argv, int argc, options *flags) {
  int flag = 1;
  int count = 0;
  for (int i = 1; i < argc; i++) {
    flag = 1;
    if (argv[i][0] != '-') {
      char **file = malloc((flags->flags_count + 1) * sizeof(char *));
      file[0] = read_file(argv[i], &flag);
      if (!flag) {
        printf("cat: %s: No such file or directory", argv[i]);
      } else {
        go_to_options(file, flags);
        count++;
      }
      int stop = 0;
      if (file) {
        for (int i = 0; i < flags->flags_count + 1 && !stop; i++) {
          if (file[i] != NULL) {
            free(file[i]);
          }
          if (!flag) {
            stop = 1;
          }
        }
        free(file);
      }
    }
  }
}
// Прогон содержимого файла по флагам
void go_to_options(char **content, options *flags) {
  int count = 0;
  if (flags->t == 1) {
    count++;
    content[count] = flag_t(content[count - 1]);
  }
  if (flags->v == 1) {
    count++;
    content[count] = flag_v(content[count - 1]);
  }
  if (flags->s == 1) {
    count++;
    content[count] = flag_s(content[count - 1]);
  }
  if (flags->b == 1 || (flags->b == 1 && flags->n == 1)) {
    count++;
    content[count] = line_numbering(content[count - 1], '\n');
  }
  if (flags->e == 1) {
    count++;
    content[count] = flag_e(content[count - 1]);
  }
  if (flags->n == 1 && flags->b == 0) {
    count++;
    content[count] = line_numbering(content[count - 1], '\0');
  }
  printf("%s", content[count]);
}
// Обработка флагов
void read_arguments(char **args, int num) {
  if (num > 1) {
    options flags = {0};
    int flag = 1;
    for (int i = 1; i < num && flag; i++) {
      if (args[i][0] == '-') {
        if (args[i][1] != '-' && args[i][1] != 'E' && args[i][1] != 'T') {
          check_flags(args[i], &flags, &flag);
        } else {
          check_gnu(args[i], &flags, &flag);
        }
      } else {
        flags.files_count++;
      }
    }
    if (flag && flags.files_count != 0) {
      cat_solution(args, num, &flags);
    } else {
      printf("Not enough arguments!\n");
    }
  } else {
    printf("Not enough arguments!\n");
  }
}
// Проверка обычных флагов
void check_flags(char *arg, options *flags, int *flag) {
  for (int i = 1; i < (int)strlen(arg) && flag; i++) {
    switch (arg[i]) {
      case 'b':
        flags->flags_count += flags->b ? 0 : 1;
        flags->b = 1;
        if (flags->n == 1) flags->flags_count--;
        break;
      case 'e':
        flags->flags_count += flags->e ? 0 : 1;
        flags->e = 1;
        if (flags->v == 0) {
          flags->flags_count++;
          flags->v = 1;
        }
        break;
      case 'n':
        flags->flags_count += flags->n ? 0 : 1;
        flags->n = 1;
        if (flags->b == 1) flags->flags_count--;
        break;
      case 's':
        flags->flags_count += flags->s ? 0 : 1;
        flags->s = 1;
        break;
      case 't':
        flags->flags_count += flags->t ? 0 : 1;
        flags->t = 1;
        if (flags->v == 0) {
          flags->flags_count++;
          flags->v = 1;
        }
        break;
      case 'v':
        flags->flags_count += flags->v ? 0 : 1;
        flags->v = 1;
        break;
      default:
        *flag = 0;
        printf("ERROR!\nWrong option: %s\n", arg);
        break;
    }
  }
}
// Проверка GNU флагов
void check_gnu(char *arg, options *flags, int *flag) {
  if (strcmp(GNU_B, arg) == 0) {
    flags->flags_count += flags->b ? 0 : 1;
    flags->b = 1;
  } else if (strcmp(GNU_E, arg) == 0) {
    flags->flags_count += flags->e ? 0 : 1;
    flags->e = 1;
  } else if (strcmp(GNU_N, arg) == 0) {
    flags->flags_count += flags->n || flags->b ? 0 : 1;
    flags->n = 1;
  } else if (strcmp(GNU_S, arg) == 0) {
    flags->flags_count += flags->s ? 0 : 1;
    flags->s = 1;
  } else if (strcmp(GNU_T, arg) == 0) {
    flags->flags_count += flags->t == 1 ? 0 : 1;
    flags->t = 1;
  } else {
    printf("ERROR!\nWrong option: %s\n", arg);
    *flag = 0;
  }
}

// Чтение содержимого файла (Просто cat)
char *read_file(char *path, int *flag) {
  FILE *file;
  file = fopen(path, "r");
  char *content = malloc(1 * sizeof(char));
  int count = 0;
  if (file) {
    while (!feof(file)) {
      content = realloc(content, count * sizeof(char) + 1);
      fscanf(file, "%c", &content[count]);
      count++;
    }
    fclose(file);
    content[--count] = '\0';
  } else {
    *flag = 0;
  }
  return content;
}
// Флаг -b и -n в зависимости от флага
char *line_numbering(char *content, char flag) {
  char *new_res = malloc(1 * sizeof(char));
  int count = 0;
  int str_count = 1;
  char *tmp;
  for (int i = 0; content[i] != '\0'; i++) {
    new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
    if ((i == 0 || (content[i - 1] == '\n')) && content[i] != flag) {
      tmp = convert_to_string(str_count);
      str_count++;
      for (int j = 0; j < (int)strlen(tmp); j++) {
        new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
        new_res[count] = tmp[j];
        count++;
      }
      new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
      free(tmp);
    }
    new_res[count] = content[i];
    count++;
  }
  new_res[count] = '\0';
  return new_res;
}
// Флаг -e
char *flag_e(char *content) {
  char *new_res = malloc(1 * sizeof(char));
  int count = 0;
  for (int i = 0; content[i] != '\0'; i++) {
    new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
    if (content[i] == '\n') {
      new_res = realloc(new_res, (count + 2) * sizeof(char) + 1);
      new_res[count] = '$';
      count++;
    }
    new_res[count] = content[i];
    count++;
  }
  new_res[count] = '\0';
  return new_res;
}
// Флаг -s
char *flag_s(char *content) {
  char *new_res = malloc(1 * sizeof(char));
  int count = 0;
  int ln_count = 0;
  for (int i = 0; content[i] != '\0'; i++) {
    new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
    if ((i == 0) || (content[i] != '\n')) {
      ln_count = 0;
      new_res[count] = content[i];
      count++;
    } else if (content[i] == '\n' && ln_count < 2) {
      new_res[count] = content[i];
      ln_count++;
      count++;
    }
  }
  new_res[count] = '\0';
  return new_res;
}
// Флаг -t
char *flag_t(char *content) {
  char *new_res = malloc(1 * sizeof(char));
  int count = 0;
  for (int i = 0; content[i] != '\0'; i++) {
    new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
    if (content[i] != '\t') {
      new_res[count] = content[i];
      count++;
    } else {
      for (int j = 0; j < (int)strlen(REP_TAB); j++) {
        new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
        new_res[count] = REP_TAB[j];
        count++;
      }
    }
  }
  new_res[count] = '\0';
  return new_res;
}
// Флаг -v
char *flag_v(char *content) {
  char *new_res = malloc(1 * sizeof(char));
  int count = 0;
  for (int i = 0; content[i] != '\0'; i++) {
    if ((content[i] >= 0 && content[i] <= 31 && content[i] != 10 &&
         content[i] != 9) ||
        content[i] == 127) {
      new_res = realloc(new_res, (count + 2) * sizeof(char) + 1);
      new_res[count] = '^';
      count++;
      if (content[i] != 127) {
        new_res[count] = content[i] + 64;
      } else {
        new_res[count] = '?';
      }
      count++;
      continue;
    }
    new_res = realloc(new_res, (count + 1) * sizeof(char) + 1);
    new_res[count] = content[i];
    count++;
  }
  new_res[count] = '\0';
  return new_res;
}
// Перевод числа в строку
char *convert_to_string(int numb) {
  char *str_numb = malloc(1 * sizeof(char));
  int count = 0;
  while (numb / 10 != 0) {
    str_numb = realloc(str_numb, (count + 1) * sizeof(char) + 1);
    str_numb[count] = numb % 10 + '0';
    numb /= 10;
    count++;
  }
  str_numb[count] = numb % 10 + '0';
  count++;
  int length = count;
  for (int i = 0; i < 6 - length; i++) {
    str_numb = realloc(str_numb, (count + 1) * sizeof(char) + 1);
    str_numb[count] = ' ';
    count++;
  }
  for (int i = 0; i < count / 2; i++) {
    char tmp = str_numb[i];
    str_numb[i] = str_numb[count - 1 - i];
    str_numb[count - 1 - i] = tmp;
  }
  str_numb = realloc(str_numb, (count + 3) * sizeof(char) + 1);
  str_numb[count] = '\t';
  count++;
  str_numb[count] = '\0';
  return str_numb;
}
