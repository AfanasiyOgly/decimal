CC=gcc
FLAG = -Wall -Werror -Wextra

CL_11 = -std=c11
ALL_FILE = help.c arithmetic.c 
ALL_FILE_O = help.o arithmetic.o 

all: clean s21_decimal.a

s21_decimal.a:
	@$(CC) $(FLAG_C) $(ALL_FILE)
	@ar rcs s21_decimal.a $(ALL_FILE_O)

test: s21_decimal.a
	@gcc test_s21_add.c s21_decimal.a -o tests.o
	@./tests.o

clean:
	@rm -f *.a
	@rm -f *.o

leaks: $(ALL_FILE_O)
	@CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./tests.o