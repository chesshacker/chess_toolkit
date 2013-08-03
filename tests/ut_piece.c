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

START_TEST(ut_piece_color)
{
  ck_assert_int_eq(ct_piece_color(WHITE_KING), WHITE_PIECE);
  ck_assert_int_eq(ct_piece_color(WHITE_QUEEN), WHITE_PIECE);
  ck_assert_int_eq(ct_piece_color(WHITE_ROOK), WHITE_PIECE);
  ck_assert_int_eq(ct_piece_color(WHITE_BISHOP), WHITE_PIECE);
  ck_assert_int_eq(ct_piece_color(WHITE_KNIGHT), WHITE_PIECE);
  ck_assert_int_eq(ct_piece_color(WHITE_PAWN), WHITE_PIECE);
  ck_assert_int_eq(ct_piece_color(BLACK_KING), BLACK_PIECE);
  ck_assert_int_eq(ct_piece_color(BLACK_QUEEN), BLACK_PIECE);
  ck_assert_int_eq(ct_piece_color(BLACK_ROOK), BLACK_PIECE);
  ck_assert_int_eq(ct_piece_color(BLACK_BISHOP), BLACK_PIECE);
  ck_assert_int_eq(ct_piece_color(BLACK_KNIGHT), BLACK_PIECE);
  ck_assert_int_eq(ct_piece_color(BLACK_PAWN), BLACK_PIECE);
  /* this function is not meant to be used on anything other than the enumerated pieces but always returns something */
  ck_assert_int_eq(ct_piece_color(-1), BLACK_PIECE);
  ck_assert_int_eq(ct_piece_color(EMPTY), WHITE_PIECE);
} END_TEST

START_TEST(ut_piece_from_char)
{
  ck_assert_int_eq(ct_piece_from_char('K'), WHITE_KING);
  ck_assert_int_eq(ct_piece_from_char('Q'), WHITE_QUEEN);
  ck_assert_int_eq(ct_piece_from_char('R'), WHITE_ROOK);
  ck_assert_int_eq(ct_piece_from_char('B'), WHITE_BISHOP);
  ck_assert_int_eq(ct_piece_from_char('N'), WHITE_KNIGHT);
  ck_assert_int_eq(ct_piece_from_char('P'), WHITE_PAWN);
  ck_assert_int_eq(ct_piece_from_char('k'), BLACK_KING);
  ck_assert_int_eq(ct_piece_from_char('q'), BLACK_QUEEN);
  ck_assert_int_eq(ct_piece_from_char('r'), BLACK_ROOK);
  ck_assert_int_eq(ct_piece_from_char('b'), BLACK_BISHOP);
  ck_assert_int_eq(ct_piece_from_char('n'), BLACK_KNIGHT);
  ck_assert_int_eq(ct_piece_from_char('p'), BLACK_PAWN);
  /* any other character returns EMPTY */
  ck_assert_int_eq(ct_piece_from_char('-'), EMPTY);
  ck_assert_int_eq(ct_piece_from_char('?'), EMPTY);
  ck_assert_int_eq(ct_piece_from_char('Z'), EMPTY);
} END_TEST

START_TEST(ut_piece_to_char)
{
  ck_assert_int_eq(ct_piece_to_char(EMPTY), '-');
  ck_assert_int_eq(ct_piece_to_char(WHITE_KING), 'K');
  ck_assert_int_eq(ct_piece_to_char(WHITE_QUEEN), 'Q');
  ck_assert_int_eq(ct_piece_to_char(WHITE_ROOK), 'R');
  ck_assert_int_eq(ct_piece_to_char(WHITE_BISHOP), 'B');
  ck_assert_int_eq(ct_piece_to_char(WHITE_KNIGHT), 'N');
  ck_assert_int_eq(ct_piece_to_char(WHITE_PAWN), 'P');
  ck_assert_int_eq(ct_piece_to_char(BLACK_KING), 'k');
  ck_assert_int_eq(ct_piece_to_char(BLACK_QUEEN), 'q');
  ck_assert_int_eq(ct_piece_to_char(BLACK_ROOK), 'r');
  ck_assert_int_eq(ct_piece_to_char(BLACK_BISHOP), 'b');
  ck_assert_int_eq(ct_piece_to_char(BLACK_KNIGHT), 'n');
  ck_assert_int_eq(ct_piece_to_char(BLACK_PAWN), 'p');
  /* passing an invalid value to this function will return 0 */
  ck_assert_int_eq(ct_piece_to_char(-1), 0);
  ck_assert_int_eq(ct_piece_to_char(PIECE_MAX_VALUE + 1), 0);
  ck_assert_int_eq(ct_piece_to_char(PIECE_MAX_VALUE + 100), 0);
  /* these values are in the range of pieces (0-15) but are not valid pieces, they are reserved values */
  ck_assert_int_eq(ct_piece_to_char(7), 0);
  ck_assert_int_eq(ct_piece_to_char(8), 0);
  ck_assert_int_eq(ct_piece_to_char(15), 0);
} END_TEST

START_TEST(ut_piece_to_white)
{
  ck_assert_int_eq(ct_piece_to_white(BLACK_PAWN), WHITE_PAWN);
  ck_assert_int_eq(ct_piece_to_white(BLACK_KNIGHT), WHITE_KNIGHT);
  ck_assert_int_eq(ct_piece_to_white(BLACK_BISHOP), WHITE_BISHOP);
  ck_assert_int_eq(ct_piece_to_white(BLACK_ROOK), WHITE_ROOK);
  ck_assert_int_eq(ct_piece_to_white(BLACK_QUEEN), WHITE_QUEEN);
  ck_assert_int_eq(ct_piece_to_white(BLACK_KING), WHITE_KING);
  ck_assert_int_eq(ct_piece_to_white(WHITE_PAWN), WHITE_PAWN);
  ck_assert_int_eq(ct_piece_to_white(WHITE_KNIGHT), WHITE_KNIGHT);
  ck_assert_int_eq(ct_piece_to_white(WHITE_BISHOP), WHITE_BISHOP);
  ck_assert_int_eq(ct_piece_to_white(WHITE_ROOK), WHITE_ROOK);
  ck_assert_int_eq(ct_piece_to_white(WHITE_QUEEN), WHITE_QUEEN);
  ck_assert_int_eq(ct_piece_to_white(WHITE_KING), WHITE_KING);
  ck_assert_int_eq(ct_piece_to_white(EMPTY), EMPTY);
} END_TEST

START_TEST(ut_piece_to_black)
{
  ck_assert_int_eq(ct_piece_to_black(WHITE_PAWN), BLACK_PAWN);
  ck_assert_int_eq(ct_piece_to_black(WHITE_KNIGHT), BLACK_KNIGHT);
  ck_assert_int_eq(ct_piece_to_black(WHITE_BISHOP), BLACK_BISHOP);
  ck_assert_int_eq(ct_piece_to_black(WHITE_ROOK), BLACK_ROOK);
  ck_assert_int_eq(ct_piece_to_black(WHITE_QUEEN), BLACK_QUEEN);
  ck_assert_int_eq(ct_piece_to_black(WHITE_KING), BLACK_KING);
  ck_assert_int_eq(ct_piece_to_black(BLACK_PAWN), BLACK_PAWN);
  ck_assert_int_eq(ct_piece_to_black(BLACK_KNIGHT), BLACK_KNIGHT);
  ck_assert_int_eq(ct_piece_to_black(BLACK_BISHOP), BLACK_BISHOP);
  ck_assert_int_eq(ct_piece_to_black(BLACK_ROOK), BLACK_ROOK);
  ck_assert_int_eq(ct_piece_to_black(BLACK_QUEEN), BLACK_QUEEN);
  ck_assert_int_eq(ct_piece_to_black(BLACK_KING), BLACK_KING);
  ck_assert_int_eq(ct_piece_to_black(EMPTY), EMPTY);
} END_TEST

START_TEST(ut_piece_is_pawn)
{
  CtPiece piece;

  for (piece = 0; piece <= PIECE_MAX_VALUE; piece++)
  {
    if (piece == WHITE_PAWN || piece == BLACK_PAWN)
      ck_assert(ct_piece_is_pawn(piece) == true);
    else
      ck_assert(ct_piece_is_pawn(piece) == false);
  }
} END_TEST

Suite *
ut_piece_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_piece");
  test_case = tcase_create("Piece");
  tcase_add_test(test_case, ut_piece_color);
  tcase_add_test(test_case, ut_piece_from_char);
  tcase_add_test(test_case, ut_piece_to_char);
  tcase_add_test(test_case, ut_piece_to_white);
  tcase_add_test(test_case, ut_piece_to_black);
  tcase_add_test(test_case, ut_piece_is_pawn);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
