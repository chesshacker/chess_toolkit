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
#include <stdio.h>

static CtGraph graph;
static CtMoveStack actual_move_stack;
static CtMoveStack expected_move_stack;

static void setup(void);
static void teardown(void);
static void ut_graph_test_fen(char *fen);
static void ut_graph_verify_move_made(void *delegate, CtMove move);

static void
setup(void)
{
  graph = ct_graph_new();
  actual_move_stack = ct_move_stack_new();
  expected_move_stack = ct_move_stack_new();
}

static void
teardown(void)
{
  ct_move_stack_free(expected_move_stack);
  ct_move_stack_free(actual_move_stack);
  ct_graph_free(graph);
}

START_TEST(ut_graph_for_each_legal_move)
{
  /* these three tests correspond to tests in ut_move_generator that produce illegal pseudo moves */
  /* black king -- won't move into check */
  ct_move_stack_push(expected_move_stack, ct_move_make(A7, B7));
  ut_graph_test_fen("7R/k7/7R/8/8/8/8/8 b - -");

  /* white rook -- must block check */
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B1));
  ut_graph_test_fen("8/8/8/8/8/8/RR6/K6r w - -");

  /* black rook -- must capture to get out of check */
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, C8));
  ut_graph_test_fen("k1Q4r/p7/P7/8/8/8/8/8 b - -");
} END_TEST

static void
ut_graph_test_fen(char *fen)
{
  CtMove move, expected_move;

  ck_assert_msg(ct_graph_from_fen(graph, fen) != 0, fen);
  ct_graph_for_each_legal_move(graph, ct_move_stack_push_command(actual_move_stack));
  while (!ct_move_stack_is_empty(actual_move_stack))
  {
    move = ct_move_stack_pop(actual_move_stack);
    expected_move = ct_move_stack_pop(expected_move_stack);
    if (move != expected_move)
    {
      char msg[CT_FEN_MAX_LENGTH + 100];
      char move_to_s[CT_MOVE_TO_S_MAX_LENGTH];
      char expected_move_to_s[CT_MOVE_TO_S_MAX_LENGTH];

      ct_move_to_s(move, move_to_s);
      ct_move_to_s(expected_move, expected_move_to_s);
      snprintf(msg, sizeof(msg), "Failed fen=%s, move=%s, expected_move=%s", fen, move_to_s, expected_move_to_s);
      ck_abort_msg(msg);
    }
  }
  ck_assert_msg(ct_move_stack_is_empty(expected_move_stack), fen);
}

START_TEST(ut_graph_make_unmake_ply)
{
  CtMove move;

  ck_assert_int_eq(ct_graph_ply(graph), 0);
  ct_graph_make_move(graph, ct_move_make(E2, E4));
  ck_assert_int_eq(ct_graph_ply(graph), 1);
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -");
  ct_graph_make_move(graph, ct_move_make(E7, E5));
  ck_assert_int_eq(ct_graph_ply(graph), 2);
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq -");
  move = ct_graph_unmake_move(graph);
  ck_assert_int_eq(move, ct_move_make(E7, E5));
  ck_assert_int_eq(ct_graph_ply(graph), 1);
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -");
  move = ct_graph_unmake_move(graph);
  ck_assert_int_eq(move, ct_move_make(E2, E4));
  ck_assert_int_eq(ct_graph_ply(graph), 0);
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
  /* unmaking when there is nothing to undo doesn't do anything */
  move = ct_graph_unmake_move(graph);
  ck_assert_int_eq(move, NULL_MOVE);
  ck_assert_int_eq(ct_graph_ply(graph), 0);
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
} END_TEST

START_TEST(ut_graph_for_each_move_made)
{
  CtMove moves[] = {0, ct_move_make(D2, D4), ct_move_make(D7, D5), ct_move_make(C2, C4), 0};
  CtMove *move_ptr;
  char *expected_fens[] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -",
    "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq -",
    "rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq -",
    0
  };
  char **next_expected_fen = &expected_fens[0];
  CtMoveCommand verify_moves = ct_move_command_new(&next_expected_fen, ut_graph_verify_move_made);

  for (move_ptr = &moves[1]; *move_ptr != 0; move_ptr++)
    ct_graph_make_move(graph, *move_ptr);
  for (move_ptr--; *move_ptr != 0; move_ptr--)        /* push in reverse order because they will be popped from last to first */
    ct_move_stack_push(expected_move_stack, *move_ptr);
  ct_graph_for_each_move_made(graph, verify_moves);
  ck_assert(ct_move_stack_is_empty(expected_move_stack));
  ck_assert(next_expected_fen == &expected_fens[3]);
} END_TEST

static void
ut_graph_verify_move_made(void *delegate, CtMove actual_move)
{
  char ***next_expected_fen_ptr = (char ***) delegate;
  char *expected_fen = **next_expected_fen_ptr;
  CtMove expected_move = ct_move_stack_pop(expected_move_stack);

  ck_assert_int_eq(actual_move, expected_move);
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), expected_fen);

  *next_expected_fen_ptr = (*next_expected_fen_ptr)++;
}

START_TEST(ut_graph_reset)
{
  ct_graph_make_move(graph, ct_move_make(E2, E4));
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -");
  ct_graph_reset(graph);
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
  ct_graph_unmake_move(graph);        /* unmake will not do anything after the reset */
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
} END_TEST

Suite *
ut_graph_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_graph");
  test_case = tcase_create("Graph");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_graph_for_each_legal_move);
  tcase_add_test(test_case, ut_graph_make_unmake_ply);
  tcase_add_test(test_case, ut_graph_for_each_move_made);
  tcase_add_test(test_case, ut_graph_reset);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
