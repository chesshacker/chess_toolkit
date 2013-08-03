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
#include "ct_debug_utilities.h"
#include "check_utilities.h"
#include <unistd.h>
#include <stdio.h>

START_TEST(ut_position_print)
{
  CtPosition position = ct_position_from_fen(0, "rn2k2r/p3R1p1/b1p5/5pBp/1p5P/1BN5/PPP2PP1/3R2K1 b kq -");
  char result[CT_POSITION_TO_S_MAX_LENGTH];
  char *expected_result =
  " 8  r n - - k - - r\n"
  " 7  p - - - R - p -\n"
  " 6  b - p - - - - -\n"
  " 5  - - - - - p B p\n"
  " 4  - p - - - - - P\n"
  " 3  - B N - - - - -\n"
  " 2  P P P - - P P -\n"
  " 1  - - - R - - K -\n"
  "    a b c d e f g h\n"
  "Black kq -\n";
  int new_fd = check_redirect_stdout();

  ct_position_print(position);
  ck_assert(check_read_exactly(new_fd, result, strlen(expected_result)));
  check_restore_stdout(new_fd);
  ck_assert_str_eq(result, expected_result);
} END_TEST

START_TEST(ut_move_print)
{
  CtMove move = ct_move_make(E2, E4);
  char result[CT_MOVE_TO_S_MAX_LENGTH];
  char *expected_result = "e2e4\n";
  int new_fd = check_redirect_stdout();

  ct_move_print(move);
  ck_assert(check_read_exactly(new_fd, result, strlen(expected_result)));
  check_restore_stdout(new_fd);
  ck_assert_str_eq(result, expected_result);
} END_TEST

START_TEST(ut_graph_print)
{
  CtGraph graph = ct_graph_from_fen(0, "rn2k2r/p3R1p1/b1p5/5pBp/1p5P/1BN5/PPP2PP1/3R2K1 b kq -");
  char result[CT_POSITION_TO_S_MAX_LENGTH];
  char *expected_result =
  " 8  r n - - - k - r\n"
  " 7  p - - - R - p -\n"
  " 6  b - p - - - - -\n"
  " 5  - - - - - p B p\n"
  " 4  - p - - - - - P\n"
  " 3  - B N - - - - -\n"
  " 2  P P P - - P P -\n"
  " 1  - - - R - - K -\n"
  "    a b c d e f g h\n"
  "White - -\n"
  "Ply 1\n";
  int new_fd = check_redirect_stdout();

  ct_graph_make_move(graph, ct_move_make(E8, F8));
  ct_graph_print(graph);
  ck_assert(check_read_exactly(new_fd, result, strlen(expected_result)));
  check_restore_stdout(new_fd);
  ck_assert_str_eq(result, expected_result);
} END_TEST

START_TEST(ut_bit_board_print)
{
  CtBitBoard bit_board = ct_bit_board_make(63) | ct_bit_board_make(31) | ct_bit_board_make(2);
  char result[CT_POSITION_TO_S_MAX_LENGTH];
  char *expected_result =
  "0x8000000080000004\n"
  " 8  . . . . . . . x\n"
  " 7  . . . . . . . .\n"
  " 6  . . . . . . . .\n"
  " 5  . . . . . . . .\n"
  " 4  . . . . . . . x\n"
  " 3  . . . . . . . .\n"
  " 2  . . . . . . . .\n"
  " 1  . . x . . . . .\n"
  "    a b c d e f g h\n";
  int new_fd = check_redirect_stdout();

  ct_bit_board_print(bit_board);
  ck_assert(check_read_exactly(new_fd, result, strlen(expected_result)));
  check_restore_stdout(new_fd);
  ck_assert_str_eq(result, expected_result);
} END_TEST

Suite *
ut_debug_utilities_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_debug_utilities");
  test_case = tcase_create("DebugUtilities");
  tcase_add_test(test_case, ut_position_print);
  tcase_add_test(test_case, ut_move_print);
  tcase_add_test(test_case, ut_graph_print);
  tcase_add_test(test_case, ut_bit_board_print);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
