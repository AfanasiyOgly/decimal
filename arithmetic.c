#include "s21_decimal.h"

int add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
    int exit_status = 0;
    int value1_scale = getScale(value_1);
    int value2_scale = getScale(value_2);
    int value1_sign = getSign(value_1);
    int value2_sign = getSign(value_2);

    if (value1_scale != value2_scale) {
        s21_equalize_scale(&value_1, &value_2);
    }

    if (value1_sign == value2_sign) {
        int new_scale = getScale(value_1);
        int add_result = add_without_scale(value_1, value_2, result);
        setDign(result, value1_sign);

        // Если произошло переполнение, делим на 10 и уменьшаем Scale на 1. Затем складывем снова
        if (add_result == 1 && new_scale != 0) {
            divide_by_10(&value_1);
            divide_by_10(&value_2);

            setScale(&value_1, new_scale - 1);
            setScale(&value_2, new_scale - 1);

            add_without_scale(value_1, value_2, result);
            setSign(result, value1_sign);
        } else if (add_result == 1 && value1_sign == 0) {
            exit_status = 1;
        } else if (add_result == 1 && value1_sign == 1) {
            exit_status = 2;
        }

        setScale(result, new_scale);
        } else {
        // Если 1 знак положительный, другой отрицательный
        // Меянем знак второго числа и вызываем функцию вычитания
        if (value2_sign == 0) {
            setSign(&value_2, 1);
        } else {
            setSign(&value_2, 0);
        }

        exit_status = sub(value_1, value_2, result);
    }

    return exit_status;
}


int sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int exit_status = 0;
    int value1_scale = getScale(value_1);
    int value2_scale = getScale(value_2);
    int value1_sign = getSign(value_1);
    int value2_sign = getSign(value_2);

    if (value1_scale != value2_scale) {
        equalize_scale(&value_1, &value_2);
    }

    // Если оба числа не положительные или оба не отрицательные
    // Вызываем функцию сложения
    if (value1_sign != value2_sign) {
        // Меняем знак второго числа перед сложением
        if (value2_sign == 0) {
            setSign(&value_2, 1);
        } else {
            setSign(&value_2, 0);
        }

        exit_status = add(value_1, value_2, result);
    } else {
        // Смотрим какое число больше и записываем указатели на них в соответсвующие переменные
        s21_decimal *bigger_number;
        s21_decimal *smaller_number;

        setSign(&value_1, 0);
        setSign(&value_2, 0);

        if (is_greater(value_1, value_2) == 1) {
            bigger_number = &value_1;
            smaller_number = &value_2;
        } else {
            bigger_number = &value_2;
            smaller_number = &value_1;
        }

        setSign(&value_1, value1_sign);
        setSign(&value_2, value2_sign);

        // Вычитаем меньшее из большего
        if (bigger_number->bits[LOW] < smaller_number->bits[LOW]) {
            if (bigger_number->bits[MID] != 0) {
                bigger_number->bits[MID] -= 1u;
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
            setSign(result, 1);
        }

        // Ставим нужный Scale
        int new_scale = getScale(value_1);
        setScale(result, new_scale);
    }

    return exit_status;
}

