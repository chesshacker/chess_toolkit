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
#include "ct_move_maker.h"
#include "ct_utilities.h"
#include "ct_position.h"
#include "ct_undo_position.h"
#include "ct_square.h"
#include "ct_move.h"

typedef struct CtMoveMakerStruct
{
  CtPosition position;
  CtUndoPosition undo_position;
} CtMoveMakerStruct;

static CtCastleRights castle_mask[64];

void
ct_move_maker_init(void)
{
  CtSquare square;

  for (square = A1; square < NUMBER_OF_SQUARES; square++)
    castle_mask[square] = CASTLE_NONE;
  castle_mask[A1] = CASTLE_Q;
  castle_mask[E1] = CASTLE_KQ;
  castle_mask[H1] = CASTLE_K;
  castle_mask[A8] = CASTLE_q;
  castle_mask[E8] = CASTLE_kq;
  castle_mask[H8] = CASTLE_k;
}

CtMoveMaker
ct_move_maker_new(CtPosition position)
{
  CtMoveMaker move_maker;

  move_maker = ct_malloc(sizeof(CtMoveMakerStruct));
  move_maker->position = position;
  move_maker->undo_position = ct_undo_position_new(position);
  return move_maker;
}

void
ct_move_maker_free(CtMoveMaker move_maker)
{
  ct_undo_position_free(move_maker->undo_position);
  ct_free(move_maker);
}

void
ct_move_maker_reset(CtMoveMaker move_maker)
{
  ct_undo_position_reset(move_maker->undo_position);
}

void
ct_move_maker_make(CtMoveMaker move_maker, CtMove move)
{
  CtSquare from, to, square;
  CtUndoPosition undo_position = move_maker->undo_position;
  CtPosition position = move_maker->position;
  CtCastleRights castle;

  if (move == NULL_MOVE)
    return;                        /* ignore null moves */
  ct_undo_position_start(undo_position);
  from = ct_move_from(move);
  to = ct_move_to(move);

  switch (ct_move_type(move))
  {
    /* For castling, move the rook too */
  case CASTLE_KINGSIDE:
    ct_undo_position_move_piece(undo_position, from + 3, from + 1);
    break;
  case CASTLE_QUEENSIDE:
    ct_undo_position_move_piece(undo_position, from - 4, from - 1);
    break;
  case EN_PASSANT_POSSIBLE:
    ct_undo_position_set_en_passant(undo_position, to);
    break;
  case EN_PASSANT_CAPTURE:
    square = ct_square_make(ct_square_file(to), ct_square_rank(from));
    ct_undo_position_set_piece(undo_position, square, EMPTY);
    break;
  case PROMOTION:
    ct_undo_position_set_piece(undo_position, from, ct_move_promotes_to(move));
    break;
  }
  ct_undo_position_move_piece(undo_position, from, to);

  castle = ct_position_get_castle(position);
  if (castle & (castle_mask[from] | castle_mask[to]))
    ct_undo_position_set_castle(undo_position, castle & ~(castle_mask[from] | castle_mask[to]));
}

void
ct_move_maker_unmake(CtMoveMaker move_maker)
{
  ct_undo_position_undo(move_maker->undo_position);
}
