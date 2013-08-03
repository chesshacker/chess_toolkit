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
#include "ct_rays.h"
#include <stdio.h>

typedef struct UtRaysTestStruct *UtRaysTest;
typedef struct UtRaysTestStruct
{
  bool end_of_tests;
  CtSquare from;
  CtDirection answers[9];
} UtRaysTestStruct;

static void
ut_rays_run_tests(CtRays rays, UtRaysTest tests)
{
  CtDirection *answer;
  int problem_number = 0;
  CtDirection result;
  UtRaysTest test;

  for (test = tests, problem_number = 1; !test->end_of_tests; problem_number++, test++)
  {
    ct_rays_from(rays, test->from);
    answer = test->answers;
    do
    {
      result = ct_rays_next_direction(rays);
      if (result != *answer)
      {
        char msg[100];

        snprintf(msg, sizeof(msg), "Failed problem %d: *answer=%d, result=%d",
                 problem_number, *answer, result);
        ck_abort_msg(msg);
      }
      answer++;
    } while (result != D_NONE);
  }
}

START_TEST(ut_rays_king)
{
  UtRaysTestStruct tests[] = {
    {false, A1, {D_N, D_NE, D_E, D_NONE}},
    {false, C1, {D_N, D_NE, D_E, D_W, D_NW, D_NONE}},
    {false, H1, {D_N, D_W, D_NW, D_NONE}},
    {false, A2, {D_N, D_NE, D_E, D_SE, D_S, D_NONE}},
    {false, D4, {D_N, D_NE, D_E, D_SE, D_S, D_SW, D_W, D_NW, D_NONE}},
    {false, H5, {D_N, D_S, D_SW, D_W, D_NW, D_NONE}},
    {false, A8, {D_E, D_SE, D_S, D_NONE}},
    {false, B8, {D_E, D_SE, D_S, D_SW, D_W, D_NONE}},
    {false, H8, {D_S, D_SW, D_W, D_NONE}},
    {true, 0, {0}}
  };
  CtRays rays = ct_rays_new_king();

  ut_rays_run_tests(rays, tests);
  ct_rays_free(rays);
} END_TEST

START_TEST(ut_rays_queen)
{
  UtRaysTestStruct tests[] = {
    {false, A1, {D_N, D_NE, D_E, D_NONE}},
    {false, C1, {D_N, D_NE, D_E, D_W, D_NW, D_NONE}},
    {false, H1, {D_N, D_W, D_NW, D_NONE}},
    {false, A2, {D_N, D_NE, D_E, D_SE, D_S, D_NONE}},
    {false, D4, {D_N, D_NE, D_E, D_SE, D_S, D_SW, D_W, D_NW, D_NONE}},
    {false, H5, {D_N, D_S, D_SW, D_W, D_NW, D_NONE}},
    {false, A8, {D_E, D_SE, D_S, D_NONE}},
    {false, B8, {D_E, D_SE, D_S, D_SW, D_W, D_NONE}},
    {false, H8, {D_S, D_SW, D_W, D_NONE}},
    {true, 0, {0}}
  };
  CtRays rays = ct_rays_new_queen();

  ut_rays_run_tests(rays, tests);
  ct_rays_free(rays);
} END_TEST

START_TEST(ut_rays_rook)
{
  UtRaysTestStruct tests[] = {
    {false, A1, {D_N, D_E, D_NONE}},
    {false, C1, {D_N, D_E, D_W, D_NONE}},
    {false, H1, {D_N, D_W, D_NONE}},
    {false, A2, {D_N, D_E, D_S, D_NONE}},
    {false, D4, {D_N, D_E, D_S, D_W, D_NONE}},
    {false, H5, {D_N, D_S, D_W, D_NONE}},
    {false, A8, {D_E, D_S, D_NONE}},
    {false, B8, {D_E, D_S, D_W, D_NONE}},
    {false, H8, {D_S, D_W, D_NONE}},
    {true, 0, {0}}
  };
  CtRays rays = ct_rays_new_rook();

  ut_rays_run_tests(rays, tests);
  ct_rays_free(rays);
} END_TEST

START_TEST(ut_rays_bishop)
{
  UtRaysTestStruct tests[] = {
    {false, A1, {D_NE, D_NONE}},
    {false, C1, {D_NE, D_NW, D_NONE}},
    {false, H1, {D_NW, D_NONE}},
    {false, A2, {D_NE, D_SE, D_NONE}},
    {false, D4, {D_NE, D_SE, D_SW, D_NW, D_NONE}},
    {false, H5, {D_SW, D_NW, D_NONE}},
    {false, A8, {D_SE, D_NONE}},
    {false, B8, {D_SE, D_SW, D_NONE}},
    {false, H8, {D_SW, D_NONE}},
    {true, 0, {0}}
  };
  CtRays rays = ct_rays_new_bishop();

  ut_rays_run_tests(rays, tests);
  ct_rays_free(rays);
} END_TEST

START_TEST(ut_rays_knight)
{
  UtRaysTestStruct tests[] = {
    {false, A1, {D_NNE, D_ENE, D_NONE}},
    {false, B1, {D_NNE, D_ENE, D_NNW, D_NONE}},
    {false, C1, {D_NNE, D_ENE, D_WNW, D_NNW, D_NONE}},
    {false, A2, {D_NNE, D_ENE, D_ESE, D_NONE}},
    {false, B2, {D_NNE, D_ENE, D_ESE, D_NNW, D_NONE}},
    {false, C2, {D_NNE, D_ENE, D_ESE, D_WSW, D_WNW, D_NNW, D_NONE}},
    {false, A3, {D_NNE, D_ENE, D_ESE, D_SSE, D_NONE}},
    {false, B3, {D_NNE, D_ENE, D_ESE, D_SSE, D_SSW, D_NNW, D_NONE}},
    {false, C3, {D_NNE, D_ENE, D_ESE, D_SSE, D_SSW, D_WSW, D_WNW, D_NNW, D_NONE}},
    {false, H2, {D_WSW, D_WNW, D_NNW, D_NONE}},
    {false, H6, {D_SSW, D_WSW, D_WNW, D_NNW, D_NONE}},
    {false, H8, {D_SSW, D_WSW, D_NONE}},
    {false, E7, {D_ENE, D_ESE, D_SSE, D_SSW, D_WSW, D_WNW, D_NONE}},
    {false, B8, {D_ESE, D_SSE, D_SSW, D_NONE}},
    {true, 0, {0}}
  };
  CtRays rays = ct_rays_new_knight();

  ut_rays_run_tests(rays, tests);
  ct_rays_free(rays);
} END_TEST

START_TEST(ut_rays_can_continue_through)
{
  CtRays rays = ct_rays_new_queen();
  CtDirection direction;
  CtSquare to;
  CtSquare answers[] = {E8, H8, H5, H2, E1, A1, A5, B8};
  CtSquare *answer = answers;

  ct_rays_from(rays, E5);
  while ((direction = ct_rays_next_direction(rays)) != D_NONE)
  {
    to = E5;
    do
    {
      to += direction;
    } while (ct_rays_can_continue_through(rays, to));
    ck_assert_int_eq(to, *answer);
    answer++;
  }
  ct_rays_free(rays);
} END_TEST

Suite *
ut_rays_make_suite(void)
{
  Suite *test_suite;
  TCase *test_case;

  test_suite = suite_create("ut_rays");
  test_case = tcase_create("Rays");
  tcase_add_test(test_case, ut_rays_king);
  tcase_add_test(test_case, ut_rays_queen);
  tcase_add_test(test_case, ut_rays_rook);
  tcase_add_test(test_case, ut_rays_bishop);
  tcase_add_test(test_case, ut_rays_knight);
  tcase_add_test(test_case, ut_rays_can_continue_through);
  suite_add_tcase(test_suite, test_case);
  return test_suite;
}
