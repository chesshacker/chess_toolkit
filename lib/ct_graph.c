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
#include "ct_position.h"
#include "ct_move_command.h"
#include "ct_move_generator.h"
#include "ct_move_maker.h"
#include "ct_move_stack.h"

static void ct_graph_filter_pseudo_moves(void *delegate, CtMove move);
static void ct_graph_replay_move(void *delegate, CtMove);

CtGraph
ct_graph_new(void)
{
  CtGraph graph;

  graph = ct_malloc(sizeof(CtGraphStruct));
  graph->position = ct_position_new();
  graph->filter_pseudo_moves = ct_move_command_new(graph, ct_graph_filter_pseudo_moves);
  graph->move_generator = ct_move_generator_new(graph->position, graph->filter_pseudo_moves);
  graph->move_maker = ct_move_maker_new(graph->position);
  graph->move_stack = ct_move_stack_new();
  graph->replay_move_command = ct_move_command_new(graph, ct_graph_replay_move);
  return graph;
}

void
ct_graph_free(CtGraph graph)
{
  ct_move_command_free(graph->replay_move_command);
  ct_move_stack_free(graph->move_stack);
  ct_move_maker_free(graph->move_maker);
  ct_move_generator_free(graph->move_generator);
  ct_move_command_free(graph->filter_pseudo_moves);
  ct_position_free(graph->position);
  ct_free(graph);
}

void
ct_graph_reset(CtGraph graph)
{
  ct_position_reset(graph->position);
  ct_move_stack_reset(graph->move_stack);
  ct_move_maker_reset(graph->move_maker);
}

int
ct_graph_ply(CtGraph graph)
{
  return ct_move_stack_length(graph->move_stack);
}

void
ct_graph_for_each_legal_move(CtGraph graph, CtMoveCommand move_command)
{
  graph->command_for_each_legal_move = move_command;
  ct_move_generator_pseudo_legal_piece_moves(graph->move_generator);
  ct_move_generator_castle_moves(graph->move_generator, graph->command_for_each_legal_move);
}

static void
ct_graph_filter_pseudo_moves(void *delegate, CtMove move)
{
  CtGraph graph = (CtGraph) delegate;
  CtMoveMaker move_maker = graph->move_maker;
  bool is_legal_move;

  ct_move_maker_make(move_maker, move);
  is_legal_move = ct_position_is_legal(graph->position);
  ct_move_maker_unmake(move_maker);
  if (is_legal_move)
    ct_move_command_execute(graph->command_for_each_legal_move, move);
}

void
ct_graph_for_each_move_made(CtGraph graph, CtMoveCommand move_command)
{
  CtMoveStack move_stack = graph->move_stack;
  int move_stack_length = ct_move_stack_length(move_stack);
  CtMoveMaker move_maker = graph->move_maker;

  graph->command_for_each_move_made = move_command;
  while (move_stack_length-- > 0)
    ct_move_maker_unmake(move_maker);
  ct_move_stack_for_each(graph->move_stack, graph->replay_move_command);
}

static void
ct_graph_replay_move(void *delegate, CtMove move)
{
  CtGraph graph = (CtGraph) delegate;

  ct_move_command_execute(graph->command_for_each_move_made, move);
  ct_move_maker_make(graph->move_maker, move);
}

void
ct_graph_make_move(CtGraph graph, CtMove move)
{
  ct_move_maker_make(graph->move_maker, move);
  ct_move_stack_push(graph->move_stack, move);
}

CtMove
ct_graph_unmake_move(CtGraph graph)
{
  ct_move_maker_unmake(graph->move_maker);
  return ct_move_stack_pop(graph->move_stack);
}
