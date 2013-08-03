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
#include "ct_move_generator.h"
#include "ct_utilities.h"
#include "ct_piece.h"
#include "ct_piece_command.h"
#include "ct_move_command.h"
#include "ct_position.h"
#include "ct_move.h"
#include "ct_pawn.h"
#include "ct_steper.h"
#include "ct_slider.h"

typedef struct CtPieceMoversStruct *CtPieceMovers;
typedef struct CtPieceMoversStruct
{
  CtPosition position;
  CtPawn pawn;
  CtSteper stepers[2];
  CtSlider sliders[3];
} CtPieceMoversStruct;

typedef struct CtMoveGeneratorStruct
{
  CtPosition position;
  CtMoveCommand pseudo_legal_move_command;
  CtPieceMovers white_piece_mgs;
  CtPieceMovers black_piece_mgs;
} CtMoveGeneratorStruct;

static CtPieceMovers ct_piece_mgs_new(CtPosition position, CtMoveCommand pseudo_legal_move_command, CtPieceColor piece_color);
static void ct_piece_mgs_free(CtPieceMovers piece_mgs);
static void ct_move_generator_pseudo_legal_piece_moves_execute(void *delegate, CtPiece piece, CtSquare square);

CtMoveGenerator
ct_move_generator_new(CtPosition position, CtMoveCommand pseudo_legal_move_command)
{
  CtMoveGenerator move_generator = ct_malloc(sizeof(CtMoveGeneratorStruct));

  move_generator->position = position;
  move_generator->pseudo_legal_move_command = pseudo_legal_move_command;
  move_generator->white_piece_mgs = ct_piece_mgs_new(position, pseudo_legal_move_command, WHITE_PIECE);
  move_generator->black_piece_mgs = ct_piece_mgs_new(position, pseudo_legal_move_command, BLACK_PIECE);
  return move_generator;
}

void
ct_move_generator_free(CtMoveGenerator move_generator)
{
  ct_piece_mgs_free(move_generator->white_piece_mgs);
  ct_piece_mgs_free(move_generator->black_piece_mgs);
  ct_free(move_generator);
}

static CtPieceMovers
ct_piece_mgs_new(CtPosition position, CtMoveCommand pseudo_legal_move_command, CtPieceColor piece_color)
{
  CtPieceMovers piece_mgs = ct_malloc(sizeof(CtPieceMoversStruct));

  piece_mgs->position = position;
  piece_mgs->pawn = ct_pawn_new(pseudo_legal_move_command, piece_color);
  piece_mgs->stepers[0] = ct_king_new(pseudo_legal_move_command, piece_color);
  piece_mgs->stepers[1] = ct_knight_new(pseudo_legal_move_command, piece_color);
  piece_mgs->sliders[0] = ct_queen_new(pseudo_legal_move_command, piece_color);
  piece_mgs->sliders[1] = ct_rook_new(pseudo_legal_move_command, piece_color);
  piece_mgs->sliders[2] = ct_bishop_new(pseudo_legal_move_command, piece_color);
  return piece_mgs;
}

static void
ct_piece_mgs_free(CtPieceMovers piece_mgs)
{
  ct_pawn_free(piece_mgs->pawn);
  ct_steper_free(piece_mgs->stepers[0]);
  ct_steper_free(piece_mgs->stepers[1]);
  ct_slider_free(piece_mgs->sliders[0]);
  ct_slider_free(piece_mgs->sliders[1]);
  ct_slider_free(piece_mgs->sliders[2]);
  ct_free(piece_mgs);
}

void
ct_move_generator_pseudo_legal_piece_moves(CtMoveGenerator move_generator)
{
  CtPosition position = move_generator->position;
  bool is_wtm = ct_position_is_white_to_move(position);
  CtPieceMovers piece_mgs = is_wtm ? move_generator->white_piece_mgs : move_generator->black_piece_mgs;
  CtPieceCommandStruct piece_command_struct = ct_piece_command_make(piece_mgs, ct_move_generator_pseudo_legal_piece_moves_execute);

  ct_position_for_each_active_piece(position, &piece_command_struct);
}

static void
ct_move_generator_pseudo_legal_piece_moves_execute(void *delegate, CtPiece piece, CtSquare square)
{
  CtPieceMovers piece_mgs = (CtPieceMovers) delegate;

  if (piece & SLIDER_BIT)
    ct_slider_move(piece_mgs->sliders[piece & SLIDER_MASK], piece_mgs->position, square);
  else if (piece & STEPER_BIT)
    ct_steper_move(piece_mgs->stepers[piece & STEPER_MASK], piece_mgs->position, square);
  else
    ct_pawn_move(piece_mgs->pawn, piece_mgs->position, square);
}

void
ct_move_generator_castle_moves(CtMoveGenerator move_generator, CtMoveCommand legal_move_command)
{
  CtPosition position = move_generator->position;
  CtCastleRights castle = ct_position_can_castle(position);
  CtMove move;
  CtSquare king_square;

  if (castle == CASTLE_NONE)
    return;
  king_square = ct_position_is_white_to_move(position) ? E1 : E8;
  if (castle & CASTLE_K)
  {
    move = ct_move_make_castle_kingside(king_square);
    ct_move_command_execute(legal_move_command, move);
  }
  if (castle & CASTLE_Q)
  {
    move = ct_move_make_castle_queenside(king_square);
    ct_move_command_execute(legal_move_command, move);
  }
}
