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
#include "ct_square.h"
#include "ct_piece.h"
#include "ct_utilities.h"

static CtPosition default_position;

static void ct_fen_reader_skip_spaces(char **fen_ptr);
static CtPosition ct_fen_reader_try_part_1(CtPosition position, char **fen_ptr);
static CtPosition ct_fen_reader_try_part_2(CtPosition position, char **fen_ptr);
static CtPosition ct_fen_reader_try_part_3(CtPosition position, char **fen_ptr);
static CtPosition ct_fen_reader_try_part_4(CtPosition position, char **fen_ptr);

void
ct_position_from_fen_init(void)
{
  default_position = ct_position_new();
}

 /* ct_position_from_fen can be passed a position to store the result, which must have been previously initialized with
    ct_position_new.  If 0 is passed in for position, a default position will be used.  Use of the default position is
    not thread safe.  Returns a pointer to the resulting position or 0 if there was a syntax error found with the fen. */

CtPosition
ct_position_from_fen(CtPosition position, char *fen)
{
  CtPosition result;

  if (fen == 0)
    return 0;
  if (position == 0)
    position = default_position;
  result = position;
  ct_position_clear(position);
  result = ct_fen_reader_try_part_1(position, &fen);
  if (result)
    result = ct_fen_reader_try_part_2(position, &fen);
  if (result)
    result = ct_fen_reader_try_part_3(position, &fen);
  if (result)
    result = ct_fen_reader_try_part_4(position, &fen);
  if (result == 0)
    ct_position_clear(position);
  return result;
}

static void
ct_fen_reader_skip_spaces(char **fen_ptr)
{
  char *fen = *fen_ptr;

  while (*fen == ' ')
    fen++;
  *fen_ptr = fen;
}

static CtPosition
ct_fen_reader_try_part_1(CtPosition position, char **fen_ptr)
{
  char *fen;
  CtRank rank;
  CtFile file;
  CtPiece piece;

  ct_fen_reader_skip_spaces(fen_ptr);
  fen = *fen_ptr;
  for (rank = LAST_RANK; rank >= RANK_1; rank--)
  {
    file = FILE_A;
    while (file <= LAST_FILE)
    {
      piece = ct_piece_from_char(*fen);
      if (piece)
      {
        ct_position_set_piece(position, ct_square_make(file, rank), piece);
        file++;
      }
      else if (*fen >= '1' && *fen <= '8')
        file += *fen - '0';
      else
        return 0;
      fen++;
    }
    if (file != LAST_FILE + 1)
      return 0;
    if (rank > RANK_1)
    {
      if (*fen != '/')
        return 0;
      fen++;
    }
  }
  if (*fen != ' ')
    return 0;
  *fen_ptr = fen;
  return position;
}

static CtPosition
ct_fen_reader_try_part_2(CtPosition position, char **fen_ptr)
{
  char *fen;
  bool is_wtm = ct_position_is_white_to_move(position);

  ct_fen_reader_skip_spaces(fen_ptr);
  fen = *fen_ptr;
  switch (*fen++)
  {
  case 'w':
    is_wtm = true;
    ct_position_set_white_to_move(position);
    break;
  case 'b':
    is_wtm = false;
    ct_position_set_black_to_move(position);
    break;
  default:
    return 0;
  }
  if (*fen != ' ')
    return 0;
  *fen_ptr = fen;
  return position;
}

/* shorthand to lookup the piece at a given square */
#define PIECE_AT(square) (ct_position_get_piece(position, (square)))

static CtPosition
ct_fen_reader_try_part_3(CtPosition position, char **fen_ptr)
{
  char *fen;
  struct
  {
    CtSquare king_square;
    CtSquare rook_square;
    CtPiece king_piece;
    CtPiece rook_piece;
  } castle_requirements;

  ct_fen_reader_skip_spaces(fen_ptr);
  fen = *fen_ptr;
  if (*fen == '-')
    fen++;
  else
  {
    CtCastleRights new_castle = CASTLE_NONE;
    for (; *fen && *fen != ' '; fen++)
    {
      switch (*fen)
      {
      case 'K':
        new_castle |= CASTLE_K;
        castle_requirements.king_square = E1;
        castle_requirements.rook_square = H1;
        castle_requirements.king_piece = WHITE_KING;
        castle_requirements.rook_piece = WHITE_ROOK;
        break;
      case 'k':
        new_castle |= CASTLE_k;
        castle_requirements.king_square = E8;
        castle_requirements.rook_square = H8;
        castle_requirements.king_piece = BLACK_KING;
        castle_requirements.rook_piece = BLACK_ROOK;
        break;
      case 'Q':
        new_castle |= CASTLE_Q;
        castle_requirements.king_square = E1;
        castle_requirements.rook_square = A1;
        castle_requirements.king_piece = WHITE_KING;
        castle_requirements.rook_piece = WHITE_ROOK;
        break;
      case 'q':
        new_castle |= CASTLE_q;
        castle_requirements.king_square = E8;
        castle_requirements.rook_square = A8;
        castle_requirements.king_piece = BLACK_KING;
        castle_requirements.rook_piece = BLACK_ROOK;
        break;
      default:
        return 0;
      }
      if (PIECE_AT(castle_requirements.king_square) != castle_requirements.king_piece
          || PIECE_AT(castle_requirements.rook_square) != castle_requirements.rook_piece)
        return 0;
    }
    ct_position_set_castle(position, new_castle);
  }
  if (*fen != ' ')
    return 0;
  *fen_ptr = fen;
  return position;
}

static CtPosition
ct_fen_reader_try_part_4(CtPosition position, char **fen_ptr)
{
  char *fen;
  char rank;
  bool is_wtm = ct_position_is_white_to_move(position);

  ct_fen_reader_skip_spaces(fen_ptr);
  fen = *fen_ptr;
  if (*fen == '-')
    fen++;
  else
  {
    if (*fen < 'a' || *fen > 'h')
      return 0;
    /* "white to move" means black just moved, so it's a black pawn */
    rank = is_wtm ? RANK_5 : RANK_4;
    ct_position_set_en_passant(position, ct_file_from_char(*fen));
    fen++;
    if ((is_wtm && *fen != '6') || (!is_wtm && *fen != '3'))
      return 0;
    fen++;
  }
  if (*fen && *fen != ' ')
    return 0;
  *fen_ptr = fen;
  return position;
}
