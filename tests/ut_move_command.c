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

static void
save_squares(void *self, CtMove move)
{
  CtSquare *squares = self;

  squares[0] = ct_move_from(move);
  squares[1] = ct_move_to(move);
}

START_TEST(ut_move_command_execute)
{
  CtSquare squares[2] = {0, 0};
  CtMoveCommand move_command = ct_move_command_new(squares, save_squares);
  CtMoveCommandStruct move_command_struct = ct_move_command_make(squares, save_squares);

  ct_move_command_execute(move_command, ct_move_make(E2, E4));
  ck_assert_int_eq(squares[0], E2);
  ck_assert_int_eq(squares[1], E4);

  ct_move_command_execute(&move_command_struct, ct_move_make_castle_queenside(E8));
  ck_assert_int_eq(squares[0], E8);
  ck_assert_int_eq(squares[1], C8);

  ct_move_command_free(move_command);
} END_TEST

Suite *
ut_move_command_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_move_command");
  test_case = tcase_create("MoveCommand");
  tcase_add_test(test_case, ut_move_command_execute);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
