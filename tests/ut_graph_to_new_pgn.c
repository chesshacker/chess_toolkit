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

static void setup(void);
static void teardown(void);
static void ut_graph_to_new_pgn_load_tags(char **tags_array);
static void ut_graph_to_new_pgn_load_moves(char **moves_array);

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

START_TEST(ut_graph_to_new_pgn)
{
  char *tags[] = {
    "Event", "Hungarian Super Championship",
    "Site", "Budapest HUN",
    "Date", "1991.12.??",
    "Round", "9",
    "White", "Tolnai, Tibor",
    "Black", "Polgar, Judit",
    "Result", "0-1",
    "WhiteElo", "2480",
    "BlackElo", "2550",
    0
  };
  char *moves[] = {
    "e4", "c5", "Nf3", "e6", "d4", "cxd4", "Nxd4", "a6", "Nc3", "Qc7", "f4", "b5", "Bd3",
    "Bb7", "Qf3", "Nf6", "Be3", "Nc6", "O-O-O", "b4", "Nce2", "Na5", "g4", "d5", "e5",
    "Nd7", "Kb1", "Nc4", "Bc1", "O-O-O", "h4", "Nc5", "b3", "Na3+", "Ka1", "f6", "c3",
    "fxe5", "fxe5", "Nc4", "Nxe6", "Nxe5", "Qg3", "Nxe6", "Bf5", "Kb8", "Bxe6",
    "bxc3", "Nxc3", "d4", "Rhf1", "Bb4", "Na4", "Rhe8", "Bf5", "Bc6", "Bb2", "g6",
    "Bb1", "Bxa4", "bxa4", "Bc3", "Bxc3", "Qxc3+", "Qxc3", "dxc3", "Rc1", "Rc8",
    "Rf4", "Rc5", "Rb4+", "Ka7", "Rb3", "Rec8", "Be4", "R8c7", "Rcb1", "Nc6", "Bxc6",
    "R5xc6", "Rb4", "Rc4", "a3", "Rxb4", "axb4", "Rc4", "h5", "a5", "hxg6", "hxg6",
    "Ka2", "Rxb4", "Rg1", "c2", "g5", "Kb6",
    0
  };
  char *result;
  char *expected_result = "[Event \"Hungarian Super Championship\"]\n"
  "[Site \"Budapest HUN\"]\n"
  "[Date \"1991.12.??\"]\n"
  "[Round \"9\"]\n"
  "[White \"Tolnai, Tibor\"]\n"
  "[Black \"Polgar, Judit\"]\n"
  "[Result \"0-1\"]\n"
  "[WhiteElo \"2480\"]\n"
  "[BlackElo \"2550\"]\n"
  "\n"
  "1. e4 c5 2. Nf3 e6 3. d4 cxd4 4. Nxd4 a6 5. Nc3 Qc7 6. f4 b5 7. Bd3 Bb7 8. Qf3\n"
  "Nf6 9. Be3 Nc6 10. O-O-O b4 11. Nce2 Na5 12. g4 d5 13. e5 Nd7 14. Kb1 Nc4 15.\n"
  "Bc1 O-O-O 16. h4 Nc5 17. b3 Na3+ 18. Ka1 f6 19. c3 fxe5 20. fxe5 Nc4 21. Nxe6\n"
  "Nxe5 22. Qg3 Nxe6 23. Bf5 Kb8 24. Bxe6 bxc3 25. Nxc3 d4 26. Rhf1 Bb4 27. Na4\n"
  "Rhe8 28. Bf5 Bc6 29. Bb2 g6 30. Bb1 Bxa4 31. bxa4 Bc3 32. Bxc3 Qxc3+ 33. Qxc3\n"
  "dxc3 34. Rc1 Rc8 35. Rf4 Rc5 36. Rb4+ Ka7 37. Rb3 Rec8 38. Be4 R8c7 39. Rcb1\n"
  "Nc6 40. Bxc6 R5xc6 41. Rb4 Rc4 42. a3 Rxb4 43. axb4 Rc4 44. h5 a5 45. hxg6 hxg6\n"
  "46. Ka2 Rxb4 47. Rg1 c2 48. g5 Kb6 0-1\n";

  ut_graph_to_new_pgn_load_tags(tags);
  ut_graph_to_new_pgn_load_moves(moves);

  result = ct_graph_to_new_pgn(graph, game_tags);
  ck_assert_str_eq(result, expected_result);
  free(result);
} END_TEST

START_TEST(ut_graph_to_new_pgn_escaped_characters)
{
  char *tags[] = {
    "Event", "The \"Quotes\" Have Been Tested",
    "Site", "The \\Slashes\\ Have Also Been Tested",
    0
  };
  char *result;
  char *expected_result = "[Event \"The \\\"Quotes\\\" Have Been Tested\"]\n"
  "[Site \"The \\\\Slashes\\\\ Have Also Been Tested\"]\n"
  "[Date \"?\"]\n"
  "[Round \"?\"]\n"
  "[White \"?\"]\n"
  "[Black \"?\"]\n"
  "[Result \"*\"]\n"
  "\n"
  "*\n";

  ut_graph_to_new_pgn_load_tags(tags);
  result = ct_graph_to_new_pgn(graph, game_tags);
  ck_assert_str_eq(result, expected_result);
  free(result);
} END_TEST

START_TEST(ut_graph_to_new_pgn_without_tags)
{
  char *result;
  char *moves[] = {"e4", "c5", "d4", "cxd4", "c3", "dxc3", "Nxc3", 0};
  char *expected_result = "1. e4 c5 2. d4 cxd4 3. c3 dxc3 4. Nxc3 *\n";

  ut_graph_to_new_pgn_load_moves(moves);
  result = ct_graph_to_new_pgn(graph, 0);
  ck_assert_str_eq(result, expected_result);
  free(result);
} END_TEST

START_TEST(ut_graph_to_new_pgn_without_graph)
{
  char *result;
  char *tags[] = {
    "Event", "Hungarian Super Championship",
    "Site", "Budapest HUN",
    "Date", "1991.12.??",
    "Round", "9",
    "White", "Tolnai, Tibor",
    "Black", "Polgar, Judit",
    "Result", "0-1",
    "WhiteElo", "2480",
    "BlackElo", "2550",
    0
  };
  char *expected_result = "[Event \"Hungarian Super Championship\"]\n"
  "[Site \"Budapest HUN\"]\n"
  "[Date \"1991.12.??\"]\n"
  "[Round \"9\"]\n"
  "[White \"Tolnai, Tibor\"]\n"
  "[Black \"Polgar, Judit\"]\n"
  "[Result \"0-1\"]\n"
  "[WhiteElo \"2480\"]\n"
  "[BlackElo \"2550\"]\n";

  ut_graph_to_new_pgn_load_tags(tags);
  result = ct_graph_to_new_pgn(0, game_tags);
  ck_assert_str_eq(result, expected_result);
  free(result);
} END_TEST

static void
ut_graph_to_new_pgn_load_tags(char **tags_array)
{
  char **tags_ptr;
  char *tag_key;
  char *tag_value;

  for (tags_ptr = tags_array; *tags_ptr != 0; tags_ptr += 2)
  {
    tag_key = tags_ptr[0];
    tag_value = tags_ptr[1];
    ct_game_tags_set(game_tags, tag_key, tag_value);
  }
}

static void
ut_graph_to_new_pgn_load_moves(char **moves_array)
{
  char **moves_ptr;
  CtMove move;

  for (moves_ptr = moves_array; *moves_ptr != 0; moves_ptr++)
  {
    move = ct_graph_move_from_san(graph, *moves_ptr);
    ck_assert_msg(move != NULL_MOVE, *moves_ptr);
    ct_graph_make_move(graph, move);
  }
}

START_TEST(ut_graph_to_new_pgn_realloc_required)
{
  char *expected_pgn = "[Event \"FIDE Candidates\"]\n"
  "[Site \"London\"]\n"
  "[Date \"2013.03.25\"]\n"
  "[Round \"9\"]\n"
  "[White \"Ivanchuk, Vassily\"]\n"
  "[Black \"Radjabov, Teimour\"]\n"
  "[Result \"1-0\"]\n"
  "[WhiteElo \"2757\"]\n"
  "[BlackElo \"2793\"]\n"
  "\n"
  "1. d4 Nf6 2. c4 e6 3. Nf3 d5 4. Nc3 Be7 5. Bg5 h6 6. Bh4 Ne4 7. Bxe7 Qxe7 8.\n"
  "cxd5 Nxc3 9. bxc3 exd5 10. Qb3 c6 11. e3 O-O 12. Be2 Nd7 13. O-O Nf6 14. Rfc1\n"
  "Bg4 15. c4 dxc4 16. Qxc4 Rfe8 17. h3 Be6 18. Qc5 Qc7 19. Rc2 Bd5 20. Rac1 Qd8\n"
  "21. Qa3 Ne4 22. Ne5 Nd6 23. Bf3 Bxf3 24. Nxf3 Qd7 25. Qb4 Rad8 26. a4 Qe6 27.\n"
  "Ne5 f6 28. Nd3 Qf7 29. Qa5 a6 30. Nc5 Rc8 31. Qb4 Rcd8 32. Qb6 Rc8 33. Rb1 Re7\n"
  "34. Nd3 Rce8 35. Qb3 Qxb3 36. Rxb3 Kf7 37. g4 g5 38. Nc5 Rc8 39. Kg2 Rcc7 40.\n"
  "Nd3 Kg6 41. Nc5 h5 42. Rb1 Ne4 43. Nxe4 Rxe4 44. Kf3 Ree7 45. e4 Rcd7 46. Ke3\n"
  "Rd5 47. f3 Rd6 48. Rb6 Red7 49. Rd2 hxg4 50. hxg4 Kf7 51. a5 Ke7 52. Rb1 Kf7\n"
  "53. Rh1 Kg7 54. Rh5 Rd8 55. Rd1 R8d7 56. e5 Rd5 57. Rdh1 Kf7 58. Rh7+ Ke6 59.\n"
  "Rxd7 Kxd7 60. Rh7+ Ke6 61. Rh6 c5 62. Rxf6+ Ke7 63. Rb6 Rxd4 64. Rxb7+ Ke6 65.\n"
  "Rg7 Ra4 66. Rxg5 Rxa5 67. f4 Ra3+ 68. Kd2 Ra2+ 69. Kc3 Rf2 70. Rg6+ Kd5 71.\n"
  "Rd6+ Ke4 72. Rf6 a5 73. e6 Kd5 74. f5 Re2 75. Rf8 Kd6 76. f6 1-0\n";
  char *pgn;

  ct_graph_from_pgn(graph, game_tags, expected_pgn, 0);
  pgn = ct_graph_to_new_pgn(graph, game_tags);
  ck_assert_str_eq(pgn, expected_pgn);
  free(pgn);
} END_TEST

Suite *
ut_graph_to_new_pgn_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_graph_to_new_pgn");
  test_case = tcase_create("Graph.to_new_pgn");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_graph_to_new_pgn);
  tcase_add_test(test_case, ut_graph_to_new_pgn_escaped_characters);
  tcase_add_test(test_case, ut_graph_to_new_pgn_without_tags);
  tcase_add_test(test_case, ut_graph_to_new_pgn_without_graph);
  tcase_add_test(test_case, ut_graph_to_new_pgn_realloc_required);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
