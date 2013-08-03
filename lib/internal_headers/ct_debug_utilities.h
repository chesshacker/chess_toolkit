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

#ifndef CT_DEBUG_UTILITIES_H
#define CT_DEBUG_UTILITIES_H

#include "ct_types.h"

void ct_position_print(CtPosition position);
void ct_move_print(CtMove move);
void ct_graph_print(CtGraph graph);
void ct_bit_board_print(CtBitBoard bit_board);

#endif                                /* CT_DEBUG_UTILITIES_H */
