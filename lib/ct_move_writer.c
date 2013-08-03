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
#include "ct_move.h"
#include "ct_position.h"
#include "ct_move_command.h"
#include "ct_square.h"
#include "ct_piece.h"
#include "ct_error.h"
#include <string.h>
#include <ctype.h>

typedef struct CtMoveWriterStruct *CtMoveWriter;
typedef struct CtMoveWriterStruct
{
  CtMove move;
  CtMoveType move_type;
  CtSquare from;
  CtSquare to;
  CtPiece from_piece;
  CtPiece to_piece;
  CtPiece promotes_to;
  CtFile from_file;
  CtFile to_file;
  CtRank from_rank;
  CtRank to_rank;
  char *output;
  /* the following are only used if a graph is available */
  CtGraph graph;
  CtPosition position;
  CtMoveCommandStruct check_for_ambiguity;
  CtMoveCommandStruct check_for_checkmate;
  bool is_ambiguous;
  bool ambiguous_rank;
  bool ambiguous_file;
  bool is_check;
  bool is_checkmate;
} CtMoveWriterStruct;

static char default_destination_graph[CT_SAN_MAX_LENGTH];
static char default_destination_move[CT_MOVE_TO_S_MAX_LENGTH];

static void ct_move_writer_init(CtMoveWriter move_writer, CtGraph graph, char *destination);
static void ct_move_writer_disect_move(CtMoveWriter move_writer, CtMove move);
static void ct_move_writer_check_for_ambiguity(void *delegate, CtMove other_move);
static void ct_move_writer_check_for_checkmate(void *delegate, CtMove other_move);
static void ct_move_writer_describe_without_graph(CtMoveWriter move_writer);
static void ct_move_writer_describe_with_graph(CtMoveWriter move_writer);
static void ct_move_writer_add_check_or_checkmate(CtMoveWriter move_writer);

char *
ct_graph_move_to_san(CtGraph graph, CtMove move, char *destination)
{
  CtMoveWriterStruct move_writer_struct;
  CtMoveWriter move_writer = &move_writer_struct;
  char *result;
  bool has_graph;

  if (destination == 0)
    destination = default_destination_graph;
  ct_move_writer_init(move_writer, graph, destination);
  has_graph = move_writer->graph != 0;
  ct_move_writer_disect_move(move_writer, move);
  result = move_writer->output;

  switch (move_writer->move_type)
  {
  case CASTLE_KINGSIDE:
    move_writer->output = stpcpy(result, "O-O");
    break;
  case CASTLE_QUEENSIDE:
    move_writer->output = stpcpy(result, "O-O-O");
    break;
  default:
    if (has_graph)
      ct_move_writer_describe_with_graph(move_writer);
    else
      ct_move_writer_describe_without_graph(move_writer);
  }
  if (has_graph)
  {
    ct_move_writer_add_check_or_checkmate(move_writer);
    if (move_writer->output - result >= CT_SAN_MAX_LENGTH)
      ct_error("ct_graph_move_to_san: CT_SAN_MAX_LENGTH is not big enough");
  }
  else
  {
    if (move_writer->output - result >= CT_MOVE_TO_S_MAX_LENGTH)
      ct_error("ct_move_to_s: CT_MOVE_TO_S_MAX_LENGTH is not big enough");
  }
  return result;
}

char *
ct_move_to_s(CtMove move, char *destination)
{
  if (destination == 0)
    destination = default_destination_move;
  return ct_graph_move_to_san(0, move, destination);
}

static void
ct_move_writer_init(CtMoveWriter move_writer, CtGraph graph, char *destination)
{
  if (graph)
  {
    move_writer->graph = graph;
    move_writer->position = graph->position;
    move_writer->check_for_ambiguity = ct_move_command_make(move_writer, ct_move_writer_check_for_ambiguity);
    move_writer->check_for_checkmate = ct_move_command_make(move_writer, ct_move_writer_check_for_checkmate);
  }
  else
    move_writer->graph = 0;
  move_writer->output = destination;
}

static void
ct_move_writer_disect_move(CtMoveWriter move_writer, CtMove move)
{
  CtGraph graph = move_writer->graph;

  move_writer->move = move;
  move_writer->move_type = ct_move_type(move);
  move_writer->from = ct_move_from(move);
  move_writer->to = ct_move_to(move);
  move_writer->from_file = ct_square_file(move_writer->from);
  move_writer->from_rank = ct_square_rank(move_writer->from);
  move_writer->to_file = ct_square_file(move_writer->to);
  move_writer->to_rank = ct_square_rank(move_writer->to);
  if (move_writer->move_type == PROMOTION)
    move_writer->promotes_to = ct_move_promotes_to(move);
  if (graph)
  {
    CtPosition position = move_writer->position;

    move_writer->from_piece = ct_position_get_piece(position, move_writer->from);
    move_writer->to_piece = ct_position_get_piece(position, move_writer->to);

    move_writer->is_ambiguous = false;
    move_writer->ambiguous_rank = false;
    move_writer->ambiguous_file = false;
    ct_graph_for_each_legal_move(graph, &move_writer->check_for_ambiguity);

    ct_graph_make_move(move_writer->graph, move);
    move_writer->is_check = ct_position_is_check(move_writer->position);
    if (move_writer->is_check)
    {
      move_writer->is_checkmate = true;        /* any legal move will change this to false */
      ct_graph_for_each_legal_move(graph, &move_writer->check_for_checkmate);
    }
    else
      move_writer->is_checkmate = false;
    ct_graph_unmake_move(move_writer->graph);
  }
}

static void
ct_move_writer_check_for_ambiguity(void *delegate, CtMove other_move)
{
  CtMoveWriter move_writer = (CtMoveWriter) delegate;
  CtSquare other_from = ct_move_from(other_move);

  if (other_move != move_writer->move && ct_move_to(other_move) == move_writer->to
      && ct_position_get_piece(move_writer->position, other_from) == move_writer->from_piece)
  {
    move_writer->is_ambiguous = true;
    if (!move_writer->ambiguous_rank)
      move_writer->ambiguous_rank = ct_square_rank(other_from) == move_writer->from_rank;
    if (!move_writer->ambiguous_file)
      move_writer->ambiguous_file = ct_square_file(other_from) == move_writer->from_file;
  }
}

static void
ct_move_writer_check_for_checkmate(void *delegate, CtMove other_move)
{
  CtMoveWriter move_writer = (CtMoveWriter) delegate;

  if (move_writer->is_checkmate)
    move_writer->is_checkmate = false;
}

static void
ct_move_writer_describe_without_graph(CtMoveWriter move_writer)
{
  char *ct = move_writer->output;

  *ct++ = ct_file_to_char(move_writer->from_file);
  *ct++ = ct_rank_to_char(move_writer->from_rank);
  *ct++ = ct_file_to_char(move_writer->to_file);
  *ct++ = ct_rank_to_char(move_writer->to_rank);
  if (move_writer->move_type == PROMOTION)
  {
    *ct++ = '=';
    *ct++ = toupper(ct_piece_to_char(move_writer->promotes_to));
  }
  *ct = 0;
  move_writer->output = ct;
}

static void
ct_move_writer_describe_with_graph(CtMoveWriter move_writer)
{
  char *ct = move_writer->output;

  if (ct_piece_is_pawn(move_writer->from_piece))
  {
    if (move_writer->from_file != move_writer->to_file)
      *ct++ = ct_file_to_char(move_writer->from_file);
  }
  else
  {
    *ct++ = toupper(ct_piece_to_char(move_writer->from_piece));
    if (move_writer->is_ambiguous)
    {
      if (!move_writer->ambiguous_file || move_writer->ambiguous_rank)
        *ct++ = ct_file_to_char(move_writer->from_file);
      if (move_writer->ambiguous_file)
        *ct++ = ct_rank_to_char(move_writer->from_rank);
    }
  }
  if (move_writer->to_piece != EMPTY || move_writer->move_type == EN_PASSANT_CAPTURE)
    *ct++ = 'x';
  *ct++ = ct_file_to_char(move_writer->to_file);
  *ct++ = ct_rank_to_char(move_writer->to_rank);
  if (move_writer->move_type == PROMOTION)
  {
    *ct++ = '=';
    *ct++ = toupper(ct_piece_to_char(move_writer->promotes_to));
  }
  *ct = 0;
  move_writer->output = ct;
}

static void
ct_move_writer_add_check_or_checkmate(CtMoveWriter move_writer)
{
  if (move_writer->is_check)
  {
    char *ct = move_writer->output;

    *ct++ = move_writer->is_checkmate ? '#' : '+';
    *ct = 0;
    move_writer->output = ct;
  }
}
