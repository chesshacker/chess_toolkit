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

#ifndef CT_H
#define CT_H

#include "chess_toolkit/ct_types.h"
#include "chess_toolkit/ct_error.h"
#include "chess_toolkit/ct_square.h"
#include "chess_toolkit/ct_piece.h"
#include "chess_toolkit/ct_bit_board.h"
#include "chess_toolkit/ct_position.h"
#include "chess_toolkit/ct_move.h"
#include "chess_toolkit/ct_move_command.h"
#include "chess_toolkit/ct_move_stack.h"
#include "chess_toolkit/ct_piece_command.h"
#include "chess_toolkit/ct_graph.h"
#include "chess_toolkit/ct_command.h"
#include "chess_toolkit/ct_game_tags.h"

void chess_toolkit_init(void);

#endif                                /* CT_H */
