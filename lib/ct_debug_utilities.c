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
#include "ct_debug_utilities.h"
#include "ct_position.h"
#include "ct_move.h"
#include "ct_graph.h"
#include "ct_bit_board.h"
#include "ct_square.h"
#include "ct_error.h"
#include <stdio.h>

void
ct_position_print(CtPosition position)
{
  char output_string[CT_POSITION_TO_S_MAX_LENGTH];

  ct_position_to_s(position, output_string);
  printf("%s\n", output_string);
}

void
ct_move_print(CtMove move)
{
  char output_string[CT_MOVE_TO_S_MAX_LENGTH];

  ct_move_to_s(move, output_string);
  printf("%s\n", output_string);
}

void
ct_graph_print(CtGraph graph)
{
  char output_string[CT_GRAPH_TO_S_MAX_LENGTH];

  ct_graph_to_s(graph, output_string);
  printf("%s\n", output_string);
}

void
ct_bit_board_print(CtBitBoard bit_board)
{
  char output_string[CT_BIT_BOARD_TO_S_MAX_LENGTH];

  ct_bit_board_to_s(bit_board, output_string);
  printf("%s\n", output_string);
}
