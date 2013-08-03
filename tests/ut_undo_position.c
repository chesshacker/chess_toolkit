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
#include "ct_undo_position.h"        /* not part of the advertised API, used by CtMoveMaker */

enum
{
  SAVED_FENS_MAX = 63 * 2
};

static CtPosition position;
static CtUndoPosition undo_position;
static char saved_fens[SAVED_FENS_MAX][CT_FEN_MAX_LENGTH];
static int saved_fens_next_index = 0;

static void setup(void);
static void teardown(void);

static void
setup(void)
{
  position = ct_position_new();
  undo_position = ct_undo_position_new(position);
}

static void
teardown(void)
{
  ct_undo_position_free(undo_position);
}

static void
save_fen(void)
{
  ck_assert(saved_fens_next_index < SAVED_FENS_MAX);
  ct_position_to_fen(position, saved_fens[saved_fens_next_index++]);
}

static bool
fen_matches_saved(void)
{
  char current_fen[CT_FEN_MAX_LENGTH];
  char *expected_fen = saved_fens[--saved_fens_next_index];
  ct_position_to_fen(position, current_fen);
  return strcmp(current_fen, expected_fen) == 0;
}

START_TEST(ut_undo_position_nothing_to_undo)
{
  save_fen();
  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());
} END_TEST

START_TEST(ut_undo_position_set_piece)
{
  save_fen();
  ck_assert(ct_position_is_white_to_move(position));
  ct_undo_position_start(undo_position);
  ck_assert(!ct_position_is_white_to_move(position));        /* start changes whose turn it is */
  ct_undo_position_set_piece(undo_position, A4, BLACK_ROOK);
  ck_assert_int_eq(ct_position_get_piece(position, A4), BLACK_ROOK);

  save_fen();
  ct_undo_position_start(undo_position);
  ck_assert(ct_position_is_white_to_move(position));        /* back to white turn */
  ct_undo_position_set_piece(undo_position, E1, WHITE_KING);
  ck_assert_int_eq(ct_position_get_piece(position, E1), WHITE_KING);
  ct_undo_position_set_piece(undo_position, H7, BLACK_KNIGHT);
  ck_assert_int_eq(ct_position_get_piece(position, H7), BLACK_KNIGHT);

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());
  ck_assert_int_eq(saved_fens_next_index, 0);
} END_TEST

START_TEST(ut_undo_position_set_en_passant)
{
  save_fen();
  ct_undo_position_start(undo_position);
  ct_undo_position_set_en_passant(undo_position, FILE_A);
  ck_assert_int_eq(ct_position_get_en_passant(position), FILE_A);

  save_fen();
  ct_undo_position_start(undo_position);
  ct_undo_position_set_en_passant(undo_position, FILE_E);
  ck_assert_int_eq(ct_position_get_en_passant(position), FILE_E);

  save_fen();
  ct_undo_position_start(undo_position);        /* start also clears en passant */
  ck_assert_int_eq(ct_position_get_en_passant(position), NO_EN_PASSANT);

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());
  ck_assert_int_eq(saved_fens_next_index, 0);
} END_TEST

START_TEST(ut_undo_position_set_castle)
{
  save_fen();
  ck_assert_int_eq(ct_position_get_castle(position), CASTLE_KQkq);
  ct_undo_position_start(undo_position);
  ct_undo_position_set_castle(undo_position, CASTLE_NONE);
  ck_assert_int_eq(ct_position_get_castle(position), CASTLE_NONE);

  save_fen();
  ct_undo_position_start(undo_position);
  ct_undo_position_set_castle(undo_position, CASTLE_Kq);
  ck_assert_int_eq(ct_position_get_castle(position), CASTLE_Kq);

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());
  ck_assert_int_eq(saved_fens_next_index, 0);
} END_TEST

START_TEST(ut_undo_position_move_piece)
{
  save_fen();
  ck_assert_int_eq(ct_position_get_piece(position, E2), WHITE_PAWN);
  ct_undo_position_start(undo_position);
  ct_undo_position_move_piece(undo_position, E2, E4);
  ck_assert_int_eq(ct_position_get_piece(position, E2), EMPTY);
  ck_assert_int_eq(ct_position_get_piece(position, E4), WHITE_PAWN);

  save_fen();
  ck_assert_int_eq(ct_position_get_piece(position, D7), BLACK_PAWN);
  ct_undo_position_start(undo_position);
  ct_undo_position_move_piece(undo_position, D7, E4);        /* it will accept and undo strange / illegal moves too */
  ck_assert_int_eq(ct_position_get_piece(position, D7), EMPTY);
  ck_assert_int_eq(ct_position_get_piece(position, E4), BLACK_PAWN);

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());

  ct_undo_position_undo(undo_position);
  ck_assert(fen_matches_saved());
  ck_assert_int_eq(saved_fens_next_index, 0);
} END_TEST

START_TEST(ut_undo_position_lots_of_undos)
{
  CtSquare square;
  char fen[CT_FEN_MAX_LENGTH];

  for (square = A1; square <= G8; square++)
  {
    save_fen();
    ct_undo_position_start(undo_position);
    ct_undo_position_move_piece(undo_position, square, square + 1);
  }
  ct_position_to_fen(position, fen);
  ck_assert_str_eq(fen, "7R/8/8/8/8/8/8/8 b KQkq -");

  for (square = H8; square >= B1; square--)
  {
    save_fen();
    ct_undo_position_start(undo_position);
    ct_undo_position_move_piece(undo_position, square, square - 1);
  }
  ct_position_to_fen(position, fen);
  ck_assert_str_eq(fen, "8/8/8/8/8/8/8/R7 w KQkq -");

  for (square = H8; square >= B1; square--)
  {
    ct_undo_position_undo(undo_position);
    ck_assert(fen_matches_saved());
  }
  ct_position_to_fen(position, fen);
  ck_assert_str_eq(fen, "7R/8/8/8/8/8/8/8 b KQkq -");

  for (square = A1; square <= G8; square++)
  {
    ct_undo_position_undo(undo_position);
    ck_assert(fen_matches_saved());
  }
  ck_assert_int_eq(saved_fens_next_index, 0);
} END_TEST

START_TEST(ut_undo_position_do_not_change_position)
{
  ct_undo_position_start(undo_position);
  ct_undo_position_move_piece(undo_position, E2, E4);

  save_fen();                        /* saved here rather than at the start so it's not saving the initial position */

  ct_undo_position_start(undo_position);
  ct_undo_position_move_piece(undo_position, E7, E5);

  ct_position_reset(position);        /* don't do this in a real program */

  /* CtUndoPosition will not work correctly after the underlying position is changed, reset or cleared... it only saves
     the operations that were done through the undo_position interface. It does not know the position was changed by
     something else */
  ct_undo_position_undo(undo_position);
  ck_assert(!fen_matches_saved());

  ck_assert_int_eq(saved_fens_next_index, 0);
} END_TEST

START_TEST(ut_undo_position_reset)
{
  ct_undo_position_start(undo_position);
  ct_undo_position_move_piece(undo_position, E2, E4);

  save_fen();                        /* save it twice to show after the last undo, nothing happens since the undo_position
                                   was reset after e2-e4 */
  save_fen();
  ct_undo_position_reset(undo_position);        /* undo_position forgets how to undo e2-e4 */

  ct_undo_position_start(undo_position);
  ct_undo_position_move_piece(undo_position, E7, E5);
  save_fen();

  ct_undo_position_start(undo_position);
  ct_undo_position_move_piece(undo_position, F2, F4);

  ct_undo_position_undo(undo_position);        /* undo f2-f4 */
  ck_assert(fen_matches_saved());

  ct_undo_position_undo(undo_position);        /* undo e7-e5 */
  ck_assert(fen_matches_saved());

  ct_undo_position_undo(undo_position);        /* nothing left to undo, since the undo_position was reset after e2-e4 */
  ck_assert(fen_matches_saved());

  ck_assert_int_eq(saved_fens_next_index, 0);
} END_TEST

Suite *
ut_undo_position_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_undo_position");
  test_case = tcase_create("UndoPosition");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_undo_position_nothing_to_undo);
  tcase_add_test(test_case, ut_undo_position_set_piece);
  tcase_add_test(test_case, ut_undo_position_set_en_passant);
  tcase_add_test(test_case, ut_undo_position_set_castle);
  tcase_add_test(test_case, ut_undo_position_move_piece);
  tcase_add_test(test_case, ut_undo_position_lots_of_undos);
  tcase_add_test(test_case, ut_undo_position_do_not_change_position);
  tcase_add_test(test_case, ut_undo_position_reset);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
