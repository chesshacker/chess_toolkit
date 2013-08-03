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
#include "ct_command.h"
#include "ct_graph.h"
#include "ct_move_stack.h"
#include "ct_move.h"
#include "ct_utilities.h"

void
ct_graph_dfs(CtGraph graph, CtCommand command, int depth)
{
  CtMoveStack move_stack;
  CtMoveCommand push_to_move_stack;
  int currentLevel;
  CtMove move;

  if (depth < 1)
    return;                        /* depth < 1 would have iterated forever */
  move_stack = ct_move_stack_new();
  push_to_move_stack = ct_move_stack_push_command(move_stack);
  ct_graph_for_each_legal_move(graph, push_to_move_stack);
  currentLevel = 1;
  while (!ct_move_stack_is_empty(move_stack))
  {
    move = ct_move_stack_pop(move_stack);
    if (move == NULL_MOVE)
    {
      ct_graph_unmake_move(graph);
      currentLevel--;
    }
    else if (currentLevel == depth)
    {
      ct_graph_make_move(graph, move);
      ct_command_execute(command);
      ct_graph_unmake_move(graph);
    }
    else
    {
      ct_move_stack_push(move_stack, NULL_MOVE);
      ct_graph_make_move(graph, move);
      ct_graph_for_each_legal_move(graph, push_to_move_stack);
      currentLevel++;
    }
  }
  ct_move_stack_free(move_stack);
}
