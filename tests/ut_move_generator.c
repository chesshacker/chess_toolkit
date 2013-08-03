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
#include "ct_move_generator.h"        /* not part of the advertised API, used by CtGraph */
#include <stdio.h>

/* static variables that are part of what's being tested */
static CtPosition mg_position;
static CtMoveStack actual_move_stack;
static CtMoveGenerator move_generator;

/* static variables that are used to help run the test */
static CtMoveStack expected_move_stack;

static void setup(void);
static void teardown(void);
static void ut_move_generator_test_fen(char *fen);

static void
setup(void)
{
  mg_position = ct_position_new();
  actual_move_stack = ct_move_stack_new();
  move_generator = ct_move_generator_new(mg_position, ct_move_stack_push_command(actual_move_stack));

  expected_move_stack = ct_move_stack_new();
}

static void
teardown(void)
{
  ct_move_stack_free(expected_move_stack);

  ct_move_generator_free(move_generator);
  ct_move_stack_free(actual_move_stack);
  ct_position_free(mg_position);
}

START_TEST(ut_move_generator_pseudo_legal_piece_moves)
{
  /* white's turn with no white pieces -- verifies no moves are generated */
  ut_move_generator_test_fen("8/7p/8/8/8/8/8/8 w - -");
  /* black's turn with no black pieces -- verifies no moves are generated */
  ut_move_generator_test_fen("8/7Q/8/8/8/8/8/8 b - -");
  /* white pawn */
  ct_move_stack_push(expected_move_stack, ct_move_make(A2, B3));
  ut_move_generator_test_fen("8/8/8/8/8/pp6/P7/8 w - -");
  /* black pawn */
  ct_move_stack_push(expected_move_stack, ct_move_make_en_passant_capture(H4, G3));
  ut_move_generator_test_fen("8/8/8/8/6Pp/7P/8/8 b - g3");
  /* white king */
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, H2));
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, G1));
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, G2));
  ut_move_generator_test_fen("8/8/8/8/8/8/8/7K w - -");
  /* black king -- pseudo legal moves can move into check */
  ct_move_stack_push(expected_move_stack, ct_move_make(A7, A8));
  ct_move_stack_push(expected_move_stack, ct_move_make(A7, B8));
  ct_move_stack_push(expected_move_stack, ct_move_make(A7, B7));        /* only LEGAL move */
  ct_move_stack_push(expected_move_stack, ct_move_make(A7, B6));
  ct_move_stack_push(expected_move_stack, ct_move_make(A7, A6));
  ut_move_generator_test_fen("7R/k7/7R/8/8/8/8/8 b - -");
  /* white rook -- pseudo legal moves can leave the king in check when they should block it */
  ct_move_stack_push(expected_move_stack, ct_move_make(A1, B1));
  ct_move_stack_push(expected_move_stack, ct_move_make(A2, A3));
  ct_move_stack_push(expected_move_stack, ct_move_make(A2, A4));
  ct_move_stack_push(expected_move_stack, ct_move_make(A2, A5));
  ct_move_stack_push(expected_move_stack, ct_move_make(A2, A6));
  ct_move_stack_push(expected_move_stack, ct_move_make(A2, A7));
  ct_move_stack_push(expected_move_stack, ct_move_make(A2, A8));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B3));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B4));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B5));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B6));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B7));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B8));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, C2));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, D2));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, E2));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, F2));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, G2));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, H2));
  ct_move_stack_push(expected_move_stack, ct_move_make(B2, B1));        /* only LEGAL move */
  ut_move_generator_test_fen("8/8/8/8/8/8/RR6/K6r w - -");
  /* black rook -- pseudo legal moves can leave the king in check when they should capture */
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, B8));
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, B7));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H7));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H6));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H5));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H4));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H3));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H2));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H1));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, G8));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, F8));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, E8));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, D8));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, C8));        /* only LEGAL move */
  ut_move_generator_test_fen("k1Q4r/p7/P7/8/8/8/8/8 b - -");
  /* white knight */
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, G3));
  ut_move_generator_test_fen("8/8/8/8/8/5p2/5P2/7N w - -");
  /* black knight */
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, F2));
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, G3));
  ut_move_generator_test_fen("8/8/8/8/8/5p2/5P2/7n b - -");
  /* white bishop */
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, B7));
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, C6));
  ut_move_generator_test_fen("B7/8/2p5/2P5/8/8/8/8 w - -");
  /* black bishop */
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, B7));
  ut_move_generator_test_fen("b7/8/2p5/2P5/8/8/8/8 b - -");
  /* white queen */
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H7));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H6));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, G7));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, G8));
  ut_move_generator_test_fen("6rQ/6r1/7p/8/8/8/8/8 w - -");
  /* black queen */
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H7));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, H6));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, G7));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, G8));
  ut_move_generator_test_fen("6Rq/6R1/7P/8/8/8/8/8 b - -");
} END_TEST

START_TEST(ut_move_generator_castle_moves)
{
  /* castling white - castling in both directions is possible */
  ct_move_stack_push(expected_move_stack, ct_move_make(A1, B1));
  ct_move_stack_push(expected_move_stack, ct_move_make(A1, C1));
  ct_move_stack_push(expected_move_stack, ct_move_make(A1, D1));
  ct_move_stack_push(expected_move_stack, ct_move_make(E1, E2));
  ct_move_stack_push(expected_move_stack, ct_move_make(E1, F2));
  ct_move_stack_push(expected_move_stack, ct_move_make(E1, F1));
  ct_move_stack_push(expected_move_stack, ct_move_make(E1, D1));
  ct_move_stack_push(expected_move_stack, ct_move_make(E1, D2));
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, G1));
  ct_move_stack_push(expected_move_stack, ct_move_make(H1, F1));
  ct_move_stack_push(expected_move_stack, ct_move_make_castle_kingside(E1));
  ct_move_stack_push(expected_move_stack, ct_move_make_castle_queenside(E1));
  ut_move_generator_test_fen("8/8/8/8/8/p6p/P6P/R3K2R w KQ -");
  /* castling black - queenside only */
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, B8));
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, C8));
  ct_move_stack_push(expected_move_stack, ct_move_make(A8, D8));
  ct_move_stack_push(expected_move_stack, ct_move_make(E8, F8));
  ct_move_stack_push(expected_move_stack, ct_move_make(E8, F7));
  ct_move_stack_push(expected_move_stack, ct_move_make(E8, E7));
  ct_move_stack_push(expected_move_stack, ct_move_make(E8, D7));
  ct_move_stack_push(expected_move_stack, ct_move_make(E8, D8));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, G8));
  ct_move_stack_push(expected_move_stack, ct_move_make(H8, F8));
  ct_move_stack_push(expected_move_stack, ct_move_make_castle_queenside(E8));
  ut_move_generator_test_fen("r3k2r/p6p/P6P/8/8/8/8/6R1 b kq -");
} END_TEST

static void
ut_move_generator_test_fen(char *fen)
{
  CtMove move, expected_move;

  ck_assert_msg(ct_position_from_fen(mg_position, fen) != 0, fen);
  ct_move_generator_pseudo_legal_piece_moves(move_generator);
  ct_move_generator_castle_moves(move_generator, ct_move_stack_push_command(actual_move_stack));
  while (!ct_move_stack_is_empty(actual_move_stack))
  {
    move = ct_move_stack_pop(actual_move_stack);
    expected_move = ct_move_stack_pop(expected_move_stack);
    if (move != expected_move)
    {
      char msg[CT_FEN_MAX_LENGTH + 100];
      char move_to_s[CT_MOVE_TO_S_MAX_LENGTH];
      char expected_move_to_s[CT_MOVE_TO_S_MAX_LENGTH];

      ct_move_to_s(move, move_to_s);
      ct_move_to_s(expected_move, expected_move_to_s);
      snprintf(msg, sizeof(msg), "Failed fen=%s, move=%s, expected_move=%s", fen, move_to_s, expected_move_to_s);
      ck_abort_msg(msg);
    }
  }
  ck_assert_msg(ct_move_stack_is_empty(expected_move_stack), fen);
}

Suite *
ut_move_generator_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_move_generator");
  test_case = tcase_create("MoveGenerator");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_move_generator_castle_moves);
  tcase_add_test(test_case, ut_move_generator_pseudo_legal_piece_moves);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
