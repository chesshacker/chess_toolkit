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

static char *starting_fen = "8/8/1p6/8/8/8/P7/8 w - -";
static char *expected_fens[] =
{
  "8/8/8/P7/1p6/8/8/8 w - -",        /* a4 b5 a5 b5 */
  "8/8/8/8/p7/8/8/8 w - -",        /* a3 b5 a4 bxa4 */
  "8/8/8/8/Pp6/8/8/8 w - -",        /* a3 b5 a4 b4 */
  ""
};
static char **current_fen = expected_fens;
static CtGraph graph = 0;

static void verify_expected_fen(void *ignore);

START_TEST(ut_graph_dfs)
{
  CtCommandStruct command = ct_command_make(0, verify_expected_fen);

  graph = ct_graph_new();
  ck_assert(ct_graph_from_fen(graph, starting_fen) != 0);

  ct_graph_dfs(graph, &command, 4);
  ck_assert_int_eq(current_fen - expected_fens, 3);        /* verified all expected fens */

  ct_graph_free(graph);
} END_TEST

static void
verify_expected_fen(void *ignore)
{
  ck_assert_str_eq(ct_graph_to_fen(graph, 0), *current_fen);
  current_fen++;
}

Suite *
ut_graph_dfs_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_graph_dfs");
  test_case = tcase_create("Graph.dfs");
  tcase_add_test(test_case, ut_graph_dfs);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
