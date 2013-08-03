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

static CtGraph graph;
static CtPosition position;

static void setup();
static void teardown();

static void
setup()
{
  graph = ct_graph_new();
  position = ct_position_new();
}

static void
teardown()
{
  ct_position_free(position);
  ct_graph_free(graph);
}

START_TEST(ut_graph_to_position)
{
  char *fen;
  char *expected_fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -";
  CtPosition result;

  ct_graph_make_move(graph, ct_move_make(E2, E4));
  result = ct_graph_to_position(graph, 0);
  fen = ct_position_to_fen(result, 0);
  ck_assert_str_eq(fen, expected_fen);

  /* test it again providing an existing position to store in */
  result = ct_graph_to_position(graph, position);
  fen = ct_position_to_fen(result, 0);
  ck_assert_str_eq(fen, expected_fen);
  ck_assert(result == position);
} END_TEST

START_TEST(ut_graph_from_position)
{
  CtGraph result;
  char *fen = "3K4/3P1k2/8/8/8/8/4R3/2r5 w - -";
  CtMove move;

  ct_position_from_fen(position, fen);
  result = ct_graph_from_position(graph, 0);
  ck_assert(result == 0);

  result = ct_graph_from_position(0, position);
  move = ct_graph_move_from_san(result, "Rf2+");
  ck_assert(move == ct_move_make(E2, F2));

  move = ct_graph_move_from_san(graph, "Rf2+");
  ck_assert(move == NULL_MOVE);
  result = ct_graph_from_position(graph, position);
  ck_assert(result == graph);
  move = ct_graph_move_from_san(result, "Rf2+");
  ck_assert(move == ct_move_make(E2, F2));
} END_TEST

START_TEST(ut_graph_to_fen)
{
  char buffer[CT_FEN_MAX_LENGTH];
  char *expected_fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -";
  char *result;

  ct_graph_make_move(graph, ct_move_make(E2, E4));
  result = ct_graph_to_fen(graph, 0);
  ck_assert_str_eq(result, expected_fen);

  /* test it again providing an existing position to store in */
  result = ct_graph_to_fen(graph, buffer);
  ck_assert_str_eq(result, expected_fen);
  ck_assert(result == buffer);
} END_TEST

START_TEST(ut_graph_from_fen)
{
  CtGraph result;
  char *fen = "3K4/3P1k2/8/8/8/8/4R3/2r5 w - -";
  CtMove move;

  result = ct_graph_from_fen(graph, 0);
  ck_assert(result == 0);

  result = ct_graph_from_fen(0, fen);
  move = ct_graph_move_from_san(result, "Rf2+");
  ck_assert(move == ct_move_make(E2, F2));

  move = ct_graph_move_from_san(graph, "Rf2+");
  ck_assert(move == NULL_MOVE);
  result = ct_graph_from_fen(graph, fen);
  ck_assert(result == graph);
  move = ct_graph_move_from_san(result, "Rf2+");
  ck_assert(move == ct_move_make(E2, F2));
} END_TEST

START_TEST(ut_graph_position_hash)
{
  char *fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -";
  int64_t result, expected_result;

  expected_result = ct_position_hash(position);
  result = ct_graph_position_hash(graph);
  ck_assert(result == expected_result);

  ct_graph_make_move(graph, ct_move_make(E2, E4));
  result = ct_graph_position_hash(graph);
  ct_position_from_fen(position, fen);
  expected_result = ct_position_hash(position);
  ck_assert(result == expected_result);
} END_TEST

START_TEST(ut_graph_to_s)
{
  char *fen = "3K4/3P1k2/8/8/8/8/4R3/2r5 w - -";
  char *moves[] = {"Rf2", "Kg7", "Rf4", 0};
  char **move_str;
  CtMove move;
  char *result;
  char buffer[CT_POSITION_TO_S_MAX_LENGTH];
  char *expected_result =
  " 8  - - - K - - - -\n"
  " 7  - - - P - - k -\n"
  " 6  - - - - - - - -\n"
  " 5  - - - - - - - -\n"
  " 4  - - - - - R - -\n"
  " 3  - - - - - - - -\n"
  " 2  - - - - - - - -\n"
  " 1  - - r - - - - -\n"
  "    a b c d e f g h\n"
  "Black - -\n"
  "Ply 3";

  ct_graph_from_fen(graph, fen);
  for (move_str = moves; *move_str; move_str++)
  {
    move = ct_graph_move_from_san(graph, *move_str);
    ck_assert(move != NULL_MOVE && move != AMBIGUOUS_MOVE);
    ct_graph_make_move(graph, move);
  }

  result = ct_graph_to_s(graph, 0);
  ck_assert_str_eq(result, expected_result);

  result = ct_graph_to_s(graph, buffer);
  ck_assert(result == buffer);
  ck_assert_str_eq(result, expected_result);
} END_TEST

Suite *
ut_graph_position_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_graph_position");
  test_case = tcase_create("Graph Position");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_graph_to_position);
  tcase_add_test(test_case, ut_graph_from_position);
  tcase_add_test(test_case, ut_graph_to_fen);
  tcase_add_test(test_case, ut_graph_from_fen);
  tcase_add_test(test_case, ut_graph_position_hash);
  tcase_add_test(test_case, ut_graph_to_s);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
