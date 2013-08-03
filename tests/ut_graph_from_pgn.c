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
#include <stdlib.h>

static CtGameTags game_tags;
static CtGraph graph;
static char error_message[CT_GRAPH_FROM_PGN_ERROR_MESSAGE_MAX_LENGTH];

static void setup(void);
static void teardown(void);
static void ut_graph_from_pgn_check_carlsen_round_10(void *delegate);

static void
setup(void)
{
  game_tags = ct_game_tags_new();
  graph = ct_graph_new();
}

static void
teardown(void)
{
  ct_graph_free(graph);
  ct_game_tags_free(game_tags);
}

START_TEST(ut_graph_from_pgn)
{
  char *pgn = "[Event \"Russia vs The Rest of the World\"]\n"
  "[Site \"Moscow RUS\"]\n"
  "[Date \"2002.09.09\"]\n"
  "[Round \"5\"]\n"
  "[White \"Judit Polgar\"]\n"
  "[Black \"Garry Kasparov\"]\n"
  "[Result \"1-0\"]\n"
  "[WhiteElo \"2681\"]\n"
  "[BlackElo \"2838\"]\n"
  "\n"
  "1. e4 e5 2. Nf3 Nc6 3. Bb5 Nf6 4. O-O Nxe4 5. d4 Nd6 6. Bxc6 dxc6 7. dxe5 Nf5\n"
  "8. Qxd8+ Kxd8 9. Nc3 h6 10. Rd1+ Ke8 11. h3 Be7 12. Ne2 Nh4 13. Nxh4 Bxh4 14.\n"
  "Be3 Bf5 15. Nd4 Bh7 16. g4 Be7 17. Kg2 h5 18. Nf5 Bf8 19. Kf3 Bg6 20. Rd2 hxg4+\n"
  "21. hxg4 Rh3+ 22. Kg2 Rh7 23. Kg3 f6 24. Bf4 Bxf5 25. gxf5 fxe5 26. Re1 Bd6 27.\n"
  "Bxe5 Kd7 28. c4 c5 29. Bxd6 cxd6 30. Re6 Rah8 31. Rexd6+ Kc8 32. R2d5 Rh3+ 33.\n"
  "Kg2 Rh2+ 34. Kf3 R2h3+ 35. Ke4 b6 36. Rc6+ Kb8 37. Rd7 Rh2 38. Ke3 Rf8 39. Rcc7\n"
  "Rxf5 40. Rb7+ Kc8 41. Rdc7+ Kd8 42. Rxg7 Kc8 1-0\n";
  char *verify_pgn;
  CtGraph result;

  result = ct_graph_from_pgn(0, game_tags, pgn, error_message);
  ck_assert(result != 0);
  ck_assert(error_message[0] == 0);
  verify_pgn = ct_graph_to_new_pgn(result, game_tags);
  ck_assert_str_eq(verify_pgn, pgn);
  free(verify_pgn);
  ct_game_tags_reset(game_tags);

  result = ct_graph_from_pgn(graph, game_tags, pgn, error_message);
  ck_assert(result == graph);
  ck_assert(error_message[0] == 0);
  verify_pgn = ct_graph_to_new_pgn(graph, game_tags);
  ck_assert_str_eq(verify_pgn, pgn);
  free(verify_pgn);

  result = ct_graph_from_pgn(0, 0, "1. d4", 0);
  ck_assert(result != 0);
  verify_pgn = ct_graph_to_new_pgn(result, 0);
  ck_assert_str_eq(verify_pgn, "1. d4 *\n");
  free(verify_pgn);
} END_TEST

START_TEST(ut_graph_from_pgn_short)
{
  char *expected_result = "[Event \"?\"]\n"
  "[Site \"?\"]\n"
  "[Date \"?\"]\n"
  "[Round \"?\"]\n"
  "[White \"?\"]\n"
  "[Black \"?\"]\n"
  "[Result \"*\"]\n"
  "\n"
  "*\n";
  char *verify_pgn;
  CtGraph result;

  result = ct_graph_from_pgn(graph, game_tags, "", error_message);
  ck_assert(result == graph);
  verify_pgn = ct_graph_to_new_pgn(graph, game_tags);
  ck_assert_str_eq(verify_pgn, expected_result);
  free(verify_pgn);

  result = ct_graph_from_pgn(graph, game_tags, "*", error_message);
  ck_assert(result == graph);
  verify_pgn = ct_graph_to_new_pgn(graph, game_tags);
  ck_assert_str_eq(verify_pgn, expected_result);
  free(verify_pgn);

  result = ct_graph_from_pgn(graph, game_tags, "1. *", error_message);
  ck_assert(result == graph);
  verify_pgn = ct_graph_to_new_pgn(graph, game_tags);
  ck_assert_str_eq(verify_pgn, expected_result);
  free(verify_pgn);
} END_TEST

START_TEST(ut_graph_from_pgn_error)
{
  CtGraph result;

  result = ct_graph_from_pgn(graph, game_tags, "foo", error_message);
  ck_assert(result == 0);
  ck_assert_str_eq(error_message, "syntax error on line 1 column 1");
  result = ct_graph_from_pgn(graph, game_tags, "[", error_message);
  ck_assert(result == 0);
  ck_assert_str_eq(error_message, "syntax error on line 1 column 1");
  result = ct_graph_from_pgn(graph, game_tags, "1. e4 e5\nbar", error_message);
  ck_assert(result == 0);
  ck_assert_str_eq(error_message, "syntax error on line 2 column 1");
  result = ct_graph_from_pgn(graph, game_tags, "1. e4 e5\n2. baz", error_message);
  ck_assert(result == 0);
  ck_assert_str_eq(error_message, "syntax error on line 2 column 4");
  result = ct_graph_from_pgn(graph, game_tags, "1. e4 e5\n2. Rd4", error_message);        /* illegal move */
  ck_assert(result == 0);
  ck_assert_str_eq(error_message, "syntax error on line 2 column 4");
} END_TEST

START_TEST(ut_graph_from_pgn_parse_file)
{
  int found_it = 0;
  CtCommand command = ct_command_new(&found_it, ut_graph_from_pgn_check_carlsen_round_10);
  FILE *file = fopen("candidates2013.pgn", "r");

  ck_assert(file != 0);
  ct_graph_from_pgn_file(graph, game_tags, file, command, error_message);
  ck_assert_int_eq(found_it, 1);

  fclose(file);
  ct_command_free(command);
} END_TEST

static void
ut_graph_from_pgn_check_carlsen_round_10(void *delegate)
{
  static char *find_round = "10";
  static char *find_white = "Carlsen, Magnus";
  static char *expected_result = "[Event \"FIDE Candidates\"]\n"
  "[Site \"London\"]\n"
  "[Date \"2013.03.27\"]\n"
  "[Round \"10\"]\n"
  "[White \"Carlsen, Magnus\"]\n"
  "[Black \"Gelfand, Boris\"]\n"
  "[Result \"1-0\"]\n"
  "[WhiteElo \"2872\"]\n"
  "[BlackElo \"2740\"]\n"
  "[ECO \"B30\"]\n"
  "\n"
  "1. e4 c5 2. Nf3 Nc6 3. Bb5 e6 4. O-O Nge7 5. Re1 a6 6. Bf1 d5 7. exd5 Nxd5 8.\n"
  "d4 Nf6 9. Be3 cxd4 10. Nxd4 Bd7 11. c4 Nxd4 12. Bxd4 Bc6 13. Nc3 Be7 14. a3 a5\n"
  "15. Qd3 O-O 16. Rad1 Qc7 17. Be5 Qb6 18. Qg3 Rfd8 19. Rxd8+ Qxd8 20. Rd1 Qb6\n"
  "21. Bd4 Qb3 22. Rd3 Qc2 23. b4 axb4 24. axb4 Nh5 25. Qe5 Bf6 26. Qxh5 Bxd4 27.\n"
  "Rxd4 Qxc3 28. Qa5 Rf8 29. Qb6 e5 30. Rd1 g6 31. b5 Be4 32. Qf6 h5 33. h4 Bf5\n"
  "34. Rd5 Qc1 35. Qxe5 Be6 36. Rd4 Ra8 37. Qe2 Kh7 38. Rd1 Qc3 39. Qe4 Ra1 40.\n"
  "Rxa1 Qxa1 41. c5 Qc3 42. Qxb7 Qe1 43. b6 Bc4 44. Qf3 Qxf1+ 45. Kh2 Qb1 46. b7\n"
  "Qb5 47. c6 Bd5 48. Qg3 1-0\n";
  char *tag_round = ct_game_tags_get(game_tags, "Round");
  char *tag_white = ct_game_tags_get(game_tags, "White");
  char *verify_pgn;

  ck_assert(error_message[0] == 0);
  if (strcmp(tag_round, find_round) == 0 && strcmp(tag_white, find_white) == 0)
  {
    int *found_it = (int *) delegate;

    verify_pgn = ct_graph_to_new_pgn(graph, game_tags);
    ck_assert_str_eq(verify_pgn, expected_result);
    free(verify_pgn);
    *found_it += 1;
  }
}

Suite *
ut_graph_from_pgn_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_graph_from_pgn");
  test_case = tcase_create("PgnReader");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_graph_from_pgn);
  tcase_add_test(test_case, ut_graph_from_pgn_short);
  tcase_add_test(test_case, ut_graph_from_pgn_error);
  tcase_add_test(test_case, ut_graph_from_pgn_parse_file);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
