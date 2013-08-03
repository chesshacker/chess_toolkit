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

typedef struct UtPieceCommandTestStruct *UtPieceCommandTest;
typedef struct UtPieceCommandTestStruct
{
  CtPiece piece;
  CtSquare square;
} UtPieceCommandTestStruct;

void ut_piece_command_test(void *delegate, CtPiece piece, CtSquare square);

START_TEST(ut_piece_command_execute)
{
  UtPieceCommandTestStruct test_struct;
  CtPieceCommandStruct piece_command_struct = ct_piece_command_make(&test_struct, ut_piece_command_test);
  CtPieceCommand piece_command = ct_piece_command_new(&test_struct, ut_piece_command_test);

  ct_piece_command_execute(&piece_command_struct, WHITE_KNIGHT, F3);
  ck_assert_int_eq(test_struct.piece, WHITE_KNIGHT);
  ck_assert_int_eq(test_struct.square, F3);

  ct_piece_command_execute(&piece_command_struct, BLACK_ROOK, D7);
  ck_assert_int_eq(test_struct.piece, BLACK_ROOK);
  ck_assert_int_eq(test_struct.square, D7);

  ct_piece_command_free(piece_command);
} END_TEST

void
ut_piece_command_test(void *delegate, CtPiece piece, CtSquare square)
{
  UtPieceCommandTest test = (UtPieceCommandTest) delegate;

  test->piece = piece;
  test->square = square;
}

Suite *
ut_piece_command_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_piece_command");
  test_case = tcase_create("PieceCommand");
  tcase_add_test(test_case, ut_piece_command_execute);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
