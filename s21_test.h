#ifndef S21_TESTS_H_
#define S21_TESTS_H_

#include <check.h>
#include <stdio.h>
#include "s21_decimal.h"

Suite *s21_add_cases(void);



void run_tests(void);
void run_testcase(Suite *testcase);


#endif  // S21_TESTS_H_
