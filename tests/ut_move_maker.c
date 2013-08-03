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
#include "ct_move_maker.h"        /* not part of the advertised API, used by CtGraph */

static CtPosition position;
static CtMoveMaker move_maker;

static void setup(void);
static void teardown(void);

static void
setup(void)
{
  position = ct_position_new();
  move_maker = ct_move_maker_new(position);
}

static void
teardown(void)
{
  ct_move_maker_free(move_maker);
  ct_position_free(position);
}

START_TEST(ut_move_maker_move)
{
  /* this is a short contrived game that has captures, en-passant, promotion and castling... */
  /* 1.e4 f5 2.exf5 g5 3.fxg6 Bg7 4.gxh7 Nc6 5.hxg8=R+ Rxg8 6.Bc4 d5 7.Nf3 Rb8 8.O-O * */
  char *fens[] =
  {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -",        /* initial position */
    "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -",        /* 1.e4 */
    "rnbqkbnr/ppppp1pp/8/5p2/4P3/8/PPPP1PPP/RNBQKBNR w KQkq -",        /* f5 */
    "rnbqkbnr/ppppp1pp/8/5P2/8/8/PPPP1PPP/RNBQKBNR b KQkq -",        /* 2.exf5 */
    "rnbqkbnr/ppppp2p/8/5Pp1/8/8/PPPP1PPP/RNBQKBNR w KQkq g6",        /* g5 */
    "rnbqkbnr/ppppp2p/6P1/8/8/8/PPPP1PPP/RNBQKBNR b KQkq -",        /* 3.fxg6 */
    "rnbqk1nr/ppppp1bp/6P1/8/8/8/PPPP1PPP/RNBQKBNR w KQkq -",        /* Bg7 */
    "rnbqk1nr/ppppp1bP/8/8/8/8/PPPP1PPP/RNBQKBNR b KQkq -",        /* 4.gxh7 */
    "r1bqk1nr/ppppp1bP/2n5/8/8/8/PPPP1PPP/RNBQKBNR w KQkq -",        /* Nc6 */
    "r1bqk1Rr/ppppp1b1/2n5/8/8/8/PPPP1PPP/RNBQKBNR b KQkq -",        /* 5.hxg8=R+ */
    "r1bqk1r1/ppppp1b1/2n5/8/8/8/PPPP1PPP/RNBQKBNR w KQq -",        /* Rxg8 */
    "r1bqk1r1/ppppp1b1/2n5/8/2B5/8/PPPP1PPP/RNBQK1NR b KQq -",        /* 6.Bc4 */
    "r1bqk1r1/ppp1p1b1/2n5/3p4/2B5/8/PPPP1PPP/RNBQK1NR w KQq -",        /* d5 */
    "r1bqk1r1/ppp1p1b1/2n5/3p4/2B5/5N2/PPPP1PPP/RNBQK2R b KQq -",        /* 7.Nf3 */
    "1rbqk1r1/ppp1p1b1/2n5/3p4/2B5/5N2/PPPP1PPP/RNBQK2R w KQ -",        /* Rb8 */
    "1rbqk1r1/ppp1p1b1/2n5/3p4/2B5/5N2/PPPP1PPP/RNBQ1RK1 b - -",        /* 8.O-O */
  };
  CtMove moves[17];
  int index = 0;

  moves[index++] = ct_move_make(E2, E4);        /* 1.e4 */
  moves[index++] = ct_move_make(F7, F5);        /* f5 */
  moves[index++] = ct_move_make(E4, F5);        /* 2.exf5 */
  moves[index++] = ct_move_make_en_passant_possible(G7, G5);        /* g5 */
  moves[index++] = ct_move_make_en_passant_capture(F5, G6);        /* 3.fxg6 */
  moves[index++] = ct_move_make(F8, G7);        /* Bg7 */
  moves[index++] = ct_move_make(G6, H7);        /* 4.gxh7 */
  moves[index++] = ct_move_make(B8, C6);        /* Nc6 */
  moves[index++] = ct_move_make_promotion_R(H7, G8);        /* 5.hxg8=R+ */
  moves[index++] = ct_move_make(H8, G8);        /* Rxg8 */
  moves[index++] = ct_move_make(F1, C4);        /* 6.Bc4 */
  moves[index++] = ct_move_make(D7, D5);        /* d5 */
  moves[index++] = ct_move_make(G1, F3);        /* 7.Nf3 */
  moves[index++] = ct_move_make(A8, B8);        /* Rb8 */
  moves[index++] = ct_move_make_castle_kingside(E1);        /* 8.O-O */
  moves[index] = NULL_MOVE;

  for (index = 0; moves[index] != NULL_MOVE; index++)
  {
    ct_move_maker_make(move_maker, moves[index]);
    ck_assert_str_eq(ct_position_to_fen(position, 0), fens[index + 1]);
  }

  for (index--; index >= 0; index--)
  {
    ct_move_maker_unmake(move_maker);
    ck_assert_str_eq(ct_position_to_fen(position, 0), fens[index]);
  }

  /* check that unmake-ing when there's nothing to unmake doesn't do anything to the position */
  ct_move_maker_unmake(move_maker);
  ck_assert_str_eq(ct_position_to_fen(position, 0), fens[0]);
} END_TEST

START_TEST(ut_move_maker_reset)
{
  char *fens[] =
  {
    "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -",        /* after 1. e4 */
    "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq -",        /* after 1... e5 */
    "rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq -",        /* after 2. Nf3 */
  };

  ct_move_maker_make(move_maker, ct_move_make(E2, E4));
  ct_move_maker_reset(move_maker);        /* move_maker forgets how to undo 1. e4 */
  ct_move_maker_make(move_maker, ct_move_make(E7, E5));
  ct_move_maker_make(move_maker, ct_move_make(G1, F3));
  ck_assert_str_eq(ct_position_to_fen(position, 0), fens[2]);
  ct_move_maker_unmake(move_maker);
  ck_assert_str_eq(ct_position_to_fen(position, 0), fens[1]);
  ct_move_maker_unmake(move_maker);
  ck_assert_str_eq(ct_position_to_fen(position, 0), fens[0]);
  ct_move_maker_unmake(move_maker);        /* can't undo it because of the reset, so it remains the same */
  ck_assert_str_eq(ct_position_to_fen(position, 0), fens[0]);
} END_TEST

Suite *
ut_move_maker_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_move_maker");
  test_case = tcase_create("MoveMaker");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_move_maker_move);
  tcase_add_test(test_case, ut_move_maker_reset);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
