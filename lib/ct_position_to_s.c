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
#include "ct_utilities.h"
#include "ct_square.h"
#include "ct_piece.h"
#include "ct_error.h"
#include <string.h>
#include <stdio.h>

static char default_destination[CT_POSITION_TO_S_MAX_LENGTH];

char *
ct_position_to_s(CtPosition position, char *destination)
{
  CtPiece *pieces;
  bool is_wtm;
  CtCastleRights castle;
  int en_passant;

  char *cf, *ct;
  CtSquare square;
  char piece_chars[NUMBER_OF_SQUARES];
  CtRank rank;
  CtFile file;

  if (destination == NULL)
    destination = default_destination;
  pieces = position->pieces;
  is_wtm = ct_position_is_white_to_move(position);
  castle = ct_position_get_castle(position);
  en_passant = ct_position_get_en_passant(position);
  ct = destination;

  for (square = 0; square < NUMBER_OF_SQUARES; square++)
    piece_chars[square] = ct_piece_to_char(pieces[square]);
  cf = piece_chars;
  for (rank = LAST_RANK; rank >= RANK_1; rank--)
  {
    *ct++ = ' ';
    *ct++ = ct_rank_to_char(rank);
    *ct++ = ' ';
    *ct++ = ' ';
    for (file = FILE_A; file <= LAST_FILE; file++)
    {
      *ct++ = cf[ct_square_make(file, rank)];
      *ct++ = ' ';
    }
    *(ct - 1) = '\n';
  }
  ct = stpcpy(ct, "   ");
  for (file = FILE_A; file <= LAST_FILE; file++)
  {
    *ct++ = ' ';
    *ct++ = ct_file_to_char(file);
  }
  *ct++ = '\n';
  if (is_wtm)
    ct = stpcpy(ct, "White ");
  else
    ct = stpcpy(ct, "Black ");
  if (castle == 0)
    *ct++ = '-';
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
    *ct++ = '-';
  else
  {
    *ct++ = ct_file_to_char(en_passant);
    *ct++ = ct_rank_to_char(is_wtm ? RANK_6 : RANK_3);
  }

  *ct = '\0';
  if (ct - destination >= CT_POSITION_TO_S_MAX_LENGTH)
    ct_error("ct_position_to_s: CT_POSITION_TO_S_MAX_LENGTH is not big enough");
  return destination;
}
