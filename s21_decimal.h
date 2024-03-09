#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

typedef struct s21_decimal{

    int bits[4];

}s21_decimal;

enum {LOW, MID, HIGH, SCALE};


/*help*/
int getBits(s21_decimal decimal, int index);
void setBits(int number, int index, int bit);
void setSign(s21_decimal *decimal, int sign);
int getSign(s21_decimal decimal);
void setScale(s21_decimal *decimal, int scale);
int getScale(s21_decimal decimal);



void equalize_scale(s21_decimal *decimal1, s21_decimal *decimal2);
void shiftLeft(s21_decimal *decimal);
void shiftRight(s21_decimal *decimal);
int is_multiply_possible(s21_decimal decimal);
int add_without_scale(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);///Складывает 2 числа с одинаковым scale
unsigned int divide_by_10(s21_decimal *number);///деление на 10, scale не меняется остаток остается в буфере
int get_highest_bit(s21_decimal decimal);///находит старший бит
void multiply_by_10(s21_decimal *decimal);///умножает на 10




int is_greater(s21_decimal number1, s21_decimal number2);///Сравнивает больше ли число 1, чем число 2


/*arithmetic*/
int add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);



#endif  //  SRC_S21_DECIMAL_H_