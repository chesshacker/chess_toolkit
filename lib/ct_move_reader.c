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
#include "ct_graph.h"
#include "ct_graph_private.h"
#include "ct_utilities.h"
#include "ct_move.h"
#include "ct_square.h"
#include "ct_move_command.h"
#include "ct_position.h"
#include "ct_piece.h"
#include <string.h>

enum
{
  COMPARE_MOVE_TYPE = 1, COMPARE_PIECE = 2, COMPARE_MOVE_TO = 4, COMPARE_MOVE_FROM_FILE = 8,
  COMPARE_MOVE_FROM_RANK = 0x10, COMPARE_MOVE_PROMOTES_TO = 0x20
};

typedef struct CtMoveReaderStruct *CtMoveReader;
typedef struct CtMoveReaderStruct
{
  CtGraph graph;
  CtPosition position;
  CtSquare move_to;
  CtPiece piece;
  CtMoveType move_type;
  CtFile move_from_file;
  CtRank move_from_rank;
  CtPiece move_promotes_to;
  CtMove result;
  CtMoveCommandStruct consider_move;
  int compare_mask;
} CtMoveReaderStruct;

static void ct_move_reader_init(CtMoveReader move_reader, CtGraph graph);
static void ct_move_reader_consider_move(void *delegate, CtMove move);

CtMove
ct_graph_move_from_san(CtGraph graph, char *notation)
{
  CtMoveReaderStruct move_reader_struct;
  CtMoveReader move_reader = &move_reader_struct;
  bool is_wtm;

  ct_move_reader_init(move_reader, graph);
  is_wtm = ct_position_is_white_to_move(move_reader->position);
  move_reader->compare_mask = 0;
  if (*notation == 'O' || *notation == '0')
  {
    if (strncmp(notation, "O-O-O", 5) == 0 || strncmp(notation, "0-0-0", 5) == 0)
    {
      move_reader->move_type = CASTLE_QUEENSIDE;
      move_reader->compare_mask |= COMPARE_MOVE_TYPE;
      notation += 5;
    }
    else if (strncmp(notation, "O-O", 3) == 0 || strncmp(notation, "0-0", 3) == 0)
    {
      move_reader->move_type = CASTLE_KINGSIDE;
      move_reader->compare_mask |= COMPARE_MOVE_TYPE;
      notation += 3;
    }
  }
  else
  {
    CtPiece piece;
    CtFile to_file;
    CtRank to_rank;
    bool has_rank, has_file, requires_to;

    if (*notation >= 'B' && *notation <= 'R')
    {
      piece = ct_piece_from_char(*notation);
      if (piece == EMPTY)
        return NULL_MOVE;
      else
      {
        notation++;
        if (!is_wtm)
          piece = ct_piece_to_black(piece);
      }
    }
    else
      piece = is_wtm ? WHITE_PAWN : BLACK_PAWN;
    move_reader->piece = piece;
    move_reader->compare_mask |= COMPARE_PIECE;

    has_rank = has_file = requires_to = false;
    if (*notation >= 'a' && *notation <= 'h')
    {
      to_file = ct_file_from_char(*notation++);
      has_file = true;
    }
    if (*notation >= '1' && *notation <= '8')
    {
      to_rank = ct_rank_from_char(*notation++);
      has_rank = true;
    }

    if (*notation == 'x' || *notation == '-')
    {
      notation++;
      requires_to = true;
    }

    if (*notation >= 'a' && *notation <= 'h')
    {
      if (has_file)
      {
        move_reader->move_from_file = to_file;
        move_reader->compare_mask |= COMPARE_MOVE_FROM_FILE;
      }
      to_file = ct_file_from_char(*notation++);
      has_file = true;
      requires_to = true;
    }
    else if (requires_to)
      return NULL_MOVE;

    if (*notation >= '1' && *notation <= '8')
    {
      if (has_rank)
      {
        move_reader->move_from_rank = to_rank;
        move_reader->compare_mask |= COMPARE_MOVE_FROM_RANK;
      }
      to_rank = ct_rank_from_char(*notation++);
      has_rank = true;
    }
    else if (requires_to)
      return NULL_MOVE;

    if (!has_file || !has_rank)
      return NULL_MOVE;
    move_reader->move_to = ct_square_make(to_file, to_rank);
    move_reader->compare_mask |= COMPARE_MOVE_TO;

    if (*notation == '=')
    {
      CtPiece move_promotes_to;
      notation++;                /* eats '=' */

      move_promotes_to = ct_piece_from_char(*notation++);
      if (move_promotes_to == EMPTY)
        return NULL_MOVE;
      if (!is_wtm)
        move_promotes_to = ct_piece_to_black(move_promotes_to);
      move_reader->move_type = PROMOTION;
      move_reader->compare_mask |= COMPARE_MOVE_TYPE;
      move_reader->move_promotes_to = move_promotes_to;
      move_reader->compare_mask |= COMPARE_MOVE_PROMOTES_TO;
    }
  }
  while (*notation && (*notation == '+' || *notation == '#' || *notation == '!' || *notation == '?'))
    notation++;
  if (*notation != 0)
    return NULL_MOVE;

  /* x, + and # could be verified, but other parts of the notation should make the move clear */

  move_reader->result = NULL_MOVE;
  ct_graph_for_each_legal_move(move_reader->graph, &move_reader->consider_move);

  return move_reader->result;
}

static void
ct_move_reader_init(CtMoveReader move_reader, CtGraph graph)
{
  move_reader->graph = graph;
  move_reader->position = graph->position;
  move_reader->consider_move = ct_move_command_make(move_reader, ct_move_reader_consider_move);
}

static void
ct_move_reader_consider_move(void *delegate, CtMove move)
{
  CtMoveReader move_reader = (CtMoveReader) delegate;
  int compare = move_reader->compare_mask;

  if (compare & COMPARE_MOVE_TO)
  {
    CtSquare move_to = ct_move_to(move);

    if (move_reader->move_to != move_to)
      return;
  }

  if (compare & COMPARE_PIECE)
  {
    CtPiece piece = ct_position_get_piece(move_reader->position, ct_move_from(move));

    if (move_reader->piece != piece)
      return;
  }

  if (compare & COMPARE_MOVE_TYPE)
  {
    CtMoveType move_type = ct_move_type(move);

    if (move_reader->move_type != move_type)
      return;
  }

  if (compare & COMPARE_MOVE_FROM_FILE)
  {
    CtSquare move_from = ct_move_from(move);
    CtFile move_from_file = ct_square_file(move_from);

    if (move_reader->move_from_file != move_from_file)
      return;
  }

  if (compare & COMPARE_MOVE_FROM_RANK)
  {
    CtSquare move_from = ct_move_from(move);
    CtRank move_from_rank = ct_square_rank(move_from);

    if (move_reader->move_from_rank != move_from_rank)
      return;
  }

  if (compare & COMPARE_MOVE_PROMOTES_TO)
  {
    CtPiece move_promotes_to = ct_move_promotes_to(move);

    if (move_reader->move_promotes_to != move_promotes_to)
      return;
  }

  if (move_reader->result == NULL_MOVE)
    move_reader->result = move;
  else
    move_reader->result = AMBIGUOUS_MOVE;
}
