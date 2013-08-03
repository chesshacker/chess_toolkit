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
#include <stdio.h>

enum
{
  LONG_PERFT_TIMEOUT = 600,        /* in seconds */
  LONG_PERFT_LENGTH = 1000000        /* any perft with at least this many nodes is considered long */
};

enum
{
  END_OF_PERFT = -1,
  MAX_DEPTH_OF_PERFT = 10
};

typedef struct AtPerftTestStruct
{
  char *fen;
  int answers[MAX_DEPTH_OF_PERFT];
} AtPerftTestStruct;

static bool skip_long_perfts = true;
static CtGraph graph;
static int node_count;
static CtCommand count_nodes;

static void setup(void);
static void teardown(void);
static void run_all_perfts(AtPerftTestStruct * problem_list);
static void run_perft(int problem, int *answers);
static void at_perft_count_nodes(void *ignore);

static void
setup(void)
{
  graph = ct_graph_new();
  count_nodes = ct_command_new(0, at_perft_count_nodes);
}

static void
teardown(void)
{
  ct_command_free(count_nodes);
  ct_graph_free(graph);
}

static void
run_all_perfts(AtPerftTestStruct * problem_list)
{
  AtPerftTestStruct *problem = problem_list;

  for (; problem->fen != NULL; problem++)
  {
    ck_assert_msg(ct_graph_from_fen(graph, problem->fen) != 0, problem->fen);
    run_perft(problem - problem_list + 1, problem->answers);
  }
}

static void
run_perft(int problem, int *answers)
{
  int depth;

  for (depth = 1; *answers != END_OF_PERFT; answers++, depth++)
  {
    node_count = 0;
    if (*answers >= LONG_PERFT_LENGTH && skip_long_perfts)
      return;
    ct_graph_dfs(graph, count_nodes, depth);
    if (*answers != node_count)
    {
      char msg[100 + CT_GRAPH_TO_S_MAX_LENGTH];

      snprintf(msg, sizeof(msg), "Failed problem %d at depth %d: *answers=%d, node_count=%d\n%s",
               problem, depth, *answers, node_count, ct_graph_to_s(graph, 0));
      ck_abort_msg(msg);
    }
  }
}

static void
at_perft_count_nodes(void *ignore)
{
  node_count++;
}

START_TEST(at_perft_long)
{
  AtPerftTestStruct problem_list[] = {
    /* http://chessprogramming.wikispaces.com/Perft+Results */
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", {20, 400, 8902, 197281, 4865609, END_OF_PERFT}},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", {48, 2039, 97862, 4085603, END_OF_PERFT}},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", {14, 191, 2812, 43238, 674624, 11030083, END_OF_PERFT}},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -", {6, 264, 9467, 422333, 15833292, END_OF_PERFT}},
    {"rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq -", {42, 1352, 53392, END_OF_PERFT}},
    {NULL, {END_OF_PERFT}}
  };

  run_all_perfts(problem_list);
} END_TEST

Suite *
at_perft_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("at_perft");
  test_case = tcase_create("Perft");
  skip_long_perfts = !check_long();
  if (!skip_long_perfts)
    tcase_set_timeout(test_case, LONG_PERFT_TIMEOUT);
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, at_perft_long);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
