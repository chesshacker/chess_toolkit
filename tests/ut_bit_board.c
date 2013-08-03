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
#include <stdio.h>

typedef struct UtBitBoardTestStruct *UtBitBoardTest;
typedef struct UtBitBoardTestStruct
{
  bool end_of_tests;
  CtSquare king_on;
  CtSquare answers[22];
} UtBitBoardTestStruct;

typedef bool (*IsAttackingMethod) (CtBitBoardArray, CtSquare);

static CtBitBoard is_attacking_bba[CT_BIT_BOARD_ARRAY_LENGTH];

static CtBitBoard ut_bit_board_make_from_squares(CtSquare *squares);
static void ut_is_attacking_setup(void);
static void ut_is_attacking_teardown(void);
static void ut_king_is_attacked_when_piece_is_on(CtPiece which_king, CtPiece piece, UtBitBoardTest tests);

START_TEST(ut_BITB)
{
  ck_assert(ct_bit_board_make(0) == 1);
  ck_assert(ct_bit_board_make(1) == 2);
  ck_assert(ct_bit_board_make(2) == 4);
  ck_assert(ct_bit_board_make(31) == 0x80000000);
  ck_assert(ct_bit_board_make(32) == 0x100000000);
  ck_assert(ct_bit_board_make(63) == 0x8000000000000000);
  ck_assert((ct_bit_board_make(63) | ct_bit_board_make(31) | ct_bit_board_make(2)) == 0x8000000080000004);
} END_TEST

START_TEST(ut_bit_board_find_first_square)
{
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(A1)) == A1);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(A8)) == A8);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(B2)) == B2);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(E6)) == E6);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(F7)) == F7);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(H8)) == H8);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(H8) | ct_bit_board_make(A2)) == A2);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(B2) | ct_bit_board_make(A2)) == A2);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(A7) | ct_bit_board_make(B2)) == B2);
  ck_assert(ct_bit_board_find_first_square(ct_bit_board_make(A7) | ct_bit_board_make(B2) | ct_bit_board_make(A2)) == A2);
  ck_assert(ct_bit_board_find_first_square(BITB_FULL) == A1);
  ck_assert(ct_bit_board_find_first_square(BITB_EMPTY) == SQUARE_NOT_FOUND);
} END_TEST

START_TEST(ut_bit_board_array_reset)
{
  CtBitBoard bit_board_array[CT_BIT_BOARD_ARRAY_LENGTH];
  int i;

  ct_bit_board_array_reset(bit_board_array);
  for (i = 0; i < CT_BIT_BOARD_ARRAY_LENGTH; i++)
  {
    if (i != EMPTY)
      ck_assert(bit_board_array[i] == BITB_EMPTY);
  }
  ck_assert(bit_board_array[EMPTY] == BITB_FULL);
} END_TEST

START_TEST(ut_bit_board_white_pawn_attacks)
{
  UtBitBoardTestStruct tests[] = {
    {false, A3, {B2, SQUARE_NOT_FOUND}},
    {false, A1, {SQUARE_NOT_FOUND}},
    {false, C1, {SQUARE_NOT_FOUND}},
    {false, A2, {B1, SQUARE_NOT_FOUND}},        /* really a pawn can't sit on the 1st rank, but if it could */
    {false, D2, {C1, E1, SQUARE_NOT_FOUND}},        /* ditto */
    {false, B3, {A2, C2, SQUARE_NOT_FOUND}},
    {false, E5, {D4, F4, SQUARE_NOT_FOUND}},
    {false, F7, {E6, G6, SQUARE_NOT_FOUND}},
    {false, G8, {F7, H7, SQUARE_NOT_FOUND}},
    {false, H8, {G7, SQUARE_NOT_FOUND}},
    {false, H5, {G4, SQUARE_NOT_FOUND}},
    {false, H1, {SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_PAWN, tests);
} END_TEST

START_TEST(ut_bit_board_black_pawn_attacks)
{
  UtBitBoardTestStruct tests[] = {
    {false, A6, {B7, SQUARE_NOT_FOUND}},
    {false, A8, {SQUARE_NOT_FOUND}},
    {false, C8, {SQUARE_NOT_FOUND}},
    {false, A7, {B8, SQUARE_NOT_FOUND}},        /* really a pawn can't sit on the 1st rank, but if it could */
    {false, D7, {C8, E8, SQUARE_NOT_FOUND}},        /* ditto */
    {false, B6, {A7, C7, SQUARE_NOT_FOUND}},
    {false, E4, {D5, F5, SQUARE_NOT_FOUND}},
    {false, F2, {E3, G3, SQUARE_NOT_FOUND}},
    {false, G1, {F2, H2, SQUARE_NOT_FOUND}},
    {false, H1, {G2, SQUARE_NOT_FOUND}},
    {false, H4, {G5, SQUARE_NOT_FOUND}},
    {false, H8, {SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_PAWN, tests);
} END_TEST

START_TEST(ut_bit_board_king_attacks)
{
  UtBitBoardTestStruct tests[] = {
    {false, A1, {B1, A2, B2, SQUARE_NOT_FOUND}},
    {false, D1, {C1, E1, C2, D2, E2, SQUARE_NOT_FOUND}},
    {false, H1, {G1, G2, H2, SQUARE_NOT_FOUND}},
    {false, A2, {A1, B1, B2, A3, B3, SQUARE_NOT_FOUND}},
    {false, B2, {A1, B1, C1, A2, C2, A3, B3, C3, SQUARE_NOT_FOUND}},
    {false, H4, {G3, H3, G4, G5, H5, SQUARE_NOT_FOUND}},
    {false, A8, {A7, B7, B8, SQUARE_NOT_FOUND}},
    {false, F8, {E7, F7, G7, E8, G8, SQUARE_NOT_FOUND}},
    {false, H8, {G7, H7, G8, SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_KING, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_KING, tests);
} END_TEST

START_TEST(ut_bit_board_knight_attacks)
{
  UtBitBoardTestStruct tests[] = {
    {false, A1, {C2, B3, SQUARE_NOT_FOUND}},
    {false, B1, {D2, A3, C3, SQUARE_NOT_FOUND}},
    {false, C1, {A2, E2, B3, D3, SQUARE_NOT_FOUND}},
    {false, A2, {C1, C3, B4, SQUARE_NOT_FOUND}},
    {false, B2, {D1, D3, A4, C4, SQUARE_NOT_FOUND}},
    {false, C2, {A1, E1, A3, E3, B4, D4, SQUARE_NOT_FOUND}},
    {false, A3, {B1, C2, C4, B5, SQUARE_NOT_FOUND}},
    {false, B3, {A1, C1, D2, D4, A5, C5, SQUARE_NOT_FOUND}},
    {false, C3, {B1, D1, A2, E2, A4, E4, B5, D5, SQUARE_NOT_FOUND}},
    {false, H2, {F1, F3, G4, SQUARE_NOT_FOUND}},
    {false, H6, {G4, F5, F7, G8, SQUARE_NOT_FOUND}},
    {false, H8, {G6, F7, SQUARE_NOT_FOUND}},
    {false, E7, {D5, F5, C6, G6, C8, G8, SQUARE_NOT_FOUND}},
    {false, B8, {A6, C6, D7, SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_KNIGHT, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_KNIGHT, tests);
} END_TEST

START_TEST(ut_bit_board_rook_attacks)
{
  UtBitBoardTestStruct tests[] = {
    {false, A1, {B1, C1, D1, E1, F1, G1, H1, A2, A3, A4, A5, A6, A7, A8, SQUARE_NOT_FOUND}},
    {false, C1, {A1, B1, D1, E1, F1, G1, H1, C2, C3, C4, C5, C6, C7, C8, SQUARE_NOT_FOUND}},
    {false, H1, {A1, B1, C1, D1, E1, F1, G1, H2, H3, H4, H5, H6, H7, H8, SQUARE_NOT_FOUND}},
    {false, A2, {A1, B2, C2, D2, E2, F2, G2, H2, A3, A4, A5, A6, A7, A8, SQUARE_NOT_FOUND}},
    {false, D4, {D1, D2, D3, A4, B4, C4, E4, F4, G4, H4, D5, D6, D7, D8, SQUARE_NOT_FOUND}},
    {false, H5, {H1, H2, H3, H4, A5, B5, C5, D5, E5, F5, G5, H6, H7, H8, SQUARE_NOT_FOUND}},
    {false, A8, {A1, A2, A3, A4, A5, A6, A7, B8, C8, D8, E8, F8, G8, H8, SQUARE_NOT_FOUND}},
    {false, B8, {B1, B2, B3, B4, B5, B6, B7, A8, C8, D8, E8, F8, G8, H8, SQUARE_NOT_FOUND}},
    {false, H8, {H1, H2, H3, H4, H5, H6, H7, A8, B8, C8, D8, E8, F8, G8, SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_ROOK, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_ROOK, tests);
} END_TEST

START_TEST(ut_bit_board_rook_attacks_with_blockers)
{
  UtBitBoardTestStruct tests[] = {
    {false, A1, {B1, C1, D1, E1, F1, G1, H1, SQUARE_NOT_FOUND}},
    {false, B1, {A1, C1, D1, E1, F1, G1, H1, B2, SQUARE_NOT_FOUND}},
    {false, B4, {A4, B5, B6, B7, B8, SQUARE_NOT_FOUND}},
    {false, E5, {E1, E2, E3, E4, F5, G5, H5, SQUARE_NOT_FOUND}},
    {false, C7, {C5, C6, A7, B7, D7, E7, C8, SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  CtSquare pawn_on_squares[] = {A2, B3, C4, D5, E6, F7, SQUARE_NOT_FOUND};

  is_attacking_bba[WHITE_PAWN] = ut_bit_board_make_from_squares(pawn_on_squares);
  is_attacking_bba[EMPTY] ^= is_attacking_bba[WHITE_PAWN];
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_ROOK, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_ROOK, tests);
} END_TEST

START_TEST(ut_bit_board_bishop_attacks)
{
  UtBitBoardTestStruct tests[] = {
    {false, A1, {B2, C3, D4, E5, F6, G7, H8, SQUARE_NOT_FOUND}},
    {false, C1, {B2, D2, A3, E3, F4, G5, H6, SQUARE_NOT_FOUND}},
    {false, H1, {G2, F3, E4, D5, C6, B7, A8, SQUARE_NOT_FOUND}},
    {false, A2, {B1, B3, C4, D5, E6, F7, G8, SQUARE_NOT_FOUND}},
    {false, D4, {A1, G1, B2, F2, C3, E3, C5, E5, B6, F6, A7, G7, H8, SQUARE_NOT_FOUND}},
    {false, H5, {D1, E2, F3, G4, G6, F7, E8, SQUARE_NOT_FOUND}},
    {false, A8, {H1, G2, F3, E4, D5, C6, B7, SQUARE_NOT_FOUND}},
    {false, B8, {H2, G3, F4, E5, D6, A7, C7, SQUARE_NOT_FOUND}},
    {false, H8, {A1, B2, C3, D4, E5, F6, G7, SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_BISHOP, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_BISHOP, tests);
} END_TEST

START_TEST(ut_bit_board_bishop_attacks_with_blockers)
{
  UtBitBoardTestStruct tests[] = {
    {false, A1, {B2, C3, D4, SQUARE_NOT_FOUND}},
    {false, C1, {B2, D2, E3, F4, G5, H6, SQUARE_NOT_FOUND}},
    {false, H1, {G2, F3, E4, SQUARE_NOT_FOUND}},
    {false, A2, {B1, B3, C4, D5, SQUARE_NOT_FOUND}},
    {false, D4, {A1, G1, B2, F2, C3, E3, SQUARE_NOT_FOUND}},
    {false, H7, {G6, G8, SQUARE_NOT_FOUND}},        /* H5 is under attack by the pawn on G4, testing H7 instead */
    {false, A8, {C6, B7, SQUARE_NOT_FOUND}},
    {false, B8, {D6, A7, C7, SQUARE_NOT_FOUND}},
    {false, H8, {F6, G7, SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  CtSquare pawn_on_squares[] = {A3, B4, C5, D5, E5, F5, G4, H3, SQUARE_NOT_FOUND};

  is_attacking_bba[WHITE_PAWN] = ut_bit_board_make_from_squares(pawn_on_squares);
  is_attacking_bba[EMPTY] ^= is_attacking_bba[WHITE_PAWN];
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_BISHOP, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_BISHOP, tests);
} END_TEST

START_TEST(ut_bit_board_queen_attacks_with_blockers)
{
  UtBitBoardTestStruct tests[] = {
    {false, A1, {B1, C1, D1, E1, F1, G1, H1, A2, B2, C3, D4, SQUARE_NOT_FOUND}},
    {false, C1, {A1, B1, D1, E1, F1, G1, H1, B2, C2, D2, C3, E3, C4, F4, G5, H6, SQUARE_NOT_FOUND}},
    {false, H1, {A1, B1, C1, D1, E1, F1, G1, G2, H2, F3, E4, SQUARE_NOT_FOUND}},
    {false, A2, {A1, B1, B2, C2, D2, E2, F2, G2, H2, B3, C4, D5, SQUARE_NOT_FOUND}},
    {false, D4, {A1, D1, G1, B2, D2, F2, C3, D3, E3, C4, E4, F4, SQUARE_NOT_FOUND}},
    {false, H7, {H4, H5, G6, H6, A7, B7, C7, D7, E7, F7, G7, G8, H8, SQUARE_NOT_FOUND}},
    {false, A8, {A4, A5, A6, C6, A7, B7, B8, C8, D8, E8, F8, G8, H8, SQUARE_NOT_FOUND}},
    {false, B8, {B5, B6, D6, A7, B7, C7, A8, C8, D8, E8, F8, G8, H8, SQUARE_NOT_FOUND}},
    {false, H8, {H4, H5, F6, H6, G7, H7, A8, B8, C8, D8, E8, F8, G8, SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  CtSquare pawn_on_squares[] = {A3, B4, C5, D5, E5, F5, G4, H3, SQUARE_NOT_FOUND};

  is_attacking_bba[WHITE_PAWN] = ut_bit_board_make_from_squares(pawn_on_squares);
  is_attacking_bba[EMPTY] ^= is_attacking_bba[WHITE_PAWN];
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, BLACK_QUEEN, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, WHITE_QUEEN, tests);
} END_TEST

/* this might make a handy function in ct_bit_board too, but I haven't needed it for anything but testing yet. */
static CtBitBoard
ut_bit_board_make_from_squares(CtSquare *squares)
{
  CtBitBoard result = 0;

  for (; *squares != SQUARE_NOT_FOUND; squares++)
    result |= ct_bit_board_make(*squares);
  return result;
}

/* tests that you can't attack your own king */
START_TEST(ut_bit_board_own_piece_attacks)
{
  UtBitBoardTestStruct tests[] = {
    {false, F1, {SQUARE_NOT_FOUND}},
    {false, D4, {SQUARE_NOT_FOUND}},
    {false, A8, {SQUARE_NOT_FOUND}},
    {true, 0, {0}}
  };
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, WHITE_QUEEN, tests);
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, WHITE_ROOK, tests);
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, WHITE_BISHOP, tests);
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, WHITE_KNIGHT, tests);
  ut_king_is_attacked_when_piece_is_on(WHITE_KING, WHITE_PAWN, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, BLACK_QUEEN, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, BLACK_ROOK, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, BLACK_BISHOP, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, BLACK_KNIGHT, tests);
  ut_king_is_attacked_when_piece_is_on(BLACK_KING, BLACK_PAWN, tests);
} END_TEST

static void
ut_is_attacking_setup(void)
{
  ct_bit_board_array_reset(is_attacking_bba);
}

static void
ut_is_attacking_teardown(void)
{
}

/* placing the specified piece on each of the empty squares, to test if the king is under attack */
static void
ut_king_is_attacked_when_piece_is_on(CtPiece which_king, CtPiece piece, UtBitBoardTest tests)
{
  CtSquare *answer;
  int problem_number = 0;
  CtSquare square;
  CtBitBoard king_on, square_bb;
  CtBitBoard *empty_bb = &is_attacking_bba[EMPTY];
  bool result;
  UtBitBoardTest test;

  CtBitBoard *king_bb;
  CtBitBoard *piece_bb = &is_attacking_bba[piece];
  IsAttackingMethod is_attacking_method = 0;

  if (which_king == WHITE_KING)
    is_attacking_method = ct_bit_board_array_is_black_attacking;
  else if (which_king == BLACK_KING)
    is_attacking_method = ct_bit_board_array_is_white_attacking;
  ck_assert(is_attacking_method != 0);        /* which king must be WHITE_KING or BLACK_KING */

  king_bb = &is_attacking_bba[which_king];
  ck_assert(*king_bb == 0);        /* the king_bb should start out empty */

  for (test = tests, problem_number = 1; !test->end_of_tests; problem_number++, test++)
  {
    answer = test->answers;
    king_on = test->king_on;
    *king_bb = ct_bit_board_make(king_on);
    *empty_bb ^= *king_bb;
    ck_assert(!is_attacking_method(is_attacking_bba, king_on));        /* king should not be attacked here */
    for (square = A1; square < NUMBER_OF_SQUARES; square++)
    {
      square_bb = ct_bit_board_make(square);
      if (*empty_bb & square_bb)/* current square is empty */
      {
        *piece_bb ^= square_bb;
        *empty_bb ^= square_bb;
        result = is_attacking_method(is_attacking_bba, king_on);
        if (result ^ (*answer == square))
        {
          char msg[100];

          snprintf(msg, sizeof(msg), "Failed problem %d: *answer=%d, square=%d, result=%s",
                   problem_number, *answer, square, (result ? "true" : "false"));
          ck_abort_msg(msg);
        }
        if (result)
          answer++;
        *piece_bb ^= square_bb;
        *empty_bb ^= square_bb;
      }
    }
    *empty_bb ^= *king_bb;
  }
  *king_bb = 0;
}

Suite *
ut_bit_board_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_bit_board");
  test_case = tcase_create("BitBoard");
  tcase_add_test(test_case, ut_BITB);
  tcase_add_test(test_case, ut_bit_board_find_first_square);
  suite_add_tcase(test_suite, test_case);

  test_case = tcase_create("BitBoardArray");
  tcase_add_test(test_case, ut_bit_board_array_reset);
  suite_add_tcase(test_suite, test_case);

  test_case = tcase_create("BitBoardArray.is_attacking");
  tcase_add_checked_fixture(test_case, ut_is_attacking_setup, ut_is_attacking_teardown);
  tcase_add_test(test_case, ut_bit_board_white_pawn_attacks);
  tcase_add_test(test_case, ut_bit_board_black_pawn_attacks);
  tcase_add_test(test_case, ut_bit_board_king_attacks);
  tcase_add_test(test_case, ut_bit_board_knight_attacks);
  tcase_add_test(test_case, ut_bit_board_rook_attacks);
  tcase_add_test(test_case, ut_bit_board_rook_attacks_with_blockers);
  tcase_add_test(test_case, ut_bit_board_bishop_attacks);
  tcase_add_test(test_case, ut_bit_board_bishop_attacks_with_blockers);
  tcase_add_test(test_case, ut_bit_board_queen_attacks_with_blockers);
  tcase_add_test(test_case, ut_bit_board_own_piece_attacks);
  suite_add_tcase(test_suite, test_case);

  return test_suite;
}
