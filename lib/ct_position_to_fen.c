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
#include "ct_piece.h"
#include "ct_square.h"
#include "ct_utilities.h"
#include "ct_error.h"
#include <string.h>

static char default_destination[CT_FEN_MAX_LENGTH];

char *
ct_position_to_fen(CtPosition position, char *destination)
{
  CtPiece *pieces;
  bool is_wtm;
  CtCastleRights castle;
  int en_passant;

  char *ct;
  CtPiece piece;
  CtRank rank;
  CtFile file;
  CtSquare square;
  int squares_output;

  if (destination == NULL)
    destination = default_destination;
  pieces = position->pieces;
  is_wtm = ct_position_is_white_to_move(position);
  castle = ct_position_get_castle(position);
  en_passant = ct_position_get_en_passant(position);
  ct = destination;

  for (rank = LAST_RANK; rank >= RANK_1; rank--)
  {
    for (file = FILE_A; file <= LAST_RANK; file += squares_output)
    {
      squares_output = 1;
      square = ct_square_make(file, rank);
      piece = pieces[square];
      if (piece != EMPTY)
        *ct++ = ct_piece_to_char(piece);
      else
      {
        while (file + squares_output <= LAST_FILE
               && pieces[square + squares_output] == EMPTY)
          squares_output++;
        *ct++ = '0' + squares_output;
      }
    }
    *ct++ = '/';
  }

  if (is_wtm)
    ct = stpcpy(ct - 1, " w ");
  else
    ct = stpcpy(ct - 1, " b ");
  if (castle == 0)
  {
    *ct++ = '-';
  }
  else
  {
    if (castle & CASTLE_K)
      *ct++ = 'K';
    if (castle & CASTLE_Q)
      *ct++ = 'Q';
    if (castle & CASTLE_k)
      *ct++ = 'k';
    if (castle & CASTLE_q)
      *ct++ = 'q';
  }
  *ct++ = ' ';
  if (en_passant == NO_EN_PASSANT)
  {
    *ct++ = '-';
  }
  else
  {
    *ct++ = ct_file_to_char(en_passant);
    *ct++ = ct_rank_to_char(is_wtm ? RANK_6 : RANK_3);
  }

  *ct = '\0';
  if (ct - destination >= CT_FEN_MAX_LENGTH)
    ct_error("ct_position_to_fen: CT_FEN_MAX_LENGTH is not big enough");
  return destination;
}
