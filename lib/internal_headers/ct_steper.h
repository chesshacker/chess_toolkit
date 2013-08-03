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

#ifndef CT_STEPER_H
#define CT_STEPER_H

#include "ct_types.h"
#include "ct_types_internal.h"

CtSteper ct_king_new(CtMoveCommand move_command, CtPieceColor piece_color);
CtSteper ct_knight_new(CtMoveCommand move_command, CtPieceColor piece_color);
void ct_steper_free(CtSteper steper);

void ct_steper_move(CtSteper steper, CtPosition position, CtSquare from);

#endif                                /* CT_STEPER_H */
