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

typedef struct UtFenReaderTestStruct *UtFenReaderTest;
typedef struct UtFenReaderTestStruct
{
  bool end_of_tests;
  char msg[100];
  char fen[CT_FEN_MAX_LENGTH];
  int error_in_part_number;        /* only zero or non-zero is currently used, but the specific part is noted for possible
                                   use in the future */
} UtFenReaderTestStruct;

START_TEST(ut_position_from_fen)
{
  CtPosition position;
  UtFenReaderTestStruct tests[] =
  {
    {false, "initial position", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 0},
    {false, "another valid fen", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 0},
    {false, "ignores extra spaces", "  8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8  w   -   - ", 0},
    /* the current implementation doesn't look after part 4 of the FEN */
    {false, "with all 6 parts", "rnbqkbnr/p1ppppp1/8/Pp5p/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 3", 0},
    {false, "with arbitrary junk after part 4", "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 foo bar baz", 0},
    {false, "invalid character in part 1", "rnbqZbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 1},
    {false, "too many pieces in a rank", "rnbqkbnr/ppppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 1},
    {false, "too many spaces in a rank", "rnbqkbnr/pppppppp/8/8/8/8/P2PPPPPP/RNBQKBNR w KQkq -", 1},
    {false, "too few pieces in a rank", "rnbqkbnr/ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 1},
    {false, "too few spaces in a rank", "rnbqkbnr/pppppppp/8/7/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 1},
    {false, "too many ranks", "rnbqkbnr/pppppppp/8/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", 1},
    {false, "invalid character in part 2", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 q - -", 2},
    {false, "too many characters in part 2", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 wq - -", 2},
    {false, "invalid character in part 3", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQzq -", 3},
    {false, "castle rights invalid in part 3", "rnbqkbnr/pppppppp/8/8/8/4K3/PPPPPPPP/RNBQ1BNR w KQkq -", 3},
    {false, "not enough parts", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w -", 3},
    {false, "invalid en passant square", "rnbqkbnr/p1ppppp1/8/Pp5p/8/8/1PPPPPPP/RNBQKBNR w KQkq b3", 4},
    {false, "valid en passant square", "rnbqkbnr/ppppppp1/8/P7/6Pp/8/1PPPPP1P/RNBQKBNR b KQkq g3", 0},
    {false, "invalid en passant square", "rnbqkbnr/ppppppp1/8/P7/6Pp/8/1PPPPP1P/RNBQKBNR b KQkq g6", 4},
    {true, "", "", 0},
  };
  UtFenReaderTest test;

  for (test = tests; !test->end_of_tests; test++)
  {
    position = ct_position_from_fen(0, test->fen);
    if (test->error_in_part_number)
      ck_assert_msg(position == 0, test->msg);
    else
      ck_assert_msg(position != 0, test->msg);
  }

  position = ct_position_from_fen(0, "rnbqkbn1/p1ppppp1/7r/Pp5p/8/R7/1PPPPPPP/1NBQKBNR w Kq b6");
  ck_assert(position != 0);
  ck_assert_int_eq(ct_position_get_piece(position, A2), EMPTY);
  ck_assert_int_eq(ct_position_get_piece(position, B2), WHITE_PAWN);
  ck_assert_int_eq(ct_position_get_piece(position, B5), BLACK_PAWN);
  ck_assert_int_eq(ct_position_get_en_passant(position), FILE_B);
  ck_assert_int_eq(ct_position_get_castle(position), CASTLE_Kq);
  ck_assert(ct_position_is_white_to_move(position));
} END_TEST

Suite *
ut_position_from_fen_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_position_from_fen");
  test_case = tcase_create("Position.from_fen");
  tcase_add_test(test_case, ut_position_from_fen);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
