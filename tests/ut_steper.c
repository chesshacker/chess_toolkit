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
#include "ct_steper.h"                /* not part of the advertised API, used by CtMoveGenerator */
#include "check_mg_piece.h"

START_TEST(ut_steper_king)
{
  CheckMgPieceTestStruct tests[] =
  {
    /* note, these are pseudo-legal moves, the king could be in check */
    {false, "8/8/8/8/8/8/8/K7 w - -", A1, {A2, B2, B1, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/3K4 w - -", D1, {D2, E2, E1, C1, C2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/7K/8/8/8 w - -", H4, {H5, H3, G3, G4, G5, SQUARE_NOT_FOUND}},
    {false, "8/8/4K3/8/8/8/8/8 w - -", E6, {E7, F7, F6, F5, E5, D5, D6, D7, SQUARE_NOT_FOUND}},
    {false, "7K/8/8/8/8/8/8/8 w - -", H8, {H7, G7, G8, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/k7 b - -", A1, {A2, B2, B1, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/3k4 b - -", D1, {D2, E2, E1, C1, C2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/7k/8/8/8 b - -", H4, {H5, H3, G3, G4, G5, SQUARE_NOT_FOUND}},
    {false, "8/8/4k3/8/8/8/8/8 b - -", E6, {E7, F7, F6, F5, E5, D5, D6, D7, SQUARE_NOT_FOUND}},
    {false, "7k/8/8/8/8/8/8/8 b - -", H8, {H7, G7, G8, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/KR6 w - -", A1, {A2, B2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/3Qq3/3K4 w - -", D1, {E2, E1, C1, C2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/6Pp/6pK/8/8/8 w - -", H4, {H5, H3, G3, G4, SQUARE_NOT_FOUND}},
    {false, "8/2bnrq2/4K3/PPPPPPPP/8/8/8/8 w - -", E6, {E7, F7, F6, D6, D7, SQUARE_NOT_FOUND}},
    {false, "6QK/6RP/8/8/8/8/8/8 w - -", H8, {SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/kr6 b - -", A1, {A2, B2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/3qQ3/3k4 b - -", D1, {E2, E1, C1, C2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/6pP/6Pk/8/8/8 b - -", H4, {H5, H3, G3, G4, SQUARE_NOT_FOUND}},
    {false, "8/2BNRQ2/4k3/pppppppp/8/8/8/8 b - -", E6, {E7, F7, F6, D6, D7, SQUARE_NOT_FOUND}},
    {false, "6qk/6rp/8/8/8/8/8/8 b - -", H8, {SQUARE_NOT_FOUND}},
    {true, "", 0, {0}},
  };
  CtMoveCommand move_command = check_mg_piece_move_command();
  CtSteper white_king = ct_king_new(move_command, WHITE_PIECE);
  CtSteper black_king = ct_king_new(move_command, BLACK_PIECE);

  check_mg_piece_run_all_tests(white_king, black_king, tests);

  ct_steper_free(white_king);
  ct_steper_free(black_king);
} END_TEST

START_TEST(ut_steper_knight)
{
  CheckMgPieceTestStruct tests[] =
  {
    {false, "8/8/8/8/8/8/8/N7 w - -", A1, {B3, C2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/3N4 w - -", D1, {E3, F2, B2, C3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/6N1/8/8/8 w - -", G4, {H6, H2, F2, E3, E5, F6, SQUARE_NOT_FOUND}},
    {false, "8/8/4N3/8/8/8/8/8 w - -", E6, {F8, G7, G5, F4, D4, C5, C7, D8, SQUARE_NOT_FOUND}},
    {false, "8/7N/8/8/8/8/8/8 w - -", H7, {G5, F6, F8, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/n7 b - -", A1, {B3, C2, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/8/3n4 b - -", D1, {E3, F2, B2, C3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/6n1/8/8/8 b - -", G4, {H6, H2, F2, E3, E5, F6, SQUARE_NOT_FOUND}},
    {false, "8/8/4n3/8/8/8/8/8 b - -", E6, {F8, G7, G5, F4, D4, C5, C7, D8, SQUARE_NOT_FOUND}},
    {false, "8/7n/8/8/8/8/8/8 b - -", H7, {G5, F6, F8, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/1P6/2P5/N7 w - -", A1, {SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/2p1Q3/1R3p2/3N4 w - -", D1, {F2, C3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/1p6/2p5/n7 b - -", A1, {SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/2P1q3/1r3P2/3n4 b - -", D1, {F2, C3, SQUARE_NOT_FOUND}},
    {true, "", 0, {0}},
  };
  CtMoveCommand move_command = check_mg_piece_move_command();
  CtSteper white_knight = ct_knight_new(move_command, WHITE_PIECE);
  CtSteper black_knight = ct_knight_new(move_command, BLACK_PIECE);

  check_mg_piece_run_all_tests(white_knight, black_knight, tests);

  ct_steper_free(white_knight);
  ct_steper_free(black_knight);
} END_TEST

Suite *
ut_steper_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_steper");
  test_case = tcase_create("Steper");
  tcase_add_checked_fixture(test_case, check_mg_piece_setup, check_mg_piece_teardown);
  tcase_add_test(test_case, ut_steper_king);
  tcase_add_test(test_case, ut_steper_knight);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
