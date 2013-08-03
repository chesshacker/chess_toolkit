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
#include "ct_slider.h"                /* not part of the advertised API, used by CtMoveGenerator */
#include "check_mg_piece.h"

START_TEST(ut_slider_queen)
{
  CheckMgPieceTestStruct tests[] =
  {
    {false, "8/8/8/4Q3/8/8/8/8 w - -", E5,
    {E6, E7, E8, F6, G7, H8, F5, G5, H5, F4, G3, H2, E4, E3, E2, E1, D4, C3, B2, A1, D5, C5, B5, A5, D6, C7, B8, SQUARE_NOT_FOUND}},
    {false, "8/8/8/4q3/8/8/8/8 b - -", E5,
    {E6, E7, E8, F6, G7, H8, F5, G5, H5, F4, G3, H2, E4, E3, E2, E1, D4, C3, B2, A1, D5, C5, B5, A5, D6, C7, B8, SQUARE_NOT_FOUND}},
    {false, "8/6p1/4R3/1P2Q3/8/8/7q/8 w - -", E5,
    {F6, G7, F5, G5, H5, F4, G3, H2, E4, E3, E2, E1, D4, C3, B2, A1, D5, C5, D6, C7, B8, SQUARE_NOT_FOUND}},
    {false, "8/6P1/4r3/1p2q3/8/8/7Q/8 b - -", E5,
    {F6, G7, F5, G5, H5, F4, G3, H2, E4, E3, E2, E1, D4, C3, B2, A1, D5, C5, D6, C7, B8, SQUARE_NOT_FOUND}},
    {true, "", 0, {0}},
  };
  CtMoveCommand move_command = check_mg_piece_move_command();
  CtSlider white_queen = ct_queen_new(move_command, WHITE_PIECE);
  CtSlider black_queen = ct_queen_new(move_command, BLACK_PIECE);

  check_mg_piece_run_all_tests(white_queen, black_queen, tests);

  ct_slider_free(white_queen);
  ct_slider_free(black_queen);
} END_TEST

START_TEST(ut_slider_rook)
{
  CheckMgPieceTestStruct tests[] =
  {
    {false, "8/8/8/8/8/3R4/8/8 w - -", D3,
    {D4, D5, D6, D7, D8, E3, F3, G3, H3, D2, D1, C3, B3, A3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/3r4/8/8 b - -", D3,
    {D4, D5, D6, D7, D8, E3, F3, G3, H3, D2, D1, C3, B3, A3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/3q4/8/3R2P1/8/8 w - -", D3,
    {D4, D5, E3, F3, D2, D1, C3, B3, A3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/3Q4/8/3r2p1/8/8 b - -", D3,
    {D4, D5, E3, F3, D2, D1, C3, B3, A3, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/8/8/7p/6qr b - -", H1,
    {SQUARE_NOT_FOUND}},
    {true, "", 0, {0}},
  };
  CtMoveCommand move_command = check_mg_piece_move_command();
  CtSlider white_rook = ct_rook_new(move_command, WHITE_PIECE);
  CtSlider black_rook = ct_rook_new(move_command, BLACK_PIECE);

  check_mg_piece_run_all_tests(white_rook, black_rook, tests);

  ct_slider_free(white_rook);
  ct_slider_free(black_rook);
} END_TEST

START_TEST(ut_slider_bishop)
{
  CheckMgPieceTestStruct tests[] =
  {
    {false, "8/8/8/8/6B1/8/8/8 w - -", G4,
    {H5, H3, F3, E2, D1, F5, E6, D7, C8, SQUARE_NOT_FOUND}},
    {false, "8/8/8/8/6b1/8/8/8 b - -", G4,
    {H5, H3, F3, E2, D1, F5, E6, D7, C8, SQUARE_NOT_FOUND}},
    {false, "8/8/4p3/7r/6B1/7P/4Q3/8 w - -", G4,
    {H5, F3, F5, E6, SQUARE_NOT_FOUND}},
    {false, "8/8/4P3/7R/6b1/7p/4q3/8 b - -", G4,
    {H5, F3, F5, E6, SQUARE_NOT_FOUND}},
    {true, "", 0, {0}},
  };
  CtMoveCommand move_command = check_mg_piece_move_command();
  CtSlider white_bishop = ct_bishop_new(move_command, WHITE_PIECE);
  CtSlider black_bishop = ct_bishop_new(move_command, BLACK_PIECE);

  check_mg_piece_run_all_tests(white_bishop, black_bishop, tests);

  ct_slider_free(white_bishop);
  ct_slider_free(black_bishop);
} END_TEST

Suite *
ut_slider_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_slider");
  test_case = tcase_create("Slider");
  tcase_add_checked_fixture(test_case, check_mg_piece_setup, check_mg_piece_teardown);
  tcase_add_test(test_case, ut_slider_queen);
  tcase_add_test(test_case, ut_slider_rook);
  tcase_add_test(test_case, ut_slider_bishop);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
