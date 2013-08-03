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
#include "ct_utilities.h"
#include "ct_square.h"
#include "ct_piece.h"
#include "ct_rays.h"
#include <string.h>

enum
{
  SIZE_OF_BIT_BOARD_ARRAY = sizeof(CtBitBoard) * CT_BIT_BOARD_ARRAY_LENGTH,
  SIZE_OF_BIT_BOARD_ATTACKS = sizeof(CtBitBoard) * NUMBER_OF_SQUARES
};

static bool ct_bit_board_is_attacked_by_line_piece(CtSquare square, CtBitBoard attackers, CtBitBoard occupied);

static CtBitBoard king_attacks[NUMBER_OF_SQUARES];
static CtBitBoard knight_attacks[NUMBER_OF_SQUARES];
static CtBitBoard white_pawn_attacks[NUMBER_OF_SQUARES];
static CtBitBoard black_pawn_attacks[NUMBER_OF_SQUARES];
static CtBitBoard queen_attacks[NUMBER_OF_SQUARES];
static CtBitBoard rook_attacks[NUMBER_OF_SQUARES];
static CtBitBoard bishop_attacks[NUMBER_OF_SQUARES];
static CtBitBoard blockers[NUMBER_OF_SQUARES][NUMBER_OF_SQUARES];

static void ct_bit_board_init_clear_all(void);
static void ct_bit_board_init_pawn_attacks(CtBitBoard * destination, CtDirection queenside, CtDirection kingside, CtRank except_on);
static void ct_bit_board_init_steper_attacks(CtBitBoard * destination, CtRays rays);
static void ct_bit_board_init_slider_attacks(CtBitBoard * destination, CtRays rays);
static void ct_bit_board_init_queen_attacks_from_rook_and_bishop(void);

void
ct_bit_board_init(void)
{
  CtRays rays;

  ct_bit_board_init_clear_all();

  ct_bit_board_init_pawn_attacks(white_pawn_attacks, D_SW, D_SE, RANK_1);
  ct_bit_board_init_pawn_attacks(black_pawn_attacks, D_NW, D_NE, RANK_8);

  rays = ct_rays_new_king();
  ct_bit_board_init_steper_attacks(king_attacks, rays);
  ct_rays_free(rays);

  rays = ct_rays_new_knight();
  ct_bit_board_init_steper_attacks(knight_attacks, rays);
  ct_rays_free(rays);

  rays = ct_rays_new_rook();
  ct_bit_board_init_slider_attacks(rook_attacks, rays);
  ct_rays_free(rays);

  rays = ct_rays_new_bishop();
  ct_bit_board_init_slider_attacks(bishop_attacks, rays);
  ct_rays_free(rays);

  ct_bit_board_init_queen_attacks_from_rook_and_bishop();
}

static void
ct_bit_board_init_clear_all(void)
{
  CtBitBoard *bit_board_attacks_array[] = {
    king_attacks, knight_attacks, white_pawn_attacks, black_pawn_attacks, queen_attacks, rook_attacks, 0
  };
  CtBitBoard **bit_board_attacks = bit_board_attacks_array;

  while (*bit_board_attacks != 0)
    memset(*bit_board_attacks++, 0, SIZE_OF_BIT_BOARD_ATTACKS);
  memset(blockers, 0, sizeof(blockers));
}

static void
ct_bit_board_init_pawn_attacks(CtBitBoard * destination, CtDirection queenside, CtDirection kingside, CtRank except_on)
{
  CtBitBoard *bit_board_ptr;
  CtSquare from;

  bit_board_ptr = destination;
  for (from = 0; from < NUMBER_OF_SQUARES; from++, bit_board_ptr++)
  {
    if (ct_square_rank(from) != except_on)
    {
      if (ct_square_file(from) != FILE_A)
        *bit_board_ptr |= ct_bit_board_make(from + queenside);
      if (ct_square_file(from) < LAST_FILE)
        *bit_board_ptr |= ct_bit_board_make(from + kingside);
    }
  }
}

static void
ct_bit_board_init_steper_attacks(CtBitBoard * destination, CtRays rays)
{
  CtBitBoard *bit_board_ptr;
  CtSquare from;
  CtDirection direction;

  bit_board_ptr = destination;
  for (from = 0; from < NUMBER_OF_SQUARES; from++, bit_board_ptr++)
  {
    ct_rays_from(rays, from);
    while (direction = ct_rays_next_direction(rays))
      *bit_board_ptr |= ct_bit_board_make(from + direction);
  }
}

static void
ct_bit_board_init_slider_attacks(CtBitBoard * destination, CtRays rays)
{
  CtBitBoard *bit_board_ptr;
  CtBitBoard next_blockers;
  CtSquare from, to;
  CtDirection direction;

  bit_board_ptr = destination;
  for (from = 0; from < NUMBER_OF_SQUARES; from++, bit_board_ptr++)
  {
    ct_rays_from(rays, from);
    while (direction = ct_rays_next_direction(rays))
    {
      to = from;
      next_blockers = BITB_EMPTY;
      do
      {
        to += direction;
        *bit_board_ptr |= ct_bit_board_make(to);
        blockers[from][to] |= next_blockers;
        next_blockers |= ct_bit_board_make(to);
      } while (ct_rays_can_continue_through(rays, to));
    }
  }
}

static void
ct_bit_board_init_queen_attacks_from_rook_and_bishop(void)
{
  CtBitBoard *bit_board_ptr;
  CtSquare from;

  bit_board_ptr = queen_attacks;
  for (from = 0; from < NUMBER_OF_SQUARES; from++, bit_board_ptr++)
    *bit_board_ptr = rook_attacks[from] | bishop_attacks[from];
}

CtSquare
ct_bit_board_find_first_square(CtBitBoard bit_board)
{
  int result;

  result = ffsl((long) bit_board);
  if (result)
    return result - 1;
  result = ffsl((long) bit_board >> 32);
  if (result)
    return result + 31;
  return SQUARE_NOT_FOUND;        /* no bit found */
}

void
ct_bit_board_array_reset(CtBitBoardArray bit_board_array)
{
  memset(bit_board_array, 0, SIZE_OF_BIT_BOARD_ARRAY);
  bit_board_array[EMPTY] = BITB_FULL;
}

bool
ct_bit_board_array_is_white_attacking(CtBitBoardArray bit_board_array, CtSquare square)
{
  CtBitBoard attackers;

  if (king_attacks[square] & bit_board_array[WHITE_KING])
    return true;
  if (knight_attacks[square] & bit_board_array[WHITE_KNIGHT])
    return true;
  if (white_pawn_attacks[square] & bit_board_array[WHITE_PAWN])
    return true;
  attackers = queen_attacks[square] & bit_board_array[WHITE_QUEEN];
  attackers |= rook_attacks[square] & bit_board_array[WHITE_ROOK];
  attackers |= bishop_attacks[square] & bit_board_array[WHITE_BISHOP];
  return ct_bit_board_is_attacked_by_line_piece(square, attackers, ~bit_board_array[EMPTY]);
}

bool
ct_bit_board_array_is_black_attacking(CtBitBoardArray bit_board_array, CtSquare square)
{
  CtBitBoard attackers;

  if (king_attacks[square] & bit_board_array[BLACK_KING])
    return true;
  if (knight_attacks[square] & bit_board_array[BLACK_KNIGHT])
    return true;
  if (black_pawn_attacks[square] & bit_board_array[BLACK_PAWN])
    return true;
  attackers = queen_attacks[square] & bit_board_array[BLACK_QUEEN];
  attackers |= rook_attacks[square] & bit_board_array[BLACK_ROOK];
  attackers |= bishop_attacks[square] & bit_board_array[BLACK_BISHOP];
  return ct_bit_board_is_attacked_by_line_piece(square, attackers, ~bit_board_array[EMPTY]);
}

static bool
ct_bit_board_is_attacked_by_line_piece(CtSquare square, CtBitBoard attackers, CtBitBoard occupied)
{
  if (attackers)
  {
    CtSquare attacking_from = 0;

    do
    {
      if (attackers & 1)
      {
        if ((blockers[attacking_from][square] & occupied) == 0)
          return true;
      }
      attacking_from++;
    }
    while (attackers >>= 1);
  }
  return false;
}
