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

typedef struct UtMoveToSTestStruct *UtMoveToSTest;
typedef struct UtMoveToSTestStruct
{
  bool end_of_tests;
  CtMove move;
  char *expected_result;
} UtMoveToSTestStruct;

typedef struct UtGraphMoveToSanTestStruct *UtGraphMoveToSanTest;
typedef struct UtGraphMoveToSanTestStruct
{
  bool end_of_tests;
  char *load_fen;
  CtMove move;
  char *expected_result;
} UtGraphMoveToSanTestStruct;

START_TEST(ut_move_to_s)
{
  UtMoveToSTestStruct tests[] =
  {
    {false, ct_move_make(E2, E4), "e2e4"},
    {false, ct_move_make_castle_kingside(E1), "O-O"},
    {false, ct_move_make_castle_queenside(E8), "O-O-O"},
    {false, ct_move_make_promotion_n(D2, D1), "d2d1=N"},
    {true, NULL_MOVE, 0}
  };
  UtMoveToSTest test;
  char to_str[CT_MOVE_TO_S_MAX_LENGTH];
  char *result;

  for (test = tests; !test->end_of_tests; test++)
  {
    result = ct_move_to_s(test->move, to_str);
    ck_assert_str_eq(result, test->expected_result);
    ck_assert(result == to_str);
    result = ct_move_to_s(test->move, 0);
    ck_assert_str_eq(result, test->expected_result);
  }
} END_TEST

START_TEST(ut_graph_move_to_san)
{
  UtGraphMoveToSanTestStruct tests[] =
  {
    {false, 0, ct_move_make(E2, E4), "e4"},
    {false, 0, ct_move_make(G1, F3), "Nf3"},
    {false, "k1K5/8/8/8/8/8/8/1Q6 w - -", ct_move_make(B1, A1), "Qa1#"},
    {false, 0, ct_move_make(B1, H1), "Qh1+"},
    {false, "8/8/8/pp6/P7/8/8/8 w - -", ct_move_make(A4, B5), "axb5"},
    {false, "8/8/8/8/8/N7/8/N7 w - -", ct_move_make(A1, C2), "N1c2"},
    {false, "8/8/8/8/8/8/8/N3N3 w - -", ct_move_make(A1, C2), "Nac2"},
    {false, "8/8/8/8/8/N7/8/N3N3 w - -", ct_move_make(A1, C2), "Na1c2"},
    {false, "5k2/8/8/8/8/8/8/4K2R w K -", ct_move_make_castle_kingside(E1), "O-O+"},
    {false, "r1b2rk1/p1qn1pp1/2p2n1p/Pp6/2p2R2/2N2NP1/1PQ1PPBP/R5K1 w - b6", ct_move_make_en_passant_capture(A5, B6), "axb6"},
    {true, 0, NULL_MOVE, 0}
  };
  UtGraphMoveToSanTest test;
  CtGraph graph = ct_graph_new();
  char to_s[CT_SAN_MAX_LENGTH];
  char *result;

  for (test = tests; !test->end_of_tests; test++)
  {
    if (test->load_fen)
      ck_assert_msg(ct_graph_from_fen(graph, test->load_fen) != 0, test->load_fen);
    result = ct_graph_move_to_san(graph, test->move, to_s);
    ck_assert_str_eq(to_s, test->expected_result);
    ck_assert(to_s == result);
    result = ct_graph_move_to_san(graph, test->move, 0);
    ck_assert_str_eq(result, test->expected_result);
  }

  /* one more test making moves instead of simply loading a fen */
  ct_graph_reset(graph);
  ct_graph_make_move(graph, ct_move_make(G1, F3));
  ct_graph_make_move(graph, ct_move_make(E7, E5));
  result = ct_graph_move_to_san(graph, ct_move_make(F3, E5), 0);
  ck_assert_str_eq(result, "Nxe5");

  ct_graph_free(graph);
} END_TEST

Suite *
ut_move_writer_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_move_writer");
  test_case = tcase_create("Move.to_s");
  tcase_add_test(test_case, ut_move_to_s);
  suite_add_tcase(test_suite, test_case);
  test_case = tcase_create("Graph.move_to_san");
  tcase_add_test(test_case, ut_graph_move_to_san);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
