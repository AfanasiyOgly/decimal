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

void setsign(s21_decimal *number, int bit){
    int mask = 1 << 31;
    if (bit == 1){
        number->bits[SCALE] = number->bits[SCALE] | mask;
    }
    if(bit == 0){
        number->bits[SCALE] = number->bits[SCALE] & ~mask;
    }
}

int getscale(s21_decimal *number){
    int scale;
    scale = (number->bits[SCALE]&SC)>>16;
    return scale;
}

void setscale(s21_decimal *number, int scale){
    int sign = getsign(number);

    number->bits[SCALE] = scale;
    number->bits[SCALE] <<= 16;

    if (sign == 1) {
        setsign(number, 1);
    }
}