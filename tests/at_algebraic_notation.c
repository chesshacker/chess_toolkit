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
#include <stdlib.h>

static CtGraph graph;
static CtCommand check_position;
static CtMoveCommand check_move;

static void setup(void);
static void teardown(void);
static void at_algebraic_notation_check_position(void *delegate);
static void at_algebraic_notation_check_move(void *ignore, CtMove move);

static void
setup(void)
{
  graph = ct_graph_new();
  check_position = ct_command_new(graph, at_algebraic_notation_check_position);
  check_move = ct_move_command_new(0, at_algebraic_notation_check_move);
}

static void
teardown(void)
{
  ct_move_command_free(check_move);
  ct_command_free(check_position);
  ct_graph_free(graph);
}

START_TEST(at_algebraic_notation)
{
  int depth;

  ck_assert(ct_graph_from_fen(graph, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -") != 0);
  at_algebraic_notation_check_position(graph);
  for (depth = 1; depth <= 3; depth++)
    ct_graph_dfs(graph, check_position, depth);
} END_TEST

static void
at_algebraic_notation_check_position(void *delegate)
{
  CtGraph delegate_graph = (CtGraph) delegate;

  ct_graph_for_each_legal_move(delegate_graph, check_move);
}

static void
at_algebraic_notation_check_move(void *ignore, CtMove move)
{
  CtMove parsed_move;
  char *notation = ct_graph_move_to_san(graph, move, 0);

  parsed_move = ct_graph_move_from_san(graph, notation);
  if (parsed_move != move)
  {
    char msg[CT_POSITION_TO_S_MAX_LENGTH + CT_FEN_MAX_LENGTH + 100];
    char fen[CT_FEN_MAX_LENGTH];
    char ascii_display[CT_GRAPH_TO_S_MAX_LENGTH];

    ct_graph_to_fen(graph, fen);
    ct_graph_to_s(graph, ascii_display);
    snprintf(msg, sizeof(msg), "Failed to parse move = %d, notation = %s, parsed_move = %d from\nfen = \"%s\"\n%s",
             move, notation, parsed_move, fen, ascii_display);
    ck_abort_msg(msg);
  }
}

Suite *
at_algebraic_notation_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("at_algebraic_notation");
  test_case = tcase_create("AlgebraicNotation");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, at_algebraic_notation);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
