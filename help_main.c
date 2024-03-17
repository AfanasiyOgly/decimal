#include <stdio.h>
#include "s21_decimal.h"
int main(){
    s21_decimal number;
    number.bits[3] = 0b10000000111111100000000000000000;
    number.bits[2] = 0b00000000000000000000000000000000;
    number.bits[1] = 0b00000000000000000000000000000000;
    number.bits[0] = 0b00000000000000000000000000000100;
    int bit = getbit(&number, 64);
    printf("getbit = %d\n", bit);
    int scale;
    scale = getscale(&number);
    printf("getscale = %d\n", scale);
    int sing;
    sing = getsign(&number);
    printf("getsign = %d\n", sing);
    setsign(&number, 0);
    sing = getsign(&number);
    printf("getsign = %d\n", sing);
    setbit(&number, 64, 1);
    bit = getbit(&number, 2);
    printf("getbit = %d\n", bit);
    setscale(&number, 56);
    scale = getscale(&number);
    printf("getscale = %d\n", scale);


    return 0;
}