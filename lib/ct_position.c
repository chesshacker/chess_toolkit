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
#include "ct_position.h"
#include "ct_position_private.h"
#include "ct_square.h"
#include "ct_bit_board.h"
#include "ct_piece_command.h"
#include "ct_piece.h"
#include "ct_utilities.h"
#include <string.h>

static CtPosition clear_position = 0;
static CtPosition initial_position = 0;

void
ct_position_init(void)
{
  CtSquare square;
  CtPiece rank_1[] = {WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK};
  CtPiece rank_8[] = {BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK};
  CtFile file;

  if (clear_position)
    ct_position_free(clear_position);
  clear_position = ct_malloc(sizeof(CtPositionStruct));
  ct_bit_board_array_reset(clear_position->bit_board_array);
  for (square = A1; square < NUMBER_OF_SQUARES; square++)
    clear_position->pieces[square] = EMPTY;
  ct_position_set_white_to_move(clear_position);
  ct_position_clear_en_passant(clear_position);
  ct_position_set_castle(clear_position, CASTLE_NONE);

  if (initial_position)
    ct_position_free(initial_position);
  initial_position = ct_malloc(sizeof(CtPositionStruct));
  ct_bit_board_array_reset(initial_position->bit_board_array);
  ct_position_copy(initial_position, clear_position);
  for (file = FILE_A; file <= LAST_FILE; file++)
  {
    ct_position_set_piece(initial_position, ct_square_make(file, RANK_1), rank_1[file]);
    ct_position_set_piece(initial_position, ct_square_make(file, RANK_2), WHITE_PAWN);
    ct_position_set_piece(initial_position, ct_square_make(file, RANK_7), BLACK_PAWN);
    ct_position_set_piece(initial_position, ct_square_make(file, RANK_8), rank_8[file]);
  }
  ct_position_set_castle(initial_position, CASTLE_KQkq);
}

CtPosition
ct_position_new(void)
{
  CtPosition position;

  position = ct_malloc(sizeof(CtPositionStruct));
  ct_position_reset(position);
  return position;
}

void
ct_position_free(CtPosition position)
{
  ct_free(position);
}

void
ct_position_clear(CtPosition position)
{
  ct_position_copy(position, clear_position);
}

void
ct_position_reset(CtPosition position)
{
  ct_position_copy(position, initial_position);
}

void
ct_position_copy(CtPosition destination, CtPosition source)
{
  memcpy(destination, source, sizeof(CtPositionStruct));
}

CtPiece
ct_position_get_piece(CtPosition position, CtSquare square)
{
  return position->pieces[square];
}

void
ct_position_set_piece(CtPosition position, CtSquare square, CtPiece value)
{
  CtBitBoard square_bb = ct_bit_board_make(square);
  CtPiece old_value = position->pieces[square];
  CtBitBoard *old_bb = &position->bit_board_array[old_value];
  CtBitBoard *new_bb = &position->bit_board_array[value];

  *old_bb &= ~square_bb;
  *new_bb |= square_bb;
  position->pieces[square] = value;
}

void
ct_position_for_each_piece(CtPosition position, CtPieceCommand piece_command)
{
  CtPiece *piece = position->pieces;
  CtSquare square;

  for (square = 0; square < NUMBER_OF_SQUARES; square++, piece++)
    if (*piece != EMPTY)
      ct_piece_command_execute(piece_command, *piece, square);
}

void
ct_position_for_each_active_piece(CtPosition position, CtPieceCommand piece_command)
{
  bool is_wtm = position->is_white_to_move;
  CtPieceColor piece_color = is_wtm ? WHITE_PIECE : BLACK_PIECE;
  CtPiece *piece = position->pieces;
  CtSquare square;

  for (square = 0; square < NUMBER_OF_SQUARES; square++, piece++)
    if (*piece != EMPTY && ct_piece_color(*piece) == piece_color)
      ct_piece_command_execute(piece_command, *piece, square);
}

CtBitBoard
ct_position_get_bit_board(CtPosition position, CtPiece piece)
{
  return position->bit_board_array[piece & PIECE_MASK_VALID_BITS];
}

bool
ct_position_is_white_to_move(CtPosition position)
{
  return position->is_white_to_move;
}

void
ct_position_set_white_to_move(CtPosition position)
{
  position->is_white_to_move = true;
}

void
ct_position_set_black_to_move(CtPosition position)
{
  position->is_white_to_move = false;
}

void
ct_position_change_turns(CtPosition position)
{
  position->is_white_to_move ^= 1;
}

CtFile
ct_position_get_en_passant(CtPosition position)
{
  return position->en_passant;
}

void
ct_position_set_en_passant(CtPosition position, CtFile file)
{
  position->en_passant = file & 7;
}

void
ct_position_clear_en_passant(CtPosition position)
{
  position->en_passant = NO_EN_PASSANT;
}

CtCastleRights
ct_position_get_castle(CtPosition position)
{
  return position->castle;
}

void
ct_position_set_castle(CtPosition position, CtCastleRights castle_rights)
{
  position->castle = castle_rights;
}
