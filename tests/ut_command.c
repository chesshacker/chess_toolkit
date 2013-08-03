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

static void ut_command_increment(void *delegate);

START_TEST(ut_command_execute)
{
  int counter = 0;
  CtCommand command = ct_command_new(&counter, ut_command_increment);
  CtCommandStruct command_struct = ct_command_make(&counter, ut_command_increment);

  ct_command_execute(command);
  ck_assert_int_eq(counter, 1);
  ct_command_execute(command);
  ck_assert_int_eq(counter, 2);
  ct_command_execute(&command_struct);
  ck_assert_int_eq(counter, 3);

  ct_command_free(command);
} END_TEST

static void
ut_command_increment(void *delegate)
{
  int *counter = (int *) delegate;

  *counter += 1;
}

Suite *
ut_command_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_command");
  test_case = tcase_create("Command");
  tcase_add_test(test_case, ut_command_execute);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
