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

typedef struct UtPositionToSTestStruct *UtPositionToSTest;
typedef struct UtPositionToSTestStruct
{
  bool end_of_tests;
  char *fen;
  char *expected_result;
} UtPositionToSTestStruct;

START_TEST(ut_position_to_s)
{
  UtPositionToSTestStruct tests[] =
  {
    {
      false, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -",
      " 8  r n b q k b n r\n"
      " 7  p p p p p p p p\n"
      " 6  - - - - - - - -\n"
      " 5  - - - - - - - -\n"
      " 4  - - - - - - - -\n"
      " 3  - - - - - - - -\n"
      " 2  P P P P P P P P\n"
      " 1  R N B Q K B N R\n"
      "    a b c d e f g h\n"
      "White KQkq -"
    },
    {
      false, "8/8/8/8/8/8/8/8 w - -",
      " 8  - - - - - - - -\n"
      " 7  - - - - - - - -\n"
      " 6  - - - - - - - -\n"
      " 5  - - - - - - - -\n"
      " 4  - - - - - - - -\n"
      " 3  - - - - - - - -\n"
      " 2  - - - - - - - -\n"
      " 1  - - - - - - - -\n"
      "    a b c d e f g h\n"
      "White - -"
    },
    {
      false, "rn2k2r/p3R1p1/b1p5/5pBp/1p5P/1BN5/PPP2PP1/3R2K1 b kq -",
      " 8  r n - - k - - r\n"
      " 7  p - - - R - p -\n"
      " 6  b - p - - - - -\n"
      " 5  - - - - - p B p\n"
      " 4  - p - - - - - P\n"
      " 3  - B N - - - - -\n"
      " 2  P P P - - P P -\n"
      " 1  - - - R - - K -\n"
      "    a b c d e f g h\n"
      "Black kq -"
    },
    {
      false, "rnbqkbnr/ppppppp1/8/P7/6Pp/8/1PPPPP1P/RNBQKBNR b KQkq g3",
      " 8  r n b q k b n r\n"
      " 7  p p p p p p p -\n"
      " 6  - - - - - - - -\n"
      " 5  P - - - - - - -\n"
      " 4  - - - - - - P p\n"
      " 3  - - - - - - - -\n"
      " 2  - P P P P P - P\n"
      " 1  R N B Q K B N R\n"
      "    a b c d e f g h\n"
      "Black KQkq g3"
    },
    {true, 0, 0}
  };
  CtPosition position;
  UtPositionToSTest test;
  char destination[CT_POSITION_TO_S_MAX_LENGTH];
  char *result;

  for (test = tests; !test->end_of_tests; test++)
  {
    position = ct_position_from_fen(0, test->fen);
    ck_assert_msg(position != 0, test->fen);
    result = ct_position_to_s(position, 0);
    ck_assert_str_eq(result, test->expected_result);
    result = ct_position_to_s(position, destination);
    ck_assert_str_eq(destination, test->expected_result);
    ck_assert(destination == result);
  }
} END_TEST

Suite *
ut_position_to_s_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_position_to_s");
  test_case = tcase_create("Position.to_s");
  tcase_add_test(test_case, ut_position_to_s);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
