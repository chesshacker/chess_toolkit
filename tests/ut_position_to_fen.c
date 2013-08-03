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

START_TEST(ut_position_to_fen)
{
  CtPosition position;
  char *result;
  char destination[CT_FEN_MAX_LENGTH];
  char *tests[] =
  {
    "8/8/8/8/8/8/8/8 w - -",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -",
    "rnbqkbnr/ppppppp1/8/P7/6Pp/8/1PPPPP1P/RNBQKBNR b KQkq g3",
    "rnbqkbnr/p1ppppp1/8/Pp5p/8/8/1PPPPPPP/RNBQKBNR w KQkq b6",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -",
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -",
    "rnbqkb1r/pp1p1ppp/2p5/4P3/2B5/8/PPP1NnPP/RNBQK2R w KQkq -",
    0
  };
  char **test;

  for (test = tests; *test != 0; test++)
  {
    position = ct_position_from_fen(0, *test);
    ck_assert_msg(position != 0, *test);
    result = ct_position_to_fen(position, 0);
    ck_assert_str_eq(result, *test);
    result = ct_position_to_fen(position, destination);
    ck_assert_str_eq(destination, *test);
    ck_assert(destination == result);
  }
} END_TEST

Suite *
ut_position_to_fen_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_position_to_fen");
  test_case = tcase_create("Position.to_fen");
  tcase_add_test(test_case, ut_position_to_fen);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
