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
#include "ct_pawn.h"                /* not part of the advertised API, used by CtMoveGenerator */
#include "check_mg_piece.h"

static CtSquare expected_from, expected_to;
static int move_count;
static CtPiece expected_promotions[] =
{
  WHITE_QUEEN, WHITE_ROOK, WHITE_BISHOP, WHITE_KNIGHT,
  BLACK_QUEEN, BLACK_ROOK, BLACK_BISHOP, BLACK_KNIGHT, EMPTY
};
static CtMoveType expected_move_type;
static bool verified_move_type;

static void special_setup(void);
static void special_teardown(void);

static void test_promotion_move_command(void *ignore, CtMove move);
static void test_move_type_command(void *ignore, CtMove move);

START_TEST(ut_pawn_promotion)
{
  CtMoveCommand move_command = ct_move_command_new(0, test_promotion_move_command);
  CtPawn white_pawn = ct_pawn_new(move_command, WHITE_PIECE);
  CtPawn black_pawn = ct_pawn_new(move_command, BLACK_PIECE);
  CtPosition position;

  position = ct_position_from_fen(0, "8/P7/8/8/8/8/8/8 w - -");
  expected_from = A7;
  expected_to = A8;
  move_count = 0;
  ct_pawn_move(white_pawn, position, expected_from);
  ck_assert_int_eq(move_count, 4);

  position = ct_position_from_fen(0, "8/8/8/8/8/8/p7/KQ6 b - -");
  expected_from = A2;
  expected_to = B1;
  ct_pawn_move(black_pawn, position, expected_from);
  ck_assert_int_eq(move_count, 8);

  ct_pawn_free(black_pawn);
  ct_pawn_free(white_pawn);
  ct_move_command_free(move_command);
} END_TEST

static void
test_promotion_move_command(void *ignore, CtMove move)
{
  ck_assert_int_eq(ct_move_from(move), expected_from);
  ck_assert_int_eq(ct_move_to(move), expected_to);
  ck_assert_int_eq(ct_move_type(move), PROMOTION);
  ck_assert_int_eq(ct_move_promotes_to(move), expected_promotions[move_count]);
  move_count++;
}

START_TEST(ut_pawn_en_passant)
{
  CtMoveCommand move_command = ct_move_command_new(0, test_move_type_command);
  CtPawn white_pawn = ct_pawn_new(move_command, WHITE_PIECE);
  CtPawn black_pawn = ct_pawn_new(move_command, BLACK_PIECE);
  CtPosition position;

  position = ct_position_from_fen(0, "8/1p6/8/P7/8/8/8/8 b - -");
  expected_from = B7;
  expected_to = B5;
  expected_move_type = EN_PASSANT_POSSIBLE;
  verified_move_type = false;
  ct_pawn_move(black_pawn, position, expected_from);
  ck_assert(verified_move_type);

  position = ct_position_from_fen(0, "8/8/8/Pp6/8/8/8/8 w - b6");
  expected_from = A5;
  expected_to = B6;
  expected_move_type = EN_PASSANT_CAPTURE;
  verified_move_type = false;
  ct_pawn_move(white_pawn, position, expected_from);
  ck_assert(verified_move_type);

  position = ct_position_from_fen(0, "8/8/8/8/5p2/8/4P3/8 w - -");
  expected_from = E2;
  expected_to = E4;
  expected_move_type = EN_PASSANT_POSSIBLE;
  verified_move_type = false;
  ct_pawn_move(white_pawn, position, expected_from);
  ck_assert(verified_move_type);

  position = ct_position_from_fen(0, "8/8/8/8/4Pp2/8/8/8 b - e3");
  expected_from = F4;
  expected_to = E3;
  expected_move_type = EN_PASSANT_CAPTURE;
  verified_move_type = false;
  ct_pawn_move(black_pawn, position, expected_from);
  ck_assert(verified_move_type);

  ct_pawn_free(black_pawn);
  ct_pawn_free(white_pawn);
  ct_move_command_free(move_command);
} END_TEST

static void
test_move_type_command(void *ignore, CtMove move)
{
  ck_assert_int_eq(ct_move_from(move), expected_from);
  if (ct_move_to(move) == expected_to)
  {
    ck_assert_int_eq(ct_move_type(move), expected_move_type);
    verified_move_type = true;
  }
  else
    ck_assert_int_eq(ct_move_type(move), NORMAL_MOVE);
}

START_TEST(ut_pawn)
{
  CheckMgPieceTestStruct tests[] =
  {
    /* pawn can move forward one or two squares from their starting rank */
    {false, "8/8/8/8/8/8/P7/8 w - -", A2, {A3, A4, SQUARE_NOT_FOUND}},
    {false, "8/p7/8/8/8/8/8/8 b - -", A7, {A6, A5, SQUARE_NOT_FOUND}},
    /* the file doesn't matter, but many of the tests are on the A file because it's easy to see in the fen */
    {false, "8/8/8/8/8/8/1P6/8 w - -", B2, {B3, B4, SQUARE_NOT_FOUND}},
    /* they can't move ahead two if any piece is occupying that square */
    {false, "8/8/8/8/q7/8/P7/8 w - -", A2, {A3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/Q7/8/P7/8 w - -", A2, {A3, SQUARE_NOT_FOUND}},
    {false, "8/p7/8/Q7/8/8/8/8 b - -", A7, {A6, SQUARE_NOT_FOUND}},
    {false, "8/p7/8/q7/8/8/8/8 b - -", A7, {A6, SQUARE_NOT_FOUND}},
    /* if any piece is directly ahead of them they can't even move forward one */
    {false, "8/8/8/8/8/q7/P7/8 w - -", A2, {SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/Q7/P7/8 w - -", A2, {SQUARE_NOT_FOUND}},
    {false, "8/p7/Q7/8/8/8/8/8 b - -", A7, {SQUARE_NOT_FOUND}},
    {false, "8/p7/q7/8/8/8/8/8 b - -", A7, {SQUARE_NOT_FOUND}},
    /* after moving from their starting rank, they can only move one square forward at a time */
    {false, "8/8/8/8/8/P7/8/8 w - -", A3, {A4, SQUARE_NOT_FOUND}},
    {false, "8/8/8/p7/8/8/8/8 b - -", A5, {A4, SQUARE_NOT_FOUND}},
    /* again, they can't move forward at all if someone is directly in front of them */
    {false, "8/8/8/8/q7/P7/8/8 w - -", A3, {SQUARE_NOT_FOUND}},
    {false, "8/8/8/p7/q7/8/8/8 b - -", A5, {SQUARE_NOT_FOUND}},
    /* if an enemy is one square diagonally forward from the pawn, the pawn can capture it */
    {false, "8/8/8/8/8/rr6/P7/8 w - -", A2, {B3, SQUARE_NOT_FOUND}},
    {false, "8/p7/RR6/8/8/8/8/8 b - -", A7, {B6, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/ppp5/pPp5/ppp5/8 w - -", B3, {A4, C4, SQUARE_NOT_FOUND}},
    {false, "8/PPP5/PpP5/PPP5/8/8/8/8 b - -", B6, {A5, C5, SQUARE_NOT_FOUND}},
    /* on the edge of the board, a pawn can only capture one direction (it can't wrap around the board) */
    {false, "8/8/8/8/7p/rr5p/P6p/8 w - -", A2, {B3, SQUARE_NOT_FOUND}},
    {false, "8/P6p/P5RR/P7/8/8/8/8 b - -", H7, {G6, SQUARE_NOT_FOUND}},
    /* you can't capture your own pieces though */
    {false, "8/8/8/8/RNB5/QPK5/RNB5/8 w - -", B3, {SQUARE_NOT_FOUND}},
    {false, "8/rnb5/kpq5/rnb5/8/8/8/8 b - -", B6, {SQUARE_NOT_FOUND}},
    /* a pawn can't capture en-passant unless the en-passant square is diagonally forward one square */
    {false, "8/8/8/P7/8/8/8/8 w - -", A5, {A6, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/4Pp2/8/8/8 b - -", F4, {F3, SQUARE_NOT_FOUND}},
    {true, "", 0, {0}},
  };
  CtMoveCommand move_command = check_mg_piece_move_command();
  CtPawn white_pawn = ct_pawn_new(move_command, WHITE_PIECE);
  CtPawn black_pawn = ct_pawn_new(move_command, BLACK_PIECE);

  check_mg_piece_run_all_tests(white_pawn, black_pawn, tests);

  ct_pawn_free(white_pawn);
  ct_pawn_free(black_pawn);
} END_TEST

Suite *
ut_pawn_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_pawn");
  test_case = tcase_create("Pawn");
  tcase_add_checked_fixture(test_case, check_mg_piece_setup, check_mg_piece_teardown);
  tcase_add_test(test_case, ut_pawn);
  suite_add_tcase(test_suite, test_case);
  test_case = tcase_create("Pawn Special");
  tcase_add_test(test_case, ut_pawn_promotion);
  tcase_add_test(test_case, ut_pawn_en_passant);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
