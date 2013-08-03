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

typedef struct UtBitBoardToSTestStruct *UtBitBoardToSTest;
typedef struct UtBitBoardToSTestStruct
{
  bool end_of_tests;
  CtBitBoard bit_board;
  char *expected_result;
} UtBitBoardToSTestStruct;

START_TEST(ut_bit_board_to_s)
{
  UtBitBoardToSTestStruct tests[] =
  {
    {
      false, 0xFFFF00000000FFFF,
      "0xFFFF00000000FFFF\n"
      " 8  x x x x x x x x\n"
      " 7  x x x x x x x x\n"
      " 6  . . . . . . . .\n"
      " 5  . . . . . . . .\n"
      " 4  . . . . . . . .\n"
      " 3  . . . . . . . .\n"
      " 2  x x x x x x x x\n"
      " 1  x x x x x x x x\n"
      "    a b c d e f g h"
    },
    {
      false, 0x0,
      "0x0000000000000000\n"
      " 8  . . . . . . . .\n"
      " 7  . . . . . . . .\n"
      " 6  . . . . . . . .\n"
      " 5  . . . . . . . .\n"
      " 4  . . . . . . . .\n"
      " 3  . . . . . . . .\n"
      " 2  . . . . . . . .\n"
      " 1  . . . . . . . .\n"
      "    a b c d e f g h"
    },
    {
      false, (ct_bit_board_make(63) | ct_bit_board_make(31) | ct_bit_board_make(2)),
      "0x8000000080000004\n"
      " 8  . . . . . . . x\n"
      " 7  . . . . . . . .\n"
      " 6  . . . . . . . .\n"
      " 5  . . . . . . . .\n"
      " 4  . . . . . . . x\n"
      " 3  . . . . . . . .\n"
      " 2  . . . . . . . .\n"
      " 1  . . x . . . . .\n"
      "    a b c d e f g h"
    },
    {true, 0, 0}
  };
  CtBitBoard bit_board;
  UtBitBoardToSTest test;
  char destination[CT_BIT_BOARD_TO_S_MAX_LENGTH];
  char *result;

  for (test = tests; !test->end_of_tests; test++)
  {
    bit_board = test->bit_board;
    result = ct_bit_board_to_s(bit_board, 0);
    ck_assert_str_eq(result, test->expected_result);
    result = ct_bit_board_to_s(bit_board, destination);
    ck_assert_str_eq(destination, test->expected_result);
    ck_assert(destination == result);
  }
} END_TEST

Suite *
ut_bit_board_to_s_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_bit_board_to_s");
  test_case = tcase_create("BitBoard.to_s");
  tcase_add_test(test_case, ut_bit_board_to_s);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
