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
#include "ct_utilities.h"

static int number_of_errors = 0;

static void
ut_utilities_error_handler(const char *msg)
{
  number_of_errors++;
}

START_TEST(ut_malloc)
{
  int *numbers;

  numbers = ct_malloc(2 * sizeof(int));
  numbers[0] = 123;
  numbers[1] = 456;

  ck_assert_int_eq(numbers[0], 123);
  ck_assert_int_eq(numbers[1], 456);

  ct_free(numbers);
} END_TEST

START_TEST(ut_realloc)
{
  int *numbers;

  numbers = ct_malloc(sizeof(int));
  numbers[0] = 123;

  numbers = ct_realloc(numbers, 2 * sizeof(int));
  numbers[1] = 456;

  ck_assert_int_eq(numbers[0], 123);
  ck_assert_int_eq(numbers[1], 456);

  ct_free(numbers);
} END_TEST

START_TEST(ut_malloc_fail)
{
  void *ptr;

  ct_error_set_custom_handler(ut_utilities_error_handler);
  number_of_errors = 0;

  ptr = ct_malloc(-1);
  ck_assert_int_eq(number_of_errors, 1);

  ct_free(ptr);
} END_TEST

START_TEST(ut_realloc_fail)
{
  void *ptr;

  ct_error_set_custom_handler(ut_utilities_error_handler);
  number_of_errors = 0;

  ptr = ct_malloc(10);
  ck_assert_int_eq(number_of_errors, 0);
  ck_assert(ptr != 0);

  ptr = ct_realloc(ptr, -1);
  ck_assert_int_eq(number_of_errors, 1);

  ct_free(ptr);
} END_TEST

Suite *
ut_utilities_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_utilities");
  test_case = tcase_create("Utilities");
  tcase_add_test(test_case, ut_malloc);
  tcase_add_test(test_case, ut_realloc);
  tcase_add_test(test_case, ut_malloc_fail);
  tcase_add_test(test_case, ut_realloc_fail);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
