CFLAGS = -Wall -Werror -Wextra
CC = gcc

all: s21_grep 

s21_grep: s21_grep.o
	$(CC) $(CFLAGS) $< -o $@

s21_grep.o: s21_grep.c
	$(CC) -Wall -c $< -o $@ 

test: s21_grep
	bash test_s21_grep.sh

clean:
	rm -rf *.o s21_grep

rebuild: clean all