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
#include "check_utilities.h"

static const char *capture_error_message = "";

static void ut_error_simple_error_handler(const char *msg);

START_TEST(ut_error)
{
  check_redirect_stdout();        /* stdout is redirected to keep the test output clean */

  ct_error("ut_error");
  /* ct_error will exit here */
  ck_abort_msg("ct_error failed to exit");
} END_TEST

static void
ut_error_simple_error_handler(const char *msg)
{
  capture_error_message = msg;
}

START_TEST(ut_error_set_custom_handler)
{
  ct_error_set_custom_handler(ut_error_simple_error_handler);
  ct_error("ut_error_set_custom_handler");
  ck_assert_str_eq(capture_error_message, "ut_error_set_custom_handler");
} END_TEST

Suite *
ut_error_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_error");
  test_case = tcase_create("Error");
  if (!check_fork_no())                /* if ut_error ran with CK_FORK=no, the test framework would fail */
    tcase_add_exit_test(test_case, ut_error, -1);
  tcase_add_test(test_case, ut_error_set_custom_handler);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
