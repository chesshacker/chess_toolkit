/*
 * Chess Toolkit: a software library for creating chess programs
 * Copyright (C) 2013 Steve Ortiz
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <config.h>
#include <check.h>
#include "chess_toolkit.h"

START_TEST(ut_square_make)
{
  ck_assert_int_eq(ct_square_make(FILE_A, RANK_1), A1);
  ck_assert_int_eq(ct_square_make(FILE_B, RANK_1), B1);
  ck_assert_int_eq(ct_square_make(FILE_H, RANK_1), H1);
  ck_assert_int_eq(ct_square_make(FILE_A, RANK_2), A2);
  ck_assert_int_eq(ct_square_make(FILE_A, RANK_8), A8);
  ck_assert_int_eq(ct_square_make(FILE_H, RANK_8), H8);
  /* this function does not check for valid input -- garbage in, garbage out */
  ck_assert_int_eq(ct_square_make(FILE_H + 1, RANK_8), A8);
  ck_assert_int_eq(ct_square_make(FILE_A - 1, RANK_1), SQUARE_NOT_FOUND);
  ck_assert_int_eq(ct_square_make(FILE_A - 1, RANK_2), SQUARE_NOT_FOUND);
  ck_assert_int_eq(ct_square_make(FILE_A, RANK_8 + 1), NUMBER_OF_SQUARES);
  ck_assert_int_eq(ct_square_make(FILE_A, RANK_1 - 1), -8);
} END_TEST

START_TEST(ut_square_file)
{
  ck_assert_int_eq(ct_square_file(A1), FILE_A);
  ck_assert_int_eq(ct_square_file(B1), FILE_B);
  ck_assert_int_eq(ct_square_file(H1), FILE_H);
  ck_assert_int_eq(ct_square_file(A2), FILE_A);
  ck_assert_int_eq(ct_square_file(A8), FILE_A);
  ck_assert_int_eq(ct_square_file(H8), FILE_H);
  /* this function does not check for valid input -- garbage in, garbage out */
  ck_assert_int_eq(ct_square_file(SQUARE_NOT_FOUND), FILE_H);
  ck_assert_int_eq(ct_square_file(NUMBER_OF_SQUARES), FILE_A);
} END_TEST

START_TEST(ut_square_rank)
{
  ck_assert_int_eq(ct_square_rank(A1), RANK_1);
  ck_assert_int_eq(ct_square_rank(B1), RANK_1);
  ck_assert_int_eq(ct_square_rank(H1), RANK_1);
  ck_assert_int_eq(ct_square_rank(A2), RANK_2);
  ck_assert_int_eq(ct_square_rank(A8), RANK_8);
  ck_assert_int_eq(ct_square_rank(H8), RANK_8);
  /* this function does not check for valid input -- garbage in, garbage out */
  ck_assert_int_eq(ct_square_rank(SQUARE_NOT_FOUND), RANK_NOT_FOUND);
  ck_assert_int_eq(ct_square_rank(NUMBER_OF_SQUARES), NUMBER_OF_RANKS);
} END_TEST

START_TEST(ut_rank_from_char)
{
  ck_assert_int_eq(ct_rank_from_char('1'), RANK_1);
  ck_assert_int_eq(ct_rank_from_char('3'), RANK_3);
  ck_assert_int_eq(ct_rank_from_char('8'), RANK_8);
  /* this function does not check for valid input -- garbage in, garbage out */
  ck_assert_int_eq(ct_rank_from_char('0'), RANK_NOT_FOUND);
  ck_assert_int_eq(ct_rank_from_char('9'), NUMBER_OF_RANKS);
} END_TEST

START_TEST(ut_file_from_char)
{
  ck_assert_int_eq(ct_file_from_char('a'), FILE_A);
  ck_assert_int_eq(ct_file_from_char('c'), FILE_C);
  ck_assert_int_eq(ct_file_from_char('h'), FILE_H);
  /* this function does not check for valid input -- garbage in, garbage out */
  ck_assert_int_eq(ct_file_from_char('A'), -32);
  ck_assert_int_eq(ct_file_from_char('`'), FILE_NOT_FOUND);
  ck_assert_int_eq(ct_file_from_char('i'), NUMBER_OF_FILES);
} END_TEST

START_TEST(ut_rank_to_char)
{
  ck_assert_int_eq(ct_rank_to_char(RANK_1), '1');
  ck_assert_int_eq(ct_rank_to_char(RANK_4), '4');
  ck_assert_int_eq(ct_rank_to_char(RANK_8), '8');
  /* this function does not check for valid input -- garbage in, garbage out */
  ck_assert_int_eq(ct_rank_to_char(RANK_NOT_FOUND), '0');
  ck_assert_int_eq(ct_rank_to_char(NUMBER_OF_RANKS), '9');
} END_TEST

START_TEST(ut_file_to_char)
{
  ck_assert_int_eq(ct_file_to_char(FILE_A), 'a');
  ck_assert_int_eq(ct_file_to_char(FILE_D), 'd');
  ck_assert_int_eq(ct_file_to_char(FILE_H), 'h');
  /* this function does not check for valid input -- garbage in, garbage out */
  ck_assert_int_eq(ct_file_to_char(FILE_NOT_FOUND), '`');
  ck_assert_int_eq(ct_file_to_char(NUMBER_OF_FILES), 'i');
} END_TEST

Suite *
ut_square_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_square");
  test_case = tcase_create("Square");
  tcase_add_test(test_case, ut_square_make);
  tcase_add_test(test_case, ut_square_file);
  tcase_add_test(test_case, ut_square_rank);
  tcase_add_test(test_case, ut_file_from_char);
  tcase_add_test(test_case, ut_rank_from_char);
  tcase_add_test(test_case, ut_file_to_char);
  tcase_add_test(test_case, ut_rank_to_char);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
