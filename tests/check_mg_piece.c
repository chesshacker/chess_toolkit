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
#include "check_mg_piece.h"
#include "chess_toolkit.h"
#include "ct_steper.h"
#include "ct_slider.h"
#include "ct_pawn.h"
#include <stdio.h>

static CheckMgPieceTest current_test;
static CtSquare *next_answer;
static CtMoveCommand move_command;

static void test_move_command(void *ignore, CtMove move);

void
check_mg_piece_setup(void)
{
  move_command = ct_move_command_new(0, test_move_command);
}

void
check_mg_piece_teardown(void)
{
  ct_move_command_free(move_command);
}

CtMoveCommand
check_mg_piece_move_command(void)
{
  return move_command;
}

void
check_mg_piece_run_all_tests(void *white_mg_piece, void *black_mg_piece, CheckMgPieceTest tests)
{
  char *fen;
  CtPosition position;
  CtSquare from;
  CtPiece piece;
  CtPieceColor color, expected_color;
  void *mg_piece;

  for (current_test = tests; !current_test->end_of_tests; current_test++)
  {
    next_answer = current_test->answers;
    fen = current_test->fen;
    position = ct_position_from_fen(0, fen);
    ck_assert_msg(position != 0, fen);
    from = current_test->from;
    piece = ct_position_get_piece(position, from);
    color = ct_piece_color(piece);
    expected_color = ct_position_is_white_to_move(position) ? WHITE_PIECE : BLACK_PIECE;
    ck_assert_msg(color == expected_color, fen);
    mg_piece = color == WHITE_PIECE ? white_mg_piece : black_mg_piece;

    ck_assert_msg(piece != EMPTY, fen);
    if (piece & SLIDER_BIT)
      ct_slider_move((CtSlider) mg_piece, position, from);
    else if (piece & STEPER_BIT)
      ct_steper_move((CtSteper) mg_piece, position, from);
    else
      ct_pawn_move((CtPawn) mg_piece, position, from);

    ck_assert_msg(*next_answer == SQUARE_NOT_FOUND, fen);
  }
}

static void
test_move_command(void *ignore, CtMove move)
{
  CtSquare to = ct_move_to(move);
  char *fen = current_test->fen;

  ck_assert_msg(ct_move_from(move) == current_test->from, fen);
  ck_assert_msg(ct_move_type(move) == NORMAL_MOVE, fen);
  if (to != *next_answer)
  {
    char msg[CT_FEN_MAX_LENGTH + 100];

    snprintf(msg, sizeof(msg), "Failed fen=%s, from=%d *next_answer=%d, to=%d",
             fen, current_test->from, *next_answer, to);
    ck_abort_msg(msg);
  }
  next_answer++;
}
