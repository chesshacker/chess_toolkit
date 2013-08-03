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

static CtPosition position;

static void setup(void);
static void teardown(void);

static void verify_like_new(void);

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

static void
verify_like_new(void)
{
  CtPiece pieces_from_A1_to_H1[] = {WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN,
  WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK};
  CtPiece pieces_from_A8_to_H8[] = {BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN,
  BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK};
  CtSquare square;

  for (square = A1; square <= H1; square++)
    ck_assert_int_eq(ct_position_get_piece(position, square), pieces_from_A1_to_H1[square]);
  for (square = A2; square <= H2; square++)
    ck_assert_int_eq(ct_position_get_piece(position, square), WHITE_PAWN);
  for (square = A3; square <= H6; square++)
    ck_assert_int_eq(ct_position_get_piece(position, square), EMPTY);
  for (square = A7; square <= H7; square++)
    ck_assert_int_eq(ct_position_get_piece(position, square), BLACK_PAWN);
  for (square = A8; square <= H8; square++)
    ck_assert_int_eq(ct_position_get_piece(position, square), pieces_from_A8_to_H8[square - A8]);
  ck_assert(ct_position_is_white_to_move(position));
  ck_assert_int_eq(ct_position_get_en_passant(position), NO_EN_PASSANT);
  ck_assert_int_eq(ct_position_get_castle(position), CASTLE_KQkq);
}

START_TEST(ut_position_new)
{
  verify_like_new();
} END_TEST

START_TEST(ut_position_clear)
{
  CtSquare square;

  ct_position_clear(position);
  for (square = A1; square <= H8; square++)
    ck_assert_int_eq(ct_position_get_piece(position, square), EMPTY);
  ck_assert(ct_position_is_white_to_move(position));
  ck_assert_int_eq(ct_position_get_en_passant(position), NO_EN_PASSANT);
  ck_assert_int_eq(ct_position_get_castle(position), CASTLE_NONE);
} END_TEST


START_TEST(ut_position_setters)
{
  ct_position_clear(position);

  ct_position_set_piece(position, A1, WHITE_KING);
  ck_assert_int_eq(ct_position_get_piece(position, A1), WHITE_KING);

  ct_position_set_black_to_move(position);
  ck_assert(!ct_position_is_white_to_move(position));
  ct_position_set_white_to_move(position);
  ck_assert(ct_position_is_white_to_move(position));

  ct_position_change_turns(position);
  ck_assert(!ct_position_is_white_to_move(position));
  ct_position_change_turns(position);
  ck_assert(ct_position_is_white_to_move(position));

  /* note, en passant doesn't make sense without any pawns, but it can be set */
  ct_position_set_en_passant(position, FILE_A);
  ck_assert_int_eq(ct_position_get_en_passant(position), FILE_A);
  ct_position_clear_en_passant(position);
  ck_assert_int_eq(ct_position_get_en_passant(position), NO_EN_PASSANT);
  /* you can also set it by passing a square or a file */
  ct_position_set_en_passant(position, B3);
  ck_assert_int_eq(ct_position_get_en_passant(position), FILE_B);
  /* note, setting en passant to NO_EN_PASSANT does not do what you probably intended */
  ct_position_set_en_passant(position, NO_EN_PASSANT);
  ck_assert_int_eq(ct_position_get_en_passant(position), FILE_H);

  /* note, castling doesn't make sense without the kings and rooks setup, but it can be set */
  ct_position_set_castle(position, CASTLE_Kq);
  ck_assert_int_eq(ct_position_get_castle(position), CASTLE_Kq);
} END_TEST

START_TEST(ut_position_reset)
{
  ct_position_clear(position);
  ct_position_set_piece(position, A1, WHITE_KING);
  ct_position_change_turns(position);
  ct_position_set_en_passant(position, FILE_A);
  ct_position_set_castle(position, CASTLE_Kq);
  ct_position_reset(position);
  verify_like_new();
} END_TEST

START_TEST(ut_position_bit_boards)
{
  int index;

  ck_assert(ct_position_get_bit_board(position, WHITE_KING) == ct_bit_board_make(E1));
  ck_assert(ct_position_get_bit_board(position, WHITE_QUEEN) == ct_bit_board_make(D1));
  ck_assert(ct_position_get_bit_board(position, WHITE_ROOK) == (ct_bit_board_make(A1) | ct_bit_board_make(H1)));
  ck_assert(ct_position_get_bit_board(position, WHITE_KNIGHT) == (ct_bit_board_make(B1) | ct_bit_board_make(G1)));
  ck_assert(ct_position_get_bit_board(position, WHITE_BISHOP) == (ct_bit_board_make(C1) | ct_bit_board_make(F1)));
  ck_assert(ct_position_get_bit_board(position, WHITE_PAWN) == 0xFF00);        /* A2 - H2 set */
  ck_assert(ct_position_get_bit_board(position, BLACK_KING) == ct_bit_board_make(E8));
  ck_assert(ct_position_get_bit_board(position, BLACK_QUEEN) == ct_bit_board_make(D8));
  ck_assert(ct_position_get_bit_board(position, BLACK_ROOK) == (ct_bit_board_make(A8) | ct_bit_board_make(H8)));
  ck_assert(ct_position_get_bit_board(position, BLACK_KNIGHT) == (ct_bit_board_make(B8) | ct_bit_board_make(G8)));
  ck_assert(ct_position_get_bit_board(position, BLACK_BISHOP) == (ct_bit_board_make(C8) | ct_bit_board_make(F8)));
  ck_assert(ct_position_get_bit_board(position, BLACK_PAWN) == (0xFF000000000000));        /* A7 - H7 set */
  ck_assert(ct_position_get_bit_board(position, EMPTY) == (0xFFFFFFFF0000));        /* A3 - H6 set */

  ct_position_clear(position);
  ck_assert(ct_position_get_bit_board(position, EMPTY) == BITB_FULL);
  for (index = 1; index <= PIECE_MAX_VALUE; index++)
    ck_assert(ct_position_get_bit_board(position, index) == BITB_EMPTY);

  ct_position_set_piece(position, E2, WHITE_KING);
  ck_assert(ct_position_get_bit_board(position, WHITE_KING) == ct_bit_board_make(E2));
  ck_assert(ct_position_get_bit_board(position, EMPTY) == ~ct_bit_board_make(E2));

  ct_position_reset(position);
  ck_assert(ct_position_get_bit_board(position, EMPTY) == (0xFFFFFFFF0000));        /* A3 - H6 set */
} END_TEST

START_TEST(ut_position_copy)
{
  CtPosition copy_position = ct_position_new();
  CtBitBoard bb_wq, bb_bn, bb_empty;

  ct_position_clear(position);
  ct_position_set_piece(position, E4, WHITE_QUEEN);
  ct_position_set_piece(position, D5, BLACK_KNIGHT);
  ct_position_set_black_to_move(position);
  ct_position_set_en_passant(position, FILE_B);
  ct_position_set_castle(position, CASTLE_Qq);

  bb_wq = ct_position_get_bit_board(position, WHITE_QUEEN);
  bb_bn = ct_position_get_bit_board(position, BLACK_KNIGHT);
  bb_empty = ct_position_get_bit_board(position, EMPTY);

  ct_position_copy(copy_position, position);
  ct_position_reset(position);        /* it doesn't matter what happens to position, we have a copy */

  ck_assert_int_eq(ct_position_get_piece(copy_position, E4), WHITE_QUEEN);
  ck_assert_int_eq(ct_position_get_piece(copy_position, D5), BLACK_KNIGHT);
  ck_assert(!ct_position_is_white_to_move(copy_position));
  ck_assert_int_eq(ct_position_get_en_passant(copy_position), FILE_B);
  ck_assert_int_eq(ct_position_get_castle(copy_position), CASTLE_Qq);

  ck_assert(ct_position_get_bit_board(copy_position, WHITE_QUEEN) == bb_wq);
  ck_assert(ct_position_get_bit_board(copy_position, BLACK_KNIGHT) == bb_bn);
  ck_assert(ct_position_get_bit_board(copy_position, EMPTY) == bb_empty);
} END_TEST

Suite *
ut_position_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_position");
  test_case = tcase_create("Position");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_position_new);
  tcase_add_test(test_case, ut_position_clear);
  tcase_add_test(test_case, ut_position_setters);
  tcase_add_test(test_case, ut_position_reset);
  tcase_add_test(test_case, ut_position_bit_boards);
  tcase_add_test(test_case, ut_position_copy);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
