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

typedef struct UtGameTagsTestStruct *UtGameTagsTest;
typedef struct UtGameTagsTestStruct
{
  bool end_of_tests;
  char key[9];
  char value[GAME_TAGS_VALUE_MAX_LENGTH];
  char expected_before[GAME_TAGS_VALUE_MAX_LENGTH];
  char expected_after[GAME_TAGS_VALUE_MAX_LENGTH];
} UtGameTagsTestStruct;

static CtGameTags game_tags;

static void setup(void);
static void teardown(void);

static void
setup(void)
{
  game_tags = ct_game_tags_new();
}

static void
teardown(void)
{
  ct_game_tags_free(game_tags);
}

START_TEST(ut_game_tags_get_set_reset)
{
  UtGameTagsTestStruct tests[] = {
    {false, "Event", "World Championship 28th", "?", "World Championship 28th"},
    {false, "Site", "Reykjavik", "?", "Reykjavik"},
    {false, "Date", "1972.07.23", "?", "1972.07.23"},
    {false, "Round", "6", "?", "6"},
    {false, "White", "Fischer, Robert James", "?", "Fischer, Robert James"},
    {false, "Black", "Spassky, Boris Vasilevich", "?", "Spassky, Boris Vasilevich"},
    {false, "Result", "1-0", "*", "1-0"},        /* result must always be a valid value (and defaults to "*") */
    {false, "WhiteElo", "2785", "?", "2785"},
    {false, "BlackElo", "2660", "?", "2660"},
    {false, "ECO", "D59", "?", "D59"},
    {false, "PlyCount", "81", "?", "?"},        /* unknown tags won't be saved */
    {false, "Site", "Reykjavik ISL", "Reykjavik", "Reykjavik ISL"},        /* tags can be changed */
    {true, "", "", "", ""}
  };
  UtGameTagsTest test;
  char *all_valid_keys[] = {"Event", "Site", "Date", "Round", "White", "Black", "WhiteElo", "BlackElo", "Result", 0};
  char **key_ptr;
  char really_long_string[GAME_TAGS_VALUE_MAX_LENGTH + 1];
  int index;

  for (test = tests; !test->end_of_tests; test++)
  {
    ck_assert_str_eq(ct_game_tags_get(game_tags, test->key), test->expected_before);
    ct_game_tags_set(game_tags, test->key, test->value);
    ck_assert_str_eq(ct_game_tags_get(game_tags, test->key), test->expected_after);
  }

  ct_game_tags_reset(game_tags);
  for (key_ptr = all_valid_keys; *key_ptr != 0; key_ptr++)
  {
    if (strcmp(*key_ptr, "Result") != 0)        /* Result must always be valid (and defaults to "*") */
      ck_assert_str_eq(ct_game_tags_get(game_tags, *key_ptr), "?");
    else
      ck_assert_str_eq(ct_game_tags_get(game_tags, *key_ptr), "*");
  }

  for (index = 0; index < GAME_TAGS_VALUE_MAX_LENGTH; index++)
    really_long_string[index] = '0' + (index % 10);
  really_long_string[GAME_TAGS_VALUE_MAX_LENGTH] = 0;
  /* this string is one character too long to fit */
  ct_game_tags_set(game_tags, "Event", really_long_string);
  really_long_string[GAME_TAGS_VALUE_MAX_LENGTH - 1] = 0;        /* shortened by one character */
  ck_assert_str_eq(ct_game_tags_get(game_tags, "Event"), really_long_string);
} END_TEST

Suite *
ut_game_tags_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_game_tags");
  test_case = tcase_create("GameTags");
  tcase_add_checked_fixture(test_case, setup, teardown);
  tcase_add_test(test_case, ut_game_tags_get_set_reset);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
