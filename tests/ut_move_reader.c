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

typedef struct UtMoveReaderTestStruct *UtMoveReaderTest;
typedef struct UtMoveReaderTestStruct
{
  bool end_of_tests;
  char fen[CT_FEN_MAX_LENGTH];
  char notation[20];
  CtMove expected_move;
} UtMoveReaderTestStruct;

static CtGraph graph;

static void setup(void);
static void teardown(void);

static void
setup(void)
{
  graph = ct_graph_new();
}

static void
teardown(void)
{
  ct_graph_free(graph);
}

START_TEST(ut_graph_move_from_san)
{
  UtMoveReaderTestStruct tests[] =
  {
    {false, "", "e4", ct_move_make(E2, E4)},
    {false, "", "e2e4", ct_move_make(E2, E4)},
    {false, "", "e2-e4", ct_move_make(E2, E4)},
    {false, "", "e2xe4", ct_move_make(E2, E4)},        /* move reader does not check that 'x' is used properly */
    {false, "", "2e4", ct_move_make(E2, E4)},        /* move reader does not complain about strange things you shouldn't
                                                   write */
    {false, "", "e5", NULL_MOVE},        /* it will return NULL_MOVE if you give it something it can't do */
    {false, "", "f3", ct_move_make(F2, F3)},
    {false, "", "Nf3", ct_move_make(G1, F3)},
    {false, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -", "f6", ct_move_make(F7, F6)},
    {false, "", "Nf6", ct_move_make(G8, F6)},
    {false, "5k2/8/8/8/8/p6p/P6P/R3K2R w KQ -", "O-O", ct_move_make_castle_kingside(E1)},
    {false, "", "0-0+", ct_move_make_castle_kingside(E1)},
    {false, "", "O-O-O", ct_move_make_castle_queenside(E1)},
    {false, "", "0-0-0", ct_move_make_castle_queenside(E1)},
    {false, "r3k2r/p6p/P6P/8/8/8/8/6R1 b kq -", "O-O-O", ct_move_make_castle_queenside(E8)},
    {false, "rnbqkbnr/pppp1ppp/8/4p3/4PP2/8/PPPP2PP/RNBQKBNR b KQkq -", "exf4", ct_move_make(E5, F4)},
    {false, "r6r/8/8/8/8/8/8/8 b - -", "Rae8", ct_move_make(A8, E8)},
    {false, "", "Rhe8", ct_move_make(H8, E8)},
    {false, "8/Q2Q4/8/8/8/8/8/3Q4 w - -", "Qad4", ct_move_make(A7, D4)},
    {false, "", "Qdd4", AMBIGUOUS_MOVE},
    {false, "", "Q7d4", AMBIGUOUS_MOVE},
    {false, "", "Qd7d4", ct_move_make(D7, D4)},
    {false, "", "Q1a4", ct_move_make(D1, A4)},
    {false, "", "Q1a4!", ct_move_make(D1, A4)},        /* it ignores ! and ? at the end */
    {false, "", "Q1a4?", ct_move_make(D1, A4)},
    {false, "", "Q1a4!?", ct_move_make(D1, A4)},
    {false, "", "Q1a4!!", ct_move_make(D1, A4)},
    {false, "", "Q1a4+", ct_move_make(D1, A4)},        /* it also ignores trailing + and # -- it doesn't check that they are
                                                   used properly */
    {false, "", "Q1a4#", ct_move_make(D1, A4)},
    {false, "", "Q1xa4#+!#??#!", ct_move_make(D1, A4)},
    {false, "", "Q1xa4#+!#??#!b!", NULL_MOVE},        /* throw on something other than a +, #, ! or ? though and it will
                                                   return NULL_MOVE */
    {false, "r3k2r/Pppp1ppp/1b3nbN/nPP5/BB2P3/q4N2/Pp1P2PP/R2Q1RK1 b kq -", "b1=Q", ct_move_make_promotion_q(B2, B1)},
    {true, "", "", 0}
  };
  UtMoveReaderTest test;
  CtMove move;

  for (test = tests; !test->end_of_tests; test++)
  {
    if (strlen(test->fen) > 0)
      ck_assert_msg(ct_graph_from_fen(graph, test->fen) != 0, test->fen);
    move = ct_graph_move_from_san(graph, test->notation);
    if (move != test->expected_move)
    {
      char msg[CT_FEN_MAX_LENGTH + 100];

      snprintf(msg, sizeof(msg), "Failed fen=%s, notation=%s, expected_move=%d, move=%d",
               test->fen, test->notation, test->expected_move, move);
      ck_abort_msg(msg);
    }
  }
} END_TEST

Suite *
ut_move_reader_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_move_reader");
  test_case = tcase_create("Graph.move_from_san");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_graph_move_from_san);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
