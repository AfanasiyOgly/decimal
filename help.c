#include "s21_decimal.h"


int getBits(s21_decimal decimal, int index) {
    int bit_decimal = index / 32;
    index = index % 32;
    int bit = (int) ((decimal.bits[bit_decimal] >> index) & 1u);

    return bit;
}

void setBits(int decimal, int index, int bit){
    int mask = 1 << index; // Создаем маску с единицей на позиции p
    return ((decimal & ~mask) | (bit << index)); // Сначала сбрасываем бит на позиции p, затем устанавливаем его на значение b


}

void setSign(s21_decimal *decimal, int sign) {
    // Устанавливаем знак + (0)
    if (sign == 0) {
        decimal->bits[SCALE] <<= 1;
        decimal->bits[SCALE] >>= 1;
    }

    // Устанавливаем знак - (1)
    if (sign == 1) {
        int mask = 0b10000000000000000000000000000000;
        decimal->bits[SCALE] = decimal->bits[SCALE] | mask;
    }
}

int getSign(s21_decimal decimal) {
    int sign = (int) ((decimal.bits[SCALE] >> 31) & 1u);

    return sign;
}
int getScale(s21_decimal decimal) {
    int scale = 0;
    decimal.bits[SCALE] >>= 16;

    for (int j = 0; j < 8; j++) {
        scale += (int) (((decimal.bits[SCALE] >> j) & 1u) * (int) pow(2, j));
    }

    return scale;
}


void setScale(s21_decimal *decimal, int scale) {
    int sign = s21_get_sign(*decimal);

    decimal->bits[SCALE] = scale;
    decimal->bits[SCALE] <<= 16;

    if (sign == 1) {
        setSign(decimal, 1);
    }
}
void shiftLeft(s21_decimal *decimal) {
    int low_last_bit = s21_get_bit(*decimal, 31);
    int mid_last_bit = s21_get_bit(*decimal, 63);

    decimal->bits[LOW] <<= 1;
    decimal->bits[MID] <<= 1;
    decimal->bits[HIGH] <<= 1;

    setBits(&decimal->bits[MID], 0, low_last_bit);
    setBits(&decimal->bits[HIGH], 0, mid_last_bit);
}


void shiftRight(s21_decimal *decimal) {
    int mid_first_bit = getBits(*decimal, 32);
    int high_first_bit = getBits(*decimal, 64);

    decimal->bits[LOW] >>= 1;
    decimal->bits[MID] >>= 1;
    decimal->bits[HIGH] >>= 1;

    setBits(&decimal->bits[MID], 31, high_first_bit);
    setBits(&decimal->bits[LOW], 31, mid_first_bit);
}
void equalize_scale(s21_decimal *decimal1, s21_decimal *decimal2) {
    int decimal1_scale = getScale(*decimal1);
    int decimal2_scale = getScale(*decimal2);
    int scale_diff = decimal1_scale - decimal2_scale;

    if (scale_diff < 0) {
        scale_diff *= -1;
    }

    if (scale_diff != 0) {
        s21_decimal *bigger_scale_decimal;
        s21_decimal *smaller_scale_decimal;

        if (decimal1_scale > decimal2_scale) {
            bigger_scale_decimal = decimal1;
            smaller_scale_decimal = decimal2;
        } else {
            bigger_scale_decimal = decimal2;
            smaller_scale_decimal = decimal1;
        }

        for (int i = 0; i < scale_diff; i++) {
            // Если число с меньшим scale можно умножить на 10,
            // то делаем умножение и увеличиваем его scale на 1
            if (is_multiply_possible(*smaller_scale_decimal) == 1) {
                int current_scale = getScale(*smaller_scale_decimal);

                multiply_by_10(smaller_scale_decimal);
                setScale(smaller_scale_decimal, current_scale + 1);
                // Если умножение невозможно,
                // то делим число с большим scale на 10 и уменьшаем его scale на 1
            } else {
                int current_scale = getScale(*bigger_scale_decimal);

                if (i == scale_diff - 1) {
                    unsigned int last = bigger_scale_decimal->bits[LOW] % 10;
                    unsigned int penultimate = bigger_scale_decimal->bits[LOW] % 100 - last;
                    int bigger_decimal_sign = getSign(*bigger_scale_decimal);

                    divide_by_10(bigger_scale_decimal);
                    setScale(bigger_scale_decimal, current_scale - 1);

                    if ((last == 5) && (penultimate % 2) == 1) {
                        s21_decimal result = {{0, 0, 0, 0}};

                        if (bigger_decimal_sign == 0) {
                            s21_decimal s21_one = {{1, 0, 0, 0}};
                            add_without_scale(*bigger_scale_decimal, s21_one, &result);

                            *bigger_scale_decimal = result;
                        } else {
                            s21_decimal s21_one = {{1, 0, 0, 0}};
                            setSign(&s21_one, 1);
                            add_without_scale(*bigger_scale_decimal, s21_one, &result);

                            *bigger_scale_decimal = result;
                        }
                    }
                } else {
                    divide_by_10(bigger_scale_decimal);
                    setScale(bigger_scale_decimal, current_scale - 1);
                }
            }
        }
    }
}

int is_multiply_possible(s21_decimal decimal) {
    int is_multiply_possible = 0;
    int bit_29 = getBits(decimal, 93);
    int bit_30 = getBits(decimal, 94);
    int bit_31 = getBits(decimal, 95);

    if (bit_29 == 0 && bit_30 == 0 && bit_31 == 0) {
        s21_decimal number_multiplied_by_8 = decimal;
        shiftLeft(&number_multiplied_by_8);
        shiftLeft(&number_multiplied_by_8);
        shiftLeft(&number_multiplied_by_8);

        s21_decimal number_multiplied_by_2 = decimal;
        shiftLeft(&number_multiplied_by_2);

        s21_decimal result = {{0, 0, 0, 0}};

        if (add_without_scale(number_multiplied_by_8,
                                  number_multiplied_by_2,
                                  &result) == 0) {
            is_multiply_possible = 1;
        }
    }

    return is_multiply_possible;
}

int add_without_scale(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int is_overflow = 0;

    // Находим меньшее LOW число
    unsigned int lowest_number = value_1.bits[LOW];

    if (value_1.bits[LOW] > value_2.bits[LOW]) {
        lowest_number = value_2.bits[LOW];
    }

    // В случае переполнение увеличиваем MID на 1
    if (value_1.bits[LOW] + value_2.bits[LOW] < lowest_number) {
        result->bits[MID] = 1u;
    } else {
        result->bits[MID] = 0;
    }

    // Складываем два LOW числа
    result->bits[LOW] = value_1.bits[LOW] + value_2.bits[LOW];

    // Находим меньшее MID число
    lowest_number = value_1.bits[MID];

    if (value_1.bits[MID] > value_2.bits[MID]) {
        lowest_number = value_2.bits[MID];
    }

    // В случае переполнение увеличиваем HIGH на 1
    if (value_1.bits[MID] + value_2.bits[MID] + result->bits[MID] < lowest_number) {
        result->bits[HIGH] = 1u;
    } else {
        result->bits[HIGH] = 0;
    }

    // Складываем два MID числа
    result->bits[MID] += value_1.bits[MID] + value_2.bits[MID];

    // Находим меньшее HIGH число
    lowest_number = value_1.bits[HIGH];

    if (value_1.bits[HIGH] > value_2.bits[HIGH]) {
        lowest_number = value_2.bits[HIGH];
    }

    // В случае переполнения ставим флаг
    if (value_1.bits[HIGH] + value_2.bits[HIGH] + result->bits[HIGH] < lowest_number) {
        result->bits[LOW] = 0;
        result->bits[MID] = 0;
        result->bits[HIGH] = 0;
        result->bits[SCALE] = 0;
        is_overflow = 1;
    } else {
        // Складываем два HIGH числа
        result->bits[HIGH] += value_1.bits[HIGH] + value_2.bits[HIGH];
    }

    return is_overflow;
}

unsigned int divide_by_10(s21_decimal *decimal) {
    unsigned int buffer = 0;
    int scale_of_number = getScale(*decimal);
    s21_decimal result = {{0, 0, 0, 0}};

    int highest_bit_position = get_highest_bit(*decimal);

    for (int i = 0; i < highest_bit_position + 2; i++) {
        if (buffer >= 10) {
            // Записываем 1 в результат
            shiftLeft(&result);
            setBits(&result.bits[LOW], 0, 1);

            // Вычитаем 1010 из накопленных битов
            buffer = buffer - 10u;

            // Берём ещё бит
            int new_bit = getBits(*decimal, highest_bit_position - i);
            buffer = buffer << 1;
            setBits(&buffer, 0, new_bit);
        } else {
            // Записываем 0 в результат
            shiftLeft(&result);
            setBits(&result.bits[LOW], 0, 0);

            // Берём ещё бит
            int new_bit = getBits(*decimal, highest_bit_position - i);
            buffer = buffer << 1;
            setBits(&buffer, 0, new_bit);
        }
    }

    buffer = buffer >> 1;
    *decimal = result;
    setScale(decimal, scale_of_number);

    return buffer;
}

int get_highest_bit(s21_decimal decimal) {
    int bit_position = -1;

    for (int i = 0; i < 96 && bit_position == -1; i++) {
        if (getBits(decimal, (95 - i)) == 1) {
            bit_position = (95 - i);
        }
    }

    return bit_position;
}

void multiply_by_10(s21_decimal *decimal) {
    s21_decimal number_multiplied_by_8 = *decimal;
    shiftLeft(&number_multiplied_by_8);
    shiftLeft(&number_multiplied_by_8);
    shiftLeft(&number_multiplied_by_8);

    s21_decimal number_multiplied_by_2 = *decimal;
    shiftLeft(&number_multiplied_by_2);

    add_without_scale(number_multiplied_by_8, number_multiplied_by_2, decimal);
}

int is_greater(s21_decimal number1, s21_decimal number2) {
    int is_greater = 0;
    int number1_scale = getScale(number1);
    int number2_scale = getScale(number2);
    int value1_sign = getSign(number1);
    int value2_sign = getSign(number2);

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