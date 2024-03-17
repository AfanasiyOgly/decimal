#include "../../s21_test.h"

START_TEST(invalid_dec_s21_add_1) {
  // ссылка на результат - NULL
  s21_decimal dec_1 = {{0x1, 0x0, 0x0, 0x0}};
  s21_decimal dec_2 = {{0x1, 0x0, 0x0, 0x0}};
  int result_value = s21_add(dec_1, dec_2, NULL);

  ck_assert_int_eq(result_value, 1);
}

START_TEST(invalid_dec_s21_add_2) {
  // степень 56 (допустимое значение от 0 до 28)
  // биты 0-15 не пустые
  // биты 24-30 не пустые
  s21_decimal dec_1 = {{0x0, 0x0, 0x0, 0x380000}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_3) {
  // степень 29 (допустимое значение от 0 до 28)
  s21_decimal dec_1 = {{0x1, 0x0, 0x0, 0x1D0000}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_4) {
  // степень 28
  // биты 0-15 не пустые
  s21_decimal dec_1 = {{0x1, 0x0, 0x0, 0x1C0001}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x140000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_5) {
  // степень 28
  // биты 0-15 не пустые
  s21_decimal dec_1 = {{0x1, 0x0, 0x0, 0x1C8000}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0x0, 0x140000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_6) {
  // степень 28
  // биты 24-30 не пустые
  s21_decimal dec_1 = {{0x1, 0x0, 0x0, 0x11C0000}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_7) {
  // степень 28
  // биты 24-30 не пустые
  s21_decimal dec_1 = {{0xA, 0x0, 0x0, 0x401C0000}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_8) {
  // Все биты имеют максимальное значение
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
  s21_decimal dec_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_9) {
  // степень 56 (допустимое значение от 0 до 28)
  // биты 0-15 не пустые
  // биты 24-30 не пустые
  s21_decimal dec_2 = {{0xA, 0x0, 0x0, 0x380000}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x140000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_10) {
  // степень 29 (допустимое значение от 0 до 28)
  s21_decimal dec_2 = {{0x1, 0x0, 0x0, 0x1D0000}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_11) {
  // степень 29 (допустимое значение от 0 до 28)
  s21_decimal dec_2 = {{0xA, 0x0, 0x0, 0x1D0000}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x20000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_12) {
  // степень 28
  // биты 0-15 не пустые
  s21_decimal dec_2 = {{0x1, 0x0, 0x0, 0x1C0001}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x20000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_13) {
  // степень 28
  // биты 0-15 не пустые
  s21_decimal dec_2 = {{0x1, 0x0, 0x0, 0x1C8000}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_14) {
  // степень 28
  // биты 24-30 не пустые
  s21_decimal dec_2 = {{0x1, 0x0, 0x0, 0x11C0000}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_15) {
  // степень 28
  // биты 24-30 не пустые
  s21_decimal dec_2 = {{0x1, 0x0, 0x0, 0x401C0000}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(invalid_dec_s21_add_16) {
  // Все биты имеют максимальное значение
  s21_decimal dec_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
  s21_decimal dec_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  s21_decimal result;
  int result_value = s21_add(dec_1, dec_2, &result);

  ck_assert_int_eq(result_value, 1);
}
END_TEST

START_TEST(s21_add_1) {
  // -1.9
  s21_decimal dec_1 = {{0x13, 0x0, 0x0, 0x80010000}};
  // 9463.525
  s21_decimal dec_2 = {{0x9066e5, 0x0, 0x0, 0x30000}};
  // 9461.625
  s21_decimal dec_check = {{0x905f79, 0x0, 0x0, 0x30000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_2) {
  // 17168808476198343782
  s21_decimal dec_1 = {{0x48849466, 0xee43dc58, 0x0, 0x0}};
  // -644279853052580940478603
  s21_decimal dec_2 = {{0xf62bf88b, 0x7b16ea5c, 0x886e, 0x80000000}};
  // -644262684244104742134821
  s21_decimal dec_check = {{0xada76425, 0x8cd30e04, 0x886d, 0x80000000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_3) {
  // -762757153011121980162
  s21_decimal dec_1 = {{0x2c330302, 0x5961c567, 0x29, 0x80000000}};
  // 7442
  s21_decimal dec_2 = {{0x1d12, 0x0, 0x0, 0x0}};
  // -762757153011121972720
  s21_decimal dec_check = {{0x2c32e5f0, 0x5961c567, 0x29, 0x80000000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
} END_TEST

START_TEST(s21_add_4) {
  // 420239419390193514712743582
  s21_decimal dec_1 = {{0x22293e9e, 0x92c6b891, 0x15b9d28, 0x0}};
  // 327564483.2416
  s21_decimal dec_2 = {{0xaba58aa0, 0x2fa, 0x0, 0x40000}};
  // 420239419390193515040308065.24
  s21_decimal dec_check = {{0xf88c31fc, 0x55a018b8, 0x87c963d9, 0x20000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_5) {
  // 1904212851619837373
  s21_decimal dec_1 = {{0xe588b5bd, 0x1a6d1f7f, 0x0, 0x0}};
  // -8312005042295236809
  s21_decimal dec_2 = {{0xd4368c9, 0x735a2c8c, 0x0, 0x80000000}};
  // -6407792190675399436
  s21_decimal dec_check = {{0x27bab30c, 0x58ed0d0c, 0x0, 0x80000000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_6) {
  // -49
  s21_decimal dec_1 = {{0x31, 0x0, 0x0, 0x80000000}};
  // -607
  s21_decimal dec_2 = {{0x25f, 0x0, 0x0, 0x80000000}};
  // -656
  s21_decimal dec_check = {{0x290, 0x0, 0x0, 0x80000000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_7) {
  // -93015156793539145182263858.12
  s21_decimal dec_1 = {{0x5ed8bb94, 0x5921fc7b, 0x1e0e088f, 0x80020000}};
  // -8300231921
  s21_decimal dec_2 = {{0xeebb7cf1, 0x1, 0x0, 0x80000000}};
  // -93015156793539153482495779.12
  s21_decimal dec_check = {{0xa01589b8, 0x5921fd3c, 0x1e0e088f, 0x80020000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_8) {
  // 97.0095
  s21_decimal dec_1 = {{0xecd6f, 0x0, 0x0, 0x40000}};
  // 33831699594620183428878740
  s21_decimal dec_2 = {{0x81fa5194, 0x1c26e061, 0x1bfc24, 0x0}};
  // 33831699594620183428878837.010
  s21_decimal dec_check = {{0xb9d02512, 0xf7dc7ce3, 0x6d50ed0d, 0x30000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_9) {
  // -4085059400
  s21_decimal dec_1 = {{0xf37d0f48, 0x0, 0x0, 0x80000000}};
  // 696466361401
  s21_decimal dec_2 = {{0x28a14c39, 0xa2, 0x0, 0x0}};
  // 692381302001
  s21_decimal dec_check = {{0x35243cf1, 0xa1, 0x0, 0x0}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_add_10) {
  // 95461126951059
  s21_decimal dec_1 = {{0x468fd093, 0x56d2, 0x0, 0x0}};
  // -9.27609763
  s21_decimal dec_2 = {{0x374a33a3, 0x0, 0x0, 0x80080000}};
  // 95461126951049.72390237
  s21_decimal dec_check = {{0xaab5ff5d, 0x7eed26b9, 0x205, 0x80000}};
  s21_decimal result;
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, 0);
  ck_assert_uint_eq(dec_check.bits[0], result.bits[0]);
  ck_assert_uint_eq(dec_check.bits[1], result.bits[1]);
  ck_assert_uint_eq(dec_check.bits[2], result.bits[2]);
  ck_assert_uint_eq(dec_check.bits[3], result.bits[3]);
}
END_TEST

START_TEST(fail_s21_add_1) {
  // 60646639049.5092451296234
  s21_decimal dec_1 = {{0x6adfc3ea, 0x9be13ea3, 0x806c, 0xd0000}};
  // 83453760985665705065512618.16
  s21_decimal dec_2 = {{0xff7b7a78, 0xd0503669, 0x1af72220, 0x20000}};
  s21_decimal result;
  // overflow
  int check = 3; // Результат слишком мал или отрицательная бесконечность.
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, check);
}
END_TEST

START_TEST(fail_s21_add_2) {
  // 860693753791403
  s21_decimal dec_1 = {{0xe17483ab, 0x30ecb, 0x0, 0x0}};
  // 58.29722231323802370861
  s21_decimal dec_2 = {{0x64da832d, 0x7a5ea32, 0x13c, 0x140000}};
  s21_decimal result;
  // overflow
  int check = 3; // Результат слишком мал или отрицательная бесконечность.
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, check);
}
END_TEST

START_TEST(fail_s21_add_3) {
  // -19.09056504041643747439462
  s21_decimal dec_1 = {{0x1491f366, 0x29138306, 0x19442, 0x80170000}};
  // 876083269661901
  s21_decimal dec_2 = {{0x814f4cd, 0x31ccb, 0x0, 0x0}};
  s21_decimal result;
  // overflow
  int check = 3; // Результат слишком мал или отрицательная бесконечность.
  int return_value = s21_add(dec_1, dec_2, &result);
  ck_assert_int_eq(return_value, check);
}
END_TEST

Suite *s21_add_cases(void) {
  Suite *c = suite_create("s21_add_cases");
  TCase *tc = tcase_create("s21_add_tc");

  tcase_add_test(tc, invalid_dec_s21_add_1);
  tcase_add_test(tc, invalid_dec_s21_add_2);
  tcase_add_test(tc, invalid_dec_s21_add_3);
  tcase_add_test(tc, invalid_dec_s21_add_4);
  tcase_add_test(tc, invalid_dec_s21_add_5);
  tcase_add_test(tc, invalid_dec_s21_add_6);
  tcase_add_test(tc, invalid_dec_s21_add_7);
  tcase_add_test(tc, invalid_dec_s21_add_8);
  tcase_add_test(tc, invalid_dec_s21_add_9);
  tcase_add_test(tc, invalid_dec_s21_add_10);
  tcase_add_test(tc, invalid_dec_s21_add_11);
  tcase_add_test(tc, invalid_dec_s21_add_12);
  tcase_add_test(tc, invalid_dec_s21_add_13);
  tcase_add_test(tc, invalid_dec_s21_add_14);
  tcase_add_test(tc, invalid_dec_s21_add_15);
  tcase_add_test(tc, invalid_dec_s21_add_16);
  tcase_add_test(tc, s21_add_1);
  tcase_add_test(tc, s21_add_2);
  tcase_add_test(tc, s21_add_3);
  tcase_add_test(tc, s21_add_4);
  tcase_add_test(tc, s21_add_5);
  tcase_add_test(tc, s21_add_6);
  tcase_add_test(tc, s21_add_7);
  tcase_add_test(tc, s21_add_8);
  tcase_add_test(tc, s21_add_9);
  tcase_add_test(tc, s21_add_10);
  tcase_add_test(tc, fail_s21_add_1);
  tcase_add_test(tc, fail_s21_add_2);
  tcase_add_test(tc, fail_s21_add_3);

  suite_add_tcase(c, tc);
  return c;
}
