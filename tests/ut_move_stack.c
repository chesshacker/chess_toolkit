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

static CtMoveStack move_stack;

static void setup(void);
static void teardown(void);
static void ut_move_stack_verify_for_each(void *delegate, CtMove move);

static void
setup(void)
{
  move_stack = ct_move_stack_new();
}

static void
teardown(void)
{
  ct_move_stack_free(move_stack);
}

START_TEST(ut_move_stack_push_and_pop)
{
  CtSquare square;
  CtMove move;
  int x;

  for (x = 0; x < 5; x++)
  {
    for (square = A1; square < H8; square++)
    {
      move = ct_move_make(square, square + 1);
      ct_move_stack_push(move_stack, move);
    }
  }

  for (x = 0; x < 5; x++)
  {
    for (square = H8; square > A1; square--)
    {
      move = ct_move_stack_pop(move_stack);
      ck_assert_int_eq(move, ct_move_make(square - 1, square));
    }
  }
} END_TEST

START_TEST(ut_move_stack_is_empty)
{
  CtMove move = ct_move_make(E2, E4);

  ck_assert(ct_move_stack_is_empty(move_stack));

  ct_move_stack_push(move_stack, move);
  ck_assert(!ct_move_stack_is_empty(move_stack));

  ct_move_stack_pop(move_stack);
  ck_assert(ct_move_stack_is_empty(move_stack));

  /* any move (including NULL_MOVE) makes the stack not empty */
  ct_move_stack_push(move_stack, NULL_MOVE);
  ck_assert(!ct_move_stack_is_empty(move_stack));

  move = ct_move_stack_pop(move_stack);
  ck_assert(ct_move_stack_is_empty(move_stack));

  ck_assert_int_eq(move, NULL_MOVE);
} END_TEST

START_TEST(ut_move_stack_reset)
{
  CtMove move = ct_move_make(E2, E3);

  ct_move_stack_push(move_stack, move);
  ck_assert(!ct_move_stack_is_empty(move_stack));

  ct_move_stack_reset(move_stack);

  ck_assert(ct_move_stack_is_empty(move_stack));
} END_TEST

START_TEST(ut_move_stack_pop_when_empty)
{
  CtMove move;

  ck_assert(ct_move_stack_is_empty(move_stack));
  move = ct_move_stack_pop(move_stack);
  ck_assert_int_eq(move, NULL_MOVE);
} END_TEST

START_TEST(ut_move_stack_push_command)
{
  CtMoveCommand push_command = ct_move_stack_push_command(move_stack);
  CtSquare square;
  CtMove move;

  for (square = A1; square < H8; square++)
  {
    move = ct_move_make(square, square + 1);
    ct_move_command_execute(push_command, move);
  }

  for (square = H8; square > A1; square--)
  {
    move = ct_move_stack_pop(move_stack);
    ck_assert_int_eq(move, ct_move_make(square - 1, square));
  }
} END_TEST

START_TEST(ut_move_stack_for_each)
{
  CtSquare square;
  CtMove move;
  CtMoveCommand verify_for_each = ct_move_command_new(&square, ut_move_stack_verify_for_each);

  for (square = A1; square < H8; square++)
  {
    move = ct_move_make(square, square + 1);
    ct_move_stack_push(move_stack, move);
  }
  square = A1;
  ct_move_stack_for_each(move_stack, verify_for_each);
  ck_assert(square == H8);
  ct_move_command_free(verify_for_each);
} END_TEST

static void
ut_move_stack_verify_for_each(void *delegate, CtMove move)
{
  CtSquare *square_ptr = (CtSquare *) delegate;
  CtSquare square = (*square_ptr)++;

  ck_assert_int_eq(move, ct_move_make(square, square + 1));
}

START_TEST(ut_move_stack_length)
{
  CtSquare square;
  CtMove move;
  int count;

  ck_assert_int_eq(ct_move_stack_length(move_stack), 0);
  for (square = A1, count = 1; square < H8; square++, count++)
  {
    move = ct_move_make(square, square + 1);
    ct_move_stack_push(move_stack, move);
    ck_assert_int_eq(ct_move_stack_length(move_stack), count);
  }
  for (square = H8, count--; square > A1; square--, count--)
  {
    ck_assert_int_eq(ct_move_stack_length(move_stack), count);
    move = ct_move_stack_pop(move_stack);
  }
  ck_assert_int_eq(ct_move_stack_length(move_stack), 0);
} END_TEST

Suite *
ut_move_stack_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_move_stack");
  test_case = tcase_create("MoveStack");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_move_stack_push_and_pop);
  tcase_add_test(test_case, ut_move_stack_is_empty);
  tcase_add_test(test_case, ut_move_stack_reset);
  tcase_add_test(test_case, ut_move_stack_pop_when_empty);
  tcase_add_test(test_case, ut_move_stack_push_command);
  tcase_add_test(test_case, ut_move_stack_for_each);
  tcase_add_test(test_case, ut_move_stack_length);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
