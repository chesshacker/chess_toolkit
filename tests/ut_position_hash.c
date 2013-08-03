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

START_TEST(ct_position_hash_hash)
{
  CtPosition position;
  int64_t hash, hash2;

  position = ct_position_from_fen(0, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
  ck_assert(position != 0);
  hash = ct_position_hash(position);
  ck_assert(hash == 3973843602409076421);

  position = ct_position_from_fen(0, "8/8/8/8/3KPpk1/8/8/8 b - e3");
  ck_assert(position != 0);
  hash = ct_position_hash(position);
  ck_assert(hash == 739132817695691147);

  position = ct_position_from_fen(0, "rnbqkbnr/pp3ppp/4p3/2ppP3/3P2Q1/8/PPP2PPP/RNB1KBNR b KQkq -");
  ck_assert(position != 0);
  hash = ct_position_hash(position);

  position = ct_position_from_fen(0, "rnbqkbnr/pp3ppp/4p3/2ppP3/3P4/4B3/PPP2PPP/RN1QKBNR b KQkq -");
  ck_assert(position != 0);
  hash2 = ct_position_hash(position);
  ck_assert(hash2 != hash);
} END_TEST

Suite *
ut_position_hash_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_position_hash");
  test_case = tcase_create("Position.hash");
  tcase_add_test(test_case, ct_position_hash_hash);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
