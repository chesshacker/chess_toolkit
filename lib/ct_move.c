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
#include "ct_move.h"

static const CtPiece promotes_to[] = {
  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  WHITE_QUEEN, WHITE_ROOK, WHITE_BISHOP, WHITE_KNIGHT,
  BLACK_QUEEN, BLACK_ROOK, BLACK_BISHOP, BLACK_KNIGHT
};

enum
{
  MOVE_TYPE_SHIFT = 12,
  PROMOTION_BIT = 0x8000,
  MOVE_TYPE_MASK = 0xF000
};

enum
{
  PROMOTES_TO_WHITE_QUEEN = 0x8000,
  PROMOTES_TO_WHITE_ROOK = 0x9000,
  PROMOTES_TO_WHITE_BISHOP = 0xA000,
  PROMOTES_TO_WHITE_KNIGHT = 0xB000,
  PROMOTES_TO_BLACK_QUEEN = 0xC000,
  PROMOTES_TO_BLACK_ROOK = 0xD000,
  PROMOTES_TO_BLACK_BISHOP = 0xE000,
  PROMOTES_TO_BLACK_KNIGHT = 0xF000
};

#define MAKE_MOVE_TYPE_MOVE(type, from, to) (((type) << 12) | ((from) << 6) | (to))
#define MAKE_PROMOTION_MOVE(promotes_to, from, to) ((promotes_to) | ((from) << 6) | (to))

CtMove
ct_move_make(CtSquare from, CtSquare to)
{
  return (from << 6) | to;
}

CtMove
ct_move_make_castle_kingside(CtSquare from)
{
  return MAKE_MOVE_TYPE_MOVE(CASTLE_KINGSIDE, from, from + 2 * D_E);
  /* To also work with Chess960, could use "(from & 070) | FILE_G" instead of "from + 2" */
}

CtMove
ct_move_make_castle_queenside(CtSquare from)
{
  return MAKE_MOVE_TYPE_MOVE(CASTLE_QUEENSIDE, from, from + 2 * D_W);
}

CtMove
ct_move_make_en_passant_possible(CtSquare from, CtSquare to)
{
  return MAKE_MOVE_TYPE_MOVE(EN_PASSANT_POSSIBLE, from, to);
}

CtMove
ct_move_make_en_passant_capture(CtSquare from, CtSquare to)
{
  return MAKE_MOVE_TYPE_MOVE(EN_PASSANT_CAPTURE, from, to);
}

CtMove
ct_move_make_promotion_Q(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_WHITE_QUEEN, from, to);
}

CtMove
ct_move_make_promotion_R(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_WHITE_ROOK, from, to);
}

CtMove
ct_move_make_promotion_B(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_WHITE_BISHOP, from, to);
}

CtMove
ct_move_make_promotion_N(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_WHITE_KNIGHT, from, to);
}

CtMove
ct_move_make_promotion_q(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_BLACK_QUEEN, from, to);
}

CtMove
ct_move_make_promotion_r(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_BLACK_ROOK, from, to);
}

CtMove
ct_move_make_promotion_b(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_BLACK_BISHOP, from, to);
}

CtMove
ct_move_make_promotion_n(CtSquare from, CtSquare to)
{
  return MAKE_PROMOTION_MOVE(PROMOTES_TO_BLACK_KNIGHT, from, to);
}

CtMoveType
ct_move_type(CtMove move)
{
  if (move & PROMOTION_BIT)
    return PROMOTION;
  return (move & MOVE_TYPE_MASK) >> MOVE_TYPE_SHIFT;
}

CtSquare
ct_move_from(CtMove move)
{
  return (move & 07700) >> 6;
}

CtSquare
ct_move_to(CtMove move)
{
  return move & 077;
}

CtPiece
ct_move_promotes_to(CtMove move)
{
  return promotes_to[(move & MOVE_TYPE_MASK) >> MOVE_TYPE_SHIFT];
}
