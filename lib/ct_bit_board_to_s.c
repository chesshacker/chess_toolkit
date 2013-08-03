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
#include "ct_bit_board.h"
#include "ct_square.h"
#include "ct_error.h"
#include <stdio.h>
#include <string.h>

static char default_destination[CT_BIT_BOARD_TO_S_MAX_LENGTH];

char *
ct_bit_board_to_s(CtBitBoard bit_board, char *destination)
{
  char *ct;
  CtRank rank;
  CtFile file;
  unsigned long upper, lower;

  if (destination == 0)
    destination = default_destination;
  upper = (bit_board >> 32) & 0xFFFFFFFF;
  lower = bit_board & 0xFFFFFFFF;
  sprintf(destination, "0x%08lX%08lX\n", upper, lower);
  ct = destination + strlen(destination);
  for (rank = LAST_RANK; rank >= RANK_1; rank--)
  {
    *ct++ = ' ';
    *ct++ = ct_rank_to_char(rank);
    *ct++ = ' ';
    for (file = FILE_A; file <= LAST_FILE; file++)
    {
      *ct++ = ' ';
      *ct++ = (ct_bit_board_make(ct_square_make(file, rank)) & bit_board) ? 'x' : '.';
    }
    *ct++ = '\n';
  }
  *ct++ = ' ';
  *ct++ = ' ';
  *ct++ = ' ';
  for (file = FILE_A; file <= LAST_FILE; file++)
  {
    *ct++ = ' ';
    *ct++ = ct_file_to_char(file);
  }
  *ct = '\0';
  if (ct - destination >= CT_BIT_BOARD_TO_S_MAX_LENGTH)
    ct_error("ct_bit_board_to_s: CT_BIT_BOARD_TO_S_MAX_LENGTH is not big enough");
  return destination;
}
