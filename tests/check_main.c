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
#include <check.h>
#include "chess_toolkit.h"

Suite *at_algebraic_notation_make_suite(void);
Suite *at_perft_make_suite(void);
Suite *ut_bit_board_make_suite(void);
Suite *ut_bit_board_to_s_make_suite(void);
Suite *ut_command_make_suite(void);
Suite *ut_debug_utilities_make_suite(void);
Suite *ut_error_make_suite(void);
Suite *ut_game_tags_make_suite(void);
Suite *ut_graph_make_suite(void);
Suite *ut_graph_dfs_make_suite(void);
Suite *ut_graph_position_make_suite(void);
Suite *ut_graph_to_new_pgn_make_suite(void);
Suite *ut_move_make_suite(void);
Suite *ut_move_command_make_suite(void);
Suite *ut_move_generator_make_suite(void);
Suite *ut_move_maker_make_suite(void);
Suite *ut_move_reader_make_suite(void);
Suite *ut_move_stack_make_suite(void);
Suite *ut_move_writer_make_suite(void);
Suite *ut_pawn_make_suite(void);
Suite *ut_graph_from_pgn_make_suite(void);
Suite *ut_piece_make_suite(void);
Suite *ut_piece_command_make_suite(void);
Suite *ut_position_make_suite(void);
Suite *ut_position_from_fen_make_suite(void);
Suite *ut_position_hash_make_suite(void);
Suite *ut_position_rules_make_suite(void);
Suite *ut_position_to_fen_make_suite(void);
Suite *ut_position_to_s_make_suite(void);
Suite *ut_rays_make_suite(void);
Suite *ut_slider_make_suite(void);
Suite *ut_square_make_suite(void);
Suite *ut_steper_make_suite(void);
Suite *ut_undo_position_make_suite(void);
Suite *ut_utilities_make_suite(void);

typedef Suite *(*MakeSuiteMethod) (void);

static MakeSuiteMethod make_suite_methods[] =
{
  at_algebraic_notation_make_suite,
  at_perft_make_suite,
  ut_bit_board_make_suite,
  ut_bit_board_to_s_make_suite,
  ut_command_make_suite,
  ut_debug_utilities_make_suite,
  ut_error_make_suite,
  ut_game_tags_make_suite,
  ut_graph_make_suite,
  ut_graph_dfs_make_suite,
  ut_graph_position_make_suite,
  ut_graph_to_new_pgn_make_suite,
  ut_move_make_suite,
  ut_move_command_make_suite,
  ut_move_generator_make_suite,
  ut_move_maker_make_suite,
  ut_move_reader_make_suite,
  ut_move_stack_make_suite,
  ut_move_writer_make_suite,
  ut_pawn_make_suite,
  ut_graph_from_pgn_make_suite,
  ut_piece_make_suite,
  ut_piece_command_make_suite,
  ut_position_make_suite,
  ut_position_from_fen_make_suite,
  ut_position_hash_make_suite,
  ut_position_rules_make_suite,
  ut_position_to_fen_make_suite,
  ut_position_to_s_make_suite,
  ut_rays_make_suite,
  ut_slider_make_suite,
  ut_square_make_suite,
  ut_steper_make_suite,
  ut_undo_position_make_suite,
  ut_utilities_make_suite,
  0
};

int
main(int argc, char **argv)
{
  MakeSuiteMethod *make_suite_method_ptr;
  SRunner *master_suite_runner;
  int number_failed = 0;

  chess_toolkit_init();
  master_suite_runner = srunner_create(NULL);
  for (make_suite_method_ptr = make_suite_methods; *make_suite_method_ptr != 0; make_suite_method_ptr++)
    srunner_add_suite(master_suite_runner, (*make_suite_method_ptr) ());
  srunner_run_all(master_suite_runner, CK_NORMAL);
  number_failed += srunner_ntests_failed(master_suite_runner);
  srunner_free(master_suite_runner);
  return (number_failed == 0) ? 0 : -1;
}
