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

#ifndef CHECK_MG_PIECE_H
#define CHECK_MG_PIECE_H

#include <check.h>
#include "ct_position.h"

/* ut_pawn, ut_steper, ut_slider share the same sort of tests... */

typedef struct CheckMgPieceTestStruct *CheckMgPieceTest;
typedef struct CheckMgPieceTestStruct
{
  bool end_of_tests;
  char fen[CT_FEN_MAX_LENGTH];
  CtSquare from;
  CtSquare answers[28];
} CheckMgPieceTestStruct;

void check_mg_piece_setup(void);
void check_mg_piece_teardown(void);
CtMoveCommand check_mg_piece_move_command(void);
void check_mg_piece_run_all_tests(void *white_mg_piece, void *black_mg_piece, CheckMgPieceTest tests);

#endif                                /* CHECK_MG_PIECE_H */
