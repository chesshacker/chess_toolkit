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
#include "ct_piece.h"
#include <string.h>

static CtPiece char_to_piece[256];
static char piece_to_char[PIECE_MAX_VALUE + 1];

void
ct_piece_init(void)
{
  memset(char_to_piece, 0, sizeof(char_to_piece));
  char_to_piece['K'] = WHITE_KING;
  char_to_piece['Q'] = WHITE_QUEEN;
  char_to_piece['R'] = WHITE_ROOK;
  char_to_piece['B'] = WHITE_BISHOP;
  char_to_piece['N'] = WHITE_KNIGHT;
  char_to_piece['P'] = WHITE_PAWN;
  char_to_piece['k'] = BLACK_KING;
  char_to_piece['q'] = BLACK_QUEEN;
  char_to_piece['r'] = BLACK_ROOK;
  char_to_piece['b'] = BLACK_BISHOP;
  char_to_piece['n'] = BLACK_KNIGHT;
  char_to_piece['p'] = BLACK_PAWN;
  memset(piece_to_char, 0, sizeof(piece_to_char));
  piece_to_char[EMPTY] = '-';
  piece_to_char[WHITE_KING] = 'K';
  piece_to_char[WHITE_QUEEN] = 'Q';
  piece_to_char[WHITE_ROOK] = 'R';
  piece_to_char[WHITE_BISHOP] = 'B';
  piece_to_char[WHITE_KNIGHT] = 'N';
  piece_to_char[WHITE_PAWN] = 'P';
  piece_to_char[BLACK_KING] = 'k';
  piece_to_char[BLACK_QUEEN] = 'q';
  piece_to_char[BLACK_ROOK] = 'r';
  piece_to_char[BLACK_BISHOP] = 'b';
  piece_to_char[BLACK_KNIGHT] = 'n';
  piece_to_char[BLACK_PAWN] = 'p';
}

CtPieceColor
ct_piece_color(CtPiece piece)
{
  return piece & COLOR_BIT;
}


CtPiece
ct_piece_from_char(char piece_char)
{
  return char_to_piece[piece_char];
}

char
ct_piece_to_char(CtPiece piece)
{
  if (piece & ~PIECE_MASK_VALID_BITS)
    return 0;
  return piece_to_char[piece];
}

CtPiece
ct_piece_to_white(CtPiece piece)
{
  return piece & ~COLOR_BIT;
}

CtPiece
ct_piece_to_black(CtPiece piece)
{
  return piece == EMPTY ? EMPTY : (piece | COLOR_BIT);
}

bool
ct_piece_is_pawn(CtPiece piece)
{
  return ct_piece_to_white(piece) == WHITE_PAWN;
}
