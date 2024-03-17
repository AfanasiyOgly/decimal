#include "s21_decimal.h"


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
int exit_status = 0;
    int value1_scale = getscale(&value_1);
    int value2_scale = getscale(&value_2);
    int value1_sign = getsign(&value_1);
    int value2_sign = getsign(&value_2);
    //приводим к общему масштабу
    if (value1_scale != value2_scale) {
        equalize_scale(&value_1, &value_2);
    }

    // Если знаки обоих чисел положительные или отрицательные
    if (value1_sign == value2_sign) {
        int new_scale = getscale(&value_1);
        int add_result = add_without_scale(value_1, value_2, result);
        setsign(result, value1_sign);

        // Если произошло переполнение, делим на 10 и уменьшаем Scale на 1. Затем складывем снова
        if (add_result == 1 && new_scale != 0) {
            divide_by_10(&value_1);
            divide_by_10(&value_2);

            setscale(&value_1, new_scale - 1);
            setscale(&value_2, new_scale - 1);

            add_without_scale(value_1, value_2, result);
            setsign(result, value1_sign);
        } else if (add_result == 1 && value1_sign == 0) {
            exit_status = 1;
        } else if (add_result == 1 && value1_sign == 1) {
            exit_status = 2;
        }

        setscale(result, new_scale);
    } else {
        // Если 1 знак положительный, другой отрицательный
        // Меянем знак второго числа и вызываем функцию вычитания
        if (value2_sign == 0) {
            setsign(&value_2, 1);
        } else {
            setsign(&value_2, 0);
        }

        exit_status = s21_sub(value_1, value_2, result);
    }

    return exit_status;
}
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
 int exit_status = 0;
    int value1_scale = getscale(&value_1);
    int value2_scale = getscale(&value_2);
    int value1_sign = getsign(&value_1);
    int value2_sign = getsign(&value_2);
    //приводим к общему  масштабу
    if (value1_scale != value2_scale) {
        equalize_scale(&value_1, &value_2);
    }

    // Если оба числа не положительные или оба не отрицательные
    // Вызываем функцию сложения
    if (value1_sign != value2_sign) {
        // Меняем знак второго числа перед сложением
        if (value2_sign == 0) {
            setsign(&value_2, 1);
        } else {
            setsign(&value_2, 0);
        }

        exit_status = s21_add(value_1, value_2, result);
    } else {
        // Смотрим какое число больше и записываем указатели на них в соответсвующие переменные
        s21_decimal *bigger_number;
        s21_decimal *smaller_number;

        setsign(&value_1, 0);
        setsign(&value_2, 0);

        if (is_greater(value_1, value_2) == 1) {
            bigger_number = &value_1;
            smaller_number = &value_2;
        } else {
            bigger_number = &value_2;
            smaller_number = &value_1;
        }

        setsign(&value_1, value1_sign);
        setsign(&value_2, value2_sign);

        // Вычитаем меньшее из большего
        if (bigger_number->bits[LOW] < smaller_number->bits[LOW]) {
            if (bigger_number->bits[MID] != 0) {///проверка на ненулевой средний бит
                bigger_number->bits[MID] -= 1u;///уменьшает средний бит на 1
                result->bits[LOW] = 4294967295u - smaller_number->bits[LOW] + bigger_number->bits[LOW];
            } else {
                bigger_number->bits[HIGH] -= 1u;
                bigger_number->bits[MID] = 4294967295u;
                result->bits[LOW] = 4294967295u - smaller_number->bits[LOW] + bigger_number->bits[LOW];
            }
        } else {
            result->bits[LOW] = bigger_number->bits[LOW] - smaller_number->bits[LOW];
        }

        if (bigger_number->bits[MID] < smaller_number->bits[MID]) {
            bigger_number->bits[HIGH] -= 1u;
            result->bits[MID] = 4294967295u - smaller_number->bits[MID] + bigger_number->bits[MID];
        } else {
            result->bits[MID] = bigger_number->bits[MID] - smaller_number->bits[MID];
        }

        result->bits[HIGH] = bigger_number->bits[HIGH] - smaller_number->bits[HIGH];

        // Ставим нужный знак
        if (is_greater(value_1, value_2) == 0) {
            setsign(result, 1);
        }

        // Ставим нужный Scale
        int new_scale = getscale(&value_1);
       setscale(result, new_scale);
    }

    return exit_status;
}

int is_greater(s21_decimal number1, s21_decimal number2) {///с равнение чисел, если первое больше то вернет 1 иначе вернет 0
    int is_greater = 0;
    int number1_scale = getscale(&number1);
    int number2_scale = getscale(&number2);
    int value1_sign = getsign(&number1);
    int value2_sign = getsign(&number2);

    if (number1_scale != number2_scale) {
        equalize_scale(&number1, &number2);
    }

    // Если 2 положительных числа или 2 отрицательных числа
    if (value1_sign == value2_sign) {
        if (number1.bits[HIGH] != number2.bits[HIGH]) {
            // Старший бит одного из чисел больше
            // Если старший бит 1 числа больше и числа положительные или
            // старший бит меньше и числа отрицательные, то оно больше
            if ((number1.bits[HIGH] > number2.bits[HIGH] && value1_sign == 0) ||
                (number1.bits[HIGH] < number2.bits[HIGH] && value1_sign == 1)) {
                is_greater = 1;
            }
        } else {
            if (number1.bits[MID] != number2.bits[MID]) {
                // Старшие биты равны, средний бит одного из чисел больше
                // Если средний бит 1 числа больше и числа положительные или
                // средний бит меньше и числа отрицательные, то оно больше
                if ((number1.bits[MID] > number2.bits[MID] && value1_sign == 0) ||
                    (number1.bits[MID] < number2.bits[MID] && value1_sign == 1)) {
                    is_greater = 1;
                }
            } else {
                if (number1.bits[LOW] != number2.bits[LOW]) {
                    // Старшие и средние биты равны, младший бит одного из чисел больше
                    // Если младший бит 1 числа больше и числа положительные или
                    // младшйи бит меньше и числа отрицательные, то оно больше
                    if ((number1.bits[LOW] > number2.bits[LOW] && value1_sign == 0) ||
                        (number1.bits[LOW] < number2.bits[LOW] && value1_sign == 1)) {
                        is_greater = 1;
                    }
                }
            }
        }
    } else {
        // Если 1 положительное и 1 отрицательное
        // Если первое положительное, то оно больше
        if (value1_sign == 0) {
            is_greater = 1;
        }
    }

    return is_greater;
}