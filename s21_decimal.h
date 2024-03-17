#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

typedef struct s21_decimal{

    int bits[4];

}s21_decimal;

enum {LOW, MID, HIGH, SCALE};
#define SC 0x00ff0000


/*help*/
void setbit(s21_decimal *number, int index, int bit);
int getbit(s21_decimal *number, int index);
int getsign(s21_decimal *number);
void setsign(s21_decimal *number, int bit);
int getscale(s21_decimal *number);
void setscale(s21_decimal *number, int scale);




/*arithmetic*/
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);



#endif  //  SRC_S21_DECIMAL_H_
