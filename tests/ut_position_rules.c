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

typedef struct IsLegalTestStruct *IsLegalTest;
typedef struct IsLegalTestStruct
{
  bool end_of_tests;
  char fen[CT_FEN_MAX_LENGTH];
  bool answer;
} IsLegalTestStruct;

/* is_legal and is_check use the same test structure */
typedef struct IsCheckTestStruct *IsCheckTest;
typedef struct IsCheckTestStruct
{
  bool end_of_tests;
  char fen[CT_FEN_MAX_LENGTH];
  bool answer;
} IsCheckTestStruct;

typedef struct CanCastleTestStruct *CanCastleTest;
typedef struct CanCastleTestStruct
{
  bool end_of_tests;
  char fen[CT_FEN_MAX_LENGTH];
  CtCastleRights answer;
} CanCastleTestStruct;

static CtPosition position;

static void setup(void);
static void teardown(void);

static void
setup(void)
{
  position = ct_position_new();
}

static void
teardown(void)
{
  ct_position_free(position);
}

START_TEST(ut_position_is_position_legal)
{
  IsLegalTestStruct tests[] =
  {
    {false, "8/8/8/8/8/8/8/8 w - -", true},
    {false, "r7/8/8/8/8/8/8/K7 b - -", false},        /* white king in check, but black's turn */
    {false, "r7/8/8/8/8/8/8/K7 w - -", true},        /* would be legal if it was white's turn */
    {false, "r7/8/8/8/8/8/8/1K6 b - -", true},        /* or if the king wasn't in check (moved king) */
    {false, "k7/8/8/8/8/8/8/R7 w - -", false},        /* black king in check, but white's turn */
    {false, "k7/8/8/8/8/8/8/R7 b - -", true},        /* would be legal if it was black's turn */
    {false, "k7/p7/8/8/8/8/8/R7 b - -", true},        /* or if the king wasn't in check (blocked) */
    {true, "", 0}
  };
  IsLegalTest test;

  for (test = tests; !test->end_of_tests; test++)
  {
    ck_assert_msg(ct_position_from_fen(position, test->fen) != 0, test->fen);
    ck_assert_msg(ct_position_is_legal(position) == test->answer, test->fen);
  }
} END_TEST

START_TEST(ut_position_is_check)
{
  IsCheckTestStruct tests[] =
  {
    {false, "8/8/8/8/8/8/8/8 w - -", false},
    {false, "rk6/8/8/8/8/8/8/K7 w - -", true},
    {false, "rk6/8/8/8/8/8/8/1K6 w - -", false},
    {false, "k7/8/8/8/8/8/8/RK6 b - -", true},
    {false, "1k6/8/8/8/8/8/8/RK6 b - -", false},
    {true, "", 0}
  };
  IsCheckTest test;

  for (test = tests; !test->end_of_tests; test++)
  {
    ck_assert_msg(ct_position_from_fen(position, test->fen) != 0, test->fen);
    ck_assert_msg(ct_position_is_check(position) == test->answer, test->fen);
  }
} END_TEST

START_TEST(ut_position_can_castle)
{
  CanCastleTestStruct tests[] =
  {
    {false, "8/8/8/8/8/8/8/8 w - -", CASTLE_NONE},
    {false, "r3k2r/8/8/8/8/8/8/R3K2R w KQkq -", CASTLE_KQ},
    {false, "r3k2r/8/8/8/8/8/8/R3K2R b KQkq -", CASTLE_KQ},
    /* can't castle if you have lost the right to (for moving a rook or king previously) */
    {false, "r3k2r/8/8/8/8/8/8/R3K2R w Qkq -", CASTLE_Q},
    {false, "r3k2r/8/8/8/8/8/8/R3K2R b KQk -", CASTLE_K},
    {false, "r3k2r/8/8/8/8/8/8/R3K2R w kq -", CASTLE_NONE},
    /* can't castle if there are pieces in the way */
    {false, "r3k2r/8/8/8/8/8/8/R2QK2R w KQkq -", CASTLE_K},
    {false, "r3k2r/8/8/8/8/8/8/R1Q1K2R w KQkq -", CASTLE_K},
    {false, "r3k2r/8/8/8/8/8/8/RQ2K2R w KQkq -", CASTLE_K},
    {false, "r3k1nr/8/8/8/8/8/8/R3K2R b KQkq -", CASTLE_Q},
    {false, "r3kb1r/8/8/8/8/8/8/R3K2R b KQkq -", CASTLE_Q},
    /* whether the pieces are your or not */
    {false, "r2NkB1r/8/8/8/8/8/8/R3K2R b KQkq -", CASTLE_NONE},
    /* can't castle if you are in check */
    {false, "r3k2r/8/8/8/4r3/8/8/R3K2R w KQkq -", CASTLE_NONE},
    {false, "r3k2r/8/8/4R3/8/8/8/R3K2R b KQkq -", CASTLE_NONE},
    /* can't castle through check */
    {false, "r3k2r/8/8/8/3r4/8/8/R3K2R w KQkq -", CASTLE_K},
    {false, "r3k2r/8/8/5R2/8/8/8/R3K2R b KQkq -", CASTLE_Q},
    /* can't castle into check */
    {false, "r3k2r/8/8/8/6r1/8/8/R3K2R w KQkq -", CASTLE_Q},
    {false, "r3k2r/8/8/2R5/8/8/8/R3K2R b KQkq -", CASTLE_K},
    /* attacks on any of the other squares on the 1st (or 8th) rank don't prevent castling */
    {false, "r3k2r/8/8/8/rr5r/8/8/R3K2R w KQkq -", CASTLE_KQ},
    {false, "r3k2r/8/8/RR5R/8/8/8/R3K2R b KQkq -", CASTLE_KQ},
    {true, "", 0}
  };
  CanCastleTest test;

  for (test = tests; !test->end_of_tests; test++)
  {
    ck_assert_msg(ct_position_from_fen(position, test->fen) != 0, test->fen);
    ck_assert_msg(ct_position_can_castle(position) == test->answer, test->fen);
  }
} END_TEST

Suite *
ut_position_rules_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_position_rules");
  test_case = tcase_create("Position Rules");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_position_is_position_legal);
  tcase_add_test(test_case, ut_position_is_check);
  tcase_add_test(test_case, ut_position_can_castle);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
