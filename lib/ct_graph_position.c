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
#include "ct_position.h"
#include <string.h>
#include <stdio.h>

static CtPosition default_graph_to_position;
static CtGraph default_graph_from_position;
static char default_graph_to_fen[CT_FEN_MAX_LENGTH];
static CtGraph default_graph_from_fen;
static char default_graph_to_s[CT_GRAPH_TO_S_MAX_LENGTH];

void
ct_graph_position_init(void)
{
  default_graph_to_position = ct_position_new();
  default_graph_from_position = ct_graph_new();
  default_graph_from_fen = ct_graph_new();
}

CtPosition
ct_graph_to_position(CtGraph graph, CtPosition position)
{
  if (position == 0)
    position = default_graph_to_position;
  ct_position_copy(position, graph->position);
  return position;
}

CtGraph
ct_graph_from_position(CtGraph graph, CtPosition position)
{
  if (position == 0)
    return 0;
  if (graph == 0)
    graph = default_graph_from_position;
  ct_graph_reset(graph);
  ct_position_copy(graph->position, position);
  return graph;
}

char *
ct_graph_to_fen(CtGraph graph, char *destination)
{
  if (destination == 0)
    destination = default_graph_to_fen;
  return ct_position_to_fen(graph->position, destination);
}

CtGraph
ct_graph_from_fen(CtGraph graph, char *fen)
{
  if (fen == 0)
    return 0;
  if (graph == 0)
    graph = default_graph_from_fen;
  ct_graph_reset(graph);
  ct_position_from_fen(graph->position, fen);
  return graph;
}

int64_t
ct_graph_position_hash(CtGraph graph)
{
  return ct_position_hash(graph->position);
}

char *
ct_graph_to_s(CtGraph graph, char *destination)
{
  int length;
  int ply;

  if (destination == 0)
    destination = default_graph_to_s;
  ct_position_to_s(graph->position, destination);
  ply = ct_graph_ply(graph);
  length = strlen(destination);
  snprintf(&destination[length], CT_GRAPH_TO_S_MAX_LENGTH - length, "\nPly %d", ply);
  return destination;
}
