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

#ifndef CT_GRAPH_PRIVATE_H
#define CT_GRAPH_PRIVATE_H

#include "ct_types.h"
#include "ct_types_internal.h"

typedef struct CtGraphStruct
{
  CtPosition position;
  CtMoveCommand filter_pseudo_moves;
  CtMoveGenerator move_generator;
  CtMoveMaker move_maker;
  CtMoveCommand command_for_each_legal_move;
  CtMoveCommand command_for_each_move_made;
  CtMoveCommand replay_move_command;
  CtMoveStack move_stack;
} CtGraphStruct;

#endif                                /* CT_GRAPH_PRIVATE_H */
