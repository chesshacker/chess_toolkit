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

#ifndef CT_MOVE_GENERATOR_H
#define CT_MOVE_GENERATOR_H

#include "ct_types.h"
#include "ct_types_internal.h"

CtMoveGenerator ct_move_generator_new(CtPosition position, CtMoveCommand pseudo_legal_move_command);
void ct_move_generator_free(CtMoveGenerator move_generator);

void ct_move_generator_pseudo_legal_piece_moves(CtMoveGenerator move_generator);
void ct_move_generator_castle_moves(CtMoveGenerator move_generator, CtMoveCommand legal_move_command);

#endif                                /* CT_MOVE_GENERATOR_H */
