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

void ct_rays_init(void);
void ct_bit_board_init(void);
void ct_piece_init(void);
void ct_position_init(void);
void ct_move_maker_init(void);
void ct_position_from_fen_init(void);
void ct_position_hash_init(void);
void ct_graph_position_init(void);
void ct_graph_from_pgn_init(void);

void
chess_toolkit_init(void)
{
  ct_rays_init();                /* must initialize rays before initializing bit_board */
  ct_bit_board_init();
  ct_piece_init();
  ct_position_init();
  ct_move_maker_init();
  ct_position_from_fen_init();
  ct_position_hash_init();
  ct_graph_position_init();
  ct_graph_from_pgn_init();
}
