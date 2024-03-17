#include "s21_decimal.h"

void setbit(s21_decimal *number, int index, int bit){///установка бита
    int poz = index/32;
    int n = index%32;
    int mask = 1 << n; 
    if (bit == 1){
        number->bits[poz] = number->bits[poz] | mask;
    }
    if(bit == 0){
        number->bits[poz] = number->bits[poz] & ~mask;
    }
}

int getbit(s21_decimal *number, int index){///возвращает бит
    int bit;
    int poz = index/32;
    int n = index%32;
    bit = (number->bits[poz] >> n) & 1u;
    return bit;
}

int getsign(s21_decimal *number){///возвращает знак числа
    int sign = (int) ((number->bits[SCALE] >> 31) & 1u);
    return sign;
}

void setsign(s21_decimal *number, int bit){///устанавливает знак числа
    int mask = 1 << 31;
    if (bit == 1){
        number->bits[SCALE] = number->bits[SCALE] | mask;
    }
    if(bit == 0){
        number->bits[SCALE] = number->bits[SCALE] & ~mask;
    }
}

int getscale(s21_decimal *number){///возвращает масштаб
    int scale;
    scale = (number->bits[SCALE]&SC)>>16;
    return scale;
}

void setscale(s21_decimal *number, int scale){///устанавливает масштаб
    int sign = getsign(number);

    number->bits[SCALE] = scale;
    number->bits[SCALE] <<= 16;

    if (sign == 1) {
        setsign(number, 1);
    }
}

void shift_left(s21_decimal *number) {///сдвигает влево на еденицу числа в bits[0]-bits[2]
    int low_last_bit = getbit(number, 31);
    int mid_last_bit = getbit(number, 63);

    number->bits[LOW] <<= 1;
    number->bits[MID] <<= 1;
    number->bits[HIGH] <<= 1;

    setbit(number, 32, low_last_bit);
    setbit(number, 64, mid_last_bit);
}
int highest_bit(s21_decimal number) {///возвращает старший бит числа от 0-95
    int bit_position = -1;

    for (int i = 0; i < 96 && bit_position == -1; i++) {
        if (getbit(&number, (95 - i)) == 1) {
            bit_position = (95 - i);
        }
    }

    return bit_position;
}

int add_without_scale(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {///проверка на переполнение
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

int is_multiply_possible(s21_decimal number) {///проверяет возможность умножения на 10 без переполнения
    int is_multiply_possible = 0; // Инициализация переменной, указывающей на возможность умножения

    // Получение значений трех старших битов числа
    int bit_29 = getbit(&number, 93);
    int bit_30 = getbit(&number, 94);
    int bit_31 = getbit(&number, 95);

    // Проверка, что старшие биты равны 0
    if (bit_29 == 0 && bit_30 == 0 && bit_31 == 0) {
        // Создание копии числа, умноженного на 8 (сдвиг влево на 3 позиции)
        s21_decimal number_multiplied_by_8 = number;
        shift_left(&number_multiplied_by_8);
        shift_left(&number_multiplied_by_8);
        shift_left(&number_multiplied_by_8);

        // Создание копии числа, умноженного на 2 (сдвиг влево на 1 позицию)
        s21_decimal number_multiplied_by_2 = number;
        shift_left(&number_multiplied_by_2);

        // Создание переменной для хранения результата
        s21_decimal result = {{0, 0, 0, 0}};

        // Проверка, не происходит ли переполнения при сложении числа, умноженного на 8, и числа, умноженного на 2
        if (add_without_scale(number_multiplied_by_8, number_multiplied_by_2, &result) == 0) {
            is_multiply_possible = 1; // Если переполнения нет, умножение возможно
        }
    }

    return is_multiply_possible; // Возвращает 1, если умножение возможно, иначе 0
}

void multiply_by_10(s21_decimal *number) {///умножение числа на 10
    s21_decimal number_multiplied_by_8 = *number;
    shift_left(&number_multiplied_by_8);
    shift_left(&number_multiplied_by_8);
    shift_left(&number_multiplied_by_8);

    s21_decimal number_multiplied_by_2 = *number;
    shift_left(&number_multiplied_by_2);

    add_without_scale(number_multiplied_by_8, number_multiplied_by_2, number);
}

void shift_right(s21_decimal *number) {/// сдвиг в право на единицу bits[0]-bits[2]
    int mid_first_bit = getbit(number, 32);
    int high_first_bit = getbit(number, 64);

    number->bits[LOW] >>= 1;
    number->bits[MID] >>= 1;
    number->bits[HIGH] >>= 1;

    setbit(number, 63, high_first_bit);
    setbit(number, 31, mid_first_bit);
}
unsigned int divide_by_10(s21_decimal *number) {
    unsigned int buffer = 0;
    int scale_of_number = getscale(number);
    s21_decimal result = {{0, 0, 0, 0}};

    int highest_bit_position = highest_bit(*number);

    for (int i = 0; i < highest_bit_position + 2; i++) {
        if (buffer >= 10) {
            // Записываем 1 в результат
            shift_left(&result);
            setbit(&result, 0, 1);

            // Вычитаем 1010 из накопленных битов
            buffer = buffer - 10u;

            // Берём ещё бит
            int new_bit = getbit(number, highest_bit_position - i);
            buffer = buffer << 1;
            buffer |= new_bit;
        } else {
            // Записываем 0 в результат
            shift_left(&result);
            setbit(&result, 0, 0);

            // Берём ещё бит
            int new_bit = getbit(number, highest_bit_position - i);
            buffer = buffer << 1;
            buffer |= new_bit;
        }
    }

    buffer = buffer >> 1;
    *number = result;
    setscale(number, scale_of_number);

    return buffer;
}

void equalize_scale(s21_decimal *number1, s21_decimal *number2) {///приведение масштаба чисел к единому масштабу 
    int number1_scale = getscale(number1);
    int number2_scale = getscale(number2);
    int scale_diff = number1_scale - number2_scale;

    if (scale_diff < 0) {/// если разница отрицательная делаем ее положительной
        scale_diff *= -1;
    }

    if (scale_diff != 0) {/// если масштабы разные
        s21_decimal *bigger_scale_number;
        s21_decimal *smaller_scale_number;

        if (number1_scale > number2_scale) {///расставляем масштабы по величине
            bigger_scale_number = number1;
            smaller_scale_number = number2;
        } else {
            bigger_scale_number = number2;
            smaller_scale_number = number1;
        }

        for (int i = 0; i < scale_diff; i++) {
            // Если число с меньшим scale можно умножить на 10,
            // то делаем умножение и увеличиваем его scale на 1
            if (is_multiply_possible(*smaller_scale_number) == 1) {
                int current_scale = getscale(smaller_scale_number);

                multiply_by_10(smaller_scale_number);
                setscale(smaller_scale_number, current_scale + 1);
                // Если умножение невозможно,
                // то делим число с большим scale на 10 и уменьшаем его scale на 1
            } else {
                int current_scale = getscale(bigger_scale_number);

                if (i == scale_diff - 1) {///последний шаг цикла банковское округление
                    unsigned int last = bigger_scale_number->bits[LOW] % 10;///последний бит числа с большим масштабом
                    unsigned int penultimate = bigger_scale_number->bits[LOW] % 100 - last;///предпоследний бит числа с большим масштабом
                    int bigger_number_sign = getsign(bigger_scale_number);

                    divide_by_10(bigger_scale_number);
                    setscale(bigger_scale_number, current_scale - 1);

                    if ((last == 5) && (penultimate % 2) == 1) {/// если последний бит равен 5 и предпоследний не четный
                        s21_decimal result = {{0, 0, 0, 0}};

                        if (bigger_number_sign == 0) {///если число положительное
                            s21_decimal one = {{1, 0, 0, 0}};
                            add_without_scale(*bigger_scale_number, one, &result);///добавляем еденицу

                            *bigger_scale_number = result;
                        } else {///если число отрицательное
                            s21_decimal one = {{1, 0, 0, 0}};
                            setsign(&one, 1);
                            add_without_scale(*bigger_scale_number, one, &result);

                            *bigger_scale_number = result;
                        }
                    }
                } else {
                    divide_by_10(bigger_scale_number);
                    setscale(bigger_scale_number, current_scale - 1);
                }
            }
        }
    }
}

