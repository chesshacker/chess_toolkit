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
#include "ct_pawn.h"
#include "ct_utilities.h"
#include "ct_square.h"
#include "ct_piece.h"
#include "ct_position.h"
#include "ct_move_command.h"
#include "ct_move.h"
#include <stdlib.h>                /* abs */

typedef struct CtPawnStruct
{
  CtMoveCommand move_command;
  CtPieceColor piece_color;
  int forward_offset;
  int forward_two_offset;
  int diagonal_queenside_offset;
  int diagonal_kingside_offset;
  CtPiece enemy_pawn;
  int starting_rank;
  int second_to_last_rank;
  int capture_en_passant_from_rank;
  int capture_en_passant_to_rank;
} CtPawnStruct;

static void ct_pawn_move_promotion(CtPawn pawn, CtSquare from, CtSquare to);

CtPawn
ct_pawn_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtPawn pawn;

  pawn = ct_malloc(sizeof(CtPawnStruct));
  pawn->move_command = move_command;
  pawn->piece_color = piece_color;
  if (piece_color == WHITE_PIECE)
  {
    pawn->forward_offset = D_N;
    pawn->forward_two_offset = 2 * D_N;
    pawn->diagonal_queenside_offset = D_NW;
    pawn->diagonal_kingside_offset = D_NE;
    pawn->enemy_pawn = BLACK_PAWN;
    pawn->starting_rank = RANK_2;
    pawn->second_to_last_rank = RANK_7;
    pawn->capture_en_passant_from_rank = RANK_5;
    pawn->capture_en_passant_to_rank = RANK_6;
  }
  else
  {
    pawn->forward_offset = D_S;
    pawn->forward_two_offset = 2 * D_S;
    pawn->diagonal_queenside_offset = D_SW;
    pawn->diagonal_kingside_offset = D_SE;
    pawn->enemy_pawn = WHITE_PAWN;
    pawn->starting_rank = RANK_7;
    pawn->second_to_last_rank = RANK_2;
    pawn->capture_en_passant_from_rank = RANK_4;
    pawn->capture_en_passant_to_rank = RANK_3;
  }
  return pawn;
}

void
ct_pawn_free(CtPawn pawn)
{
  ct_free(pawn);
}


void
ct_pawn_move(CtPawn pawn, CtPosition position, CtSquare from)
{
  CtMoveCommand move_command = pawn->move_command;
  CtFile from_file = ct_square_file(from);
  CtRank from_rank = ct_square_rank(from);
  CtFile en_passant_file;
  CtSquare to_forward, to_forward_two, to_queenside, to_kingside, en_passant_square;
  CtPiece to_piece;
  bool able_to_move_forward = false;
  bool able_to_move_forward_two = false;
  bool able_to_capture_queenside = false;
  bool able_to_capture_kingside = false;
  bool en_passant_possible = false;
  bool can_capture_en_passant = false;
  CtMove move;

  to_forward = from + pawn->forward_offset;
  to_piece = ct_position_get_piece(position, to_forward);
  if (to_piece == EMPTY)
    able_to_move_forward = true;

  if (from_file != FILE_A)
  {
    to_queenside = from + pawn->diagonal_queenside_offset;
    to_piece = ct_position_get_piece(position, to_queenside);
    if (to_piece != EMPTY && ct_piece_color(to_piece) != pawn->piece_color)
      able_to_capture_queenside = true;
  }

  if (from_file != LAST_FILE)
  {
    to_kingside = from + pawn->diagonal_kingside_offset;
    to_piece = ct_position_get_piece(position, to_kingside);
    if (to_piece != EMPTY && ct_piece_color(to_piece) != pawn->piece_color)
      able_to_capture_kingside = true;
  }

  if (able_to_move_forward && from_rank == pawn->starting_rank)
  {
    to_forward_two = from + pawn->forward_two_offset;
    to_piece = ct_position_get_piece(position, to_forward_two);
    if (to_piece == EMPTY)
      able_to_move_forward_two = true;
  }

  if (able_to_move_forward_two)
  {
    if (from_file != FILE_A)
      en_passant_possible = ct_position_get_piece(position, to_forward_two + D_W) == pawn->enemy_pawn;
    if (!en_passant_possible && from_file != LAST_FILE)
      en_passant_possible = ct_position_get_piece(position, to_forward_two + D_E) == pawn->enemy_pawn;
  }

  if (from_rank == pawn->capture_en_passant_from_rank)
  {
    en_passant_file = ct_position_get_en_passant(position);
    if (en_passant_file != NO_EN_PASSANT && abs(en_passant_file - from_file) == 1)
      can_capture_en_passant = true;
  }

  if (from_rank == pawn->second_to_last_rank)
  {
    if (able_to_move_forward)
      ct_pawn_move_promotion(pawn, from, to_forward);
    if (able_to_capture_queenside)
      ct_pawn_move_promotion(pawn, from, to_queenside);
    if (able_to_capture_kingside)
      ct_pawn_move_promotion(pawn, from, to_kingside);
  }
  else
  {
    if (able_to_move_forward)
      ct_move_command_execute(move_command, ct_move_make(from, to_forward));
    if (able_to_move_forward_two)
    {
      if (en_passant_possible)
        move = ct_move_make_en_passant_possible(from, to_forward_two);
      else
        move = ct_move_make(from, to_forward_two);
      ct_move_command_execute(move_command, move);
    }
    if (able_to_capture_queenside)
      ct_move_command_execute(move_command, ct_move_make(from, to_queenside));
    if (able_to_capture_kingside)
      ct_move_command_execute(move_command, ct_move_make(from, to_kingside));
    if (can_capture_en_passant)
    {
      en_passant_square = ct_square_make(en_passant_file, pawn->capture_en_passant_to_rank);
      move = ct_move_make_en_passant_capture(from, en_passant_square);
      ct_move_command_execute(move_command, move);
    }
  }
}

static void
ct_pawn_move_promotion(CtPawn pawn, CtSquare from, CtSquare to)
{
  CtMoveCommand move_command = pawn->move_command;

  if (pawn->piece_color == WHITE_PIECE)
  {
    ct_move_command_execute(move_command, ct_move_make_promotion_Q(from, to));
    ct_move_command_execute(move_command, ct_move_make_promotion_R(from, to));
    ct_move_command_execute(move_command, ct_move_make_promotion_B(from, to));
    ct_move_command_execute(move_command, ct_move_make_promotion_N(from, to));
  }
  else
  {
    ct_move_command_execute(move_command, ct_move_make_promotion_q(from, to));
    ct_move_command_execute(move_command, ct_move_make_promotion_r(from, to));
    ct_move_command_execute(move_command, ct_move_make_promotion_b(from, to));
    ct_move_command_execute(move_command, ct_move_make_promotion_n(from, to));
  }
}
