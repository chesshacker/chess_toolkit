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
#include "ct_bit_board.h"
#include "ct_utilities.h"

typedef struct CanCastleHelperStruct *CanCastleHelper;
typedef struct CanCastleHelperStruct
{
  CtCastleRights castle_kingside_bit;
  CtCastleRights castle_queenside_bit;
  CtSquare king_square;
  bool (*is_attacking) (CtBitBoardArray bit_board_array, CtSquare square);
  bool (*is_attacking_between) (CtPosition position, CtSquare from, CtSquare to);
} CanCastleHelperStruct;

static bool ct_position_is_white_king_attacked(CtPosition position);
static bool ct_position_is_black_king_attacked(CtPosition position);
static bool ct_position_is_white_attacking_between(CtPosition position, CtSquare from, CtSquare to);
static bool ct_position_is_black_attacking_between(CtPosition position, CtSquare from, CtSquare to);

static CanCastleHelperStruct white_castle_helper[] = {{
    CASTLE_K, CASTLE_Q, E1, ct_bit_board_array_is_black_attacking, ct_position_is_black_attacking_between
}};

static CanCastleHelperStruct black_castle_helper[] = {{
    CASTLE_k, CASTLE_q, E8, ct_bit_board_array_is_white_attacking, ct_position_is_white_attacking_between
}};

bool
ct_position_is_legal(CtPosition position)
{
  bool result;

  if (position->is_white_to_move)
    result = !ct_position_is_black_king_attacked(position);
  else
    result = !ct_position_is_white_king_attacked(position);
  return result;
}

bool
ct_position_is_check(CtPosition position)
{
  bool result;

  if (position->is_white_to_move)
    result = ct_position_is_white_king_attacked(position);
  else
    result = ct_position_is_black_king_attacked(position);
  return result;
}

static bool
ct_position_is_white_king_attacked(CtPosition position)
{
  bool result = false;
  CtBitBoardArray bit_board_array = position->bit_board_array;
  CtSquare square = ct_bit_board_find_first_square(bit_board_array[WHITE_KING]);

  if (square >= 0)
    result = ct_bit_board_array_is_black_attacking(bit_board_array, square);
  return result;
}

static bool
ct_position_is_black_king_attacked(CtPosition position)
{
  bool result = false;
  CtBitBoardArray bit_board_array = position->bit_board_array;
  CtSquare square = ct_bit_board_find_first_square(bit_board_array[BLACK_KING]);

  if (square >= 0)
    result = ct_bit_board_array_is_white_attacking(bit_board_array, square);
  return result;
}

CtCastleRights
ct_position_can_castle(CtPosition position)
{
  CtCastleRights result = CASTLE_NONE;
  CtCastleRights castle = position->castle;
  CtPiece *pieces = position->pieces;
  bool verified_king_not_in_check = false;
  CanCastleHelper helper = position->is_white_to_move ? white_castle_helper : black_castle_helper;
  CtSquare king_square = helper->king_square;
  CtSquare verify_to;

  if (castle == CASTLE_NONE)
    return result;

  if ((castle & helper->castle_kingside_bit) != 0
      && pieces[king_square + D_E] == EMPTY
      && pieces[king_square + 2 * D_E] == EMPTY)
  {
    if (helper->is_attacking(position->bit_board_array, king_square))
      return result;
    verified_king_not_in_check = true;
    if (!helper->is_attacking_between(position, king_square + D_E, king_square + 2 * D_E))
      result |= CASTLE_K;
  }

  if ((castle & helper->castle_queenside_bit) != 0
      && pieces[king_square + D_W] == EMPTY
      && pieces[king_square + 2 * D_W] == EMPTY
      && pieces[king_square + 3 * D_W] == EMPTY)
  {
    verify_to = verified_king_not_in_check ? king_square + D_W : king_square;
    if (!helper->is_attacking_between(position, king_square + 2 * D_W, verify_to))
      result |= CASTLE_Q;
  }
  return result;
}

static bool
ct_position_is_white_attacking_between(CtPosition position, CtSquare from, CtSquare to)
{
  bool result = false;
  CtSquare square;

  for (square = from; result == false && square <= to; square++)
    result = ct_bit_board_array_is_white_attacking(position->bit_board_array, square);
  return result;
}

static bool
ct_position_is_black_attacking_between(CtPosition position, CtSquare from, CtSquare to)
{
  bool result = false;
  CtSquare square;

  for (square = from; result == false && square <= to; square++)
    result = ct_bit_board_array_is_black_attacking(position->bit_board_array, square);
  return result;
}
