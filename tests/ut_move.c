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

static CtMove move;

START_TEST(ut_move_null_move)
{
  move = NULL_MOVE;
  ck_assert(!move);
  ck_assert_int_eq(ct_move_from(move), A1);
  ck_assert_int_eq(ct_move_to(move), A1);
  ck_assert_int_eq(ct_move_type(move), NORMAL_MOVE);
  ck_assert_int_eq(ct_move_promotes_to(move), EMPTY);

  /* a NULL_MOVE is the same as a "normal move" from A1 to A1 */
  move = ct_move_make(A1, A1);
  ck_assert(move == NULL_MOVE);
} END_TEST

START_TEST(ut_move_make)
{
  move = ct_move_make(E8, G6);
  ck_assert_int_eq(ct_move_from(move), E8);
  ck_assert_int_eq(ct_move_to(move), G6);
  ck_assert_int_eq(ct_move_type(move), NORMAL_MOVE);
  /* it doesn't make sense to check for promotes_to in this case, however it returns EMPTY */
  ck_assert_int_eq(ct_move_promotes_to(move), EMPTY);
} END_TEST

START_TEST(ut_move_make_castle)
{
  move = ct_move_make_castle_kingside(E1);
  ck_assert_int_eq(ct_move_from(move), E1);
  ck_assert_int_eq(ct_move_to(move), G1);
  ck_assert_int_eq(ct_move_type(move), CASTLE_KINGSIDE);
  /* it doesn't make sense to check for promotes_to in this case, however it returns EMPTY */
  ck_assert_int_eq(ct_move_promotes_to(move), EMPTY);

  move = ct_move_make_castle_queenside(E8);
  ck_assert_int_eq(ct_move_from(move), E8);
  ck_assert_int_eq(ct_move_to(move), C8);
  ck_assert_int_eq(ct_move_type(move), CASTLE_QUEENSIDE);
} END_TEST

START_TEST(ut_move_make_en_passant_possible)
{
  move = ct_move_make_en_passant_possible(E2, E4);
  ck_assert_int_eq(ct_move_from(move), E2);
  ck_assert_int_eq(ct_move_to(move), E4);
  ck_assert_int_eq(ct_move_type(move), EN_PASSANT_POSSIBLE);
  /* it doesn't make sense to check for promotes_to in this case, however it returns EMPTY */
  ck_assert_int_eq(ct_move_promotes_to(move), EMPTY);
} END_TEST

START_TEST(ut_move_make_en_passant_capture)
{
  move = ct_move_make_en_passant_capture(D4, E3);
  ck_assert_int_eq(ct_move_from(move), D4);
  ck_assert_int_eq(ct_move_to(move), E3);
  ck_assert_int_eq(ct_move_type(move), EN_PASSANT_CAPTURE);
  /* it doesn't make sense to check for promotes_to in this case, however it returns EMPTY */
  ck_assert_int_eq(ct_move_promotes_to(move), EMPTY);
} END_TEST

START_TEST(ut_move_make_promote)
{
  move = ct_move_make_promotion_Q(E7, F8);
  ck_assert_int_eq(ct_move_from(move), E7);
  ck_assert_int_eq(ct_move_to(move), F8);
  ck_assert_int_eq(ct_move_type(move), PROMOTION);
  ck_assert_int_eq(ct_move_promotes_to(move), WHITE_QUEEN);

  move = ct_move_make_promotion_n(B2, B1);
  ck_assert_int_eq(ct_move_from(move), B2);
  ck_assert_int_eq(ct_move_to(move), B1);
  ck_assert_int_eq(ct_move_type(move), PROMOTION);
  ck_assert_int_eq(ct_move_promotes_to(move), BLACK_KNIGHT);
} END_TEST

Suite *
ut_move_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_move");
  test_case = tcase_create("Move");
  tcase_add_test(test_case, ut_move_null_move);
  tcase_add_test(test_case, ut_move_make);
  tcase_add_test(test_case, ut_move_make_castle);
  tcase_add_test(test_case, ut_move_make_en_passant_possible);
  tcase_add_test(test_case, ut_move_make_en_passant_capture);
  tcase_add_test(test_case, ut_move_make_promote);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
