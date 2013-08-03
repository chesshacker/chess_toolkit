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

#ifndef CT_BIT_BOARD_H
#define CT_BIT_BOARD_H

#include "ct_types.h"

#define BITB_EMPTY ((CtBitBoard) 0)
#define BITB_ONE ((CtBitBoard) 1)
#define BITB_FULL ((CtBitBoard) 0xFFFFFFFFFFFFFFFF)

/* making this inline appears to save significant time on the long perft test */
static inline CtBitBoard
ct_bit_board_make(CtSquare square)
{
  return BITB_ONE << square;
}

/* if bit_board is empty, it returns SQUARE_NOT_FOUND (-1) */
CtSquare ct_bit_board_find_first_square(CtBitBoard bit_board);

/* ct_bit_board_to_s is defined in ct_bit_board_to_s.c */
enum
{
  CT_BIT_BOARD_TO_S_MAX_LENGTH = 199
};
char *ct_bit_board_to_s(CtBitBoard bit_board, char *destination);

void ct_bit_board_array_reset(CtBitBoardArray bit_board_array);

bool ct_bit_board_array_is_white_attacking(CtBitBoardArray bit_board_array, CtSquare square);
bool ct_bit_board_array_is_black_attacking(CtBitBoardArray bit_board_array, CtSquare square);

#endif                                /* CT_BIT_BOARD_H */
