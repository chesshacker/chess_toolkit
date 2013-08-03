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
#include "ct_rays.h"
#include "ct_utilities.h"
#include "ct_square.h"
#include <strings.h>

typedef struct CtRaysStruct
{
  unsigned char *directions_available;
  CtDirection *direction_array;
  unsigned int directions_remaining;
  unsigned int bit;
} CtRaysStruct;

static unsigned char king_directions_available[64];
static unsigned char knight_directions_available[64];
static unsigned char rook_directions_available[64];
static unsigned char bishop_directions_available[64];

static CtDirection adjacent_direction_array[] = {
  D_N, D_NE, D_E, D_SE, D_S, D_SW, D_W, D_NW
};

static CtDirection knight_direction_array[] = {
  D_NNE, D_ENE, D_ESE, D_SSE, D_SSW, D_WSW, D_WNW, D_NNW
};

enum
{
  ROOK_MASK = 0x55,
  BISHOP_MASK = 0xAA,
};

CtRays ct_rays_new(unsigned char *directions_available, CtDirection *direction_array);

void
ct_rays_init(void)
{
  CtSquare square;
  CtRank rank;
  CtFile file;

  for (square = 0; square < NUMBER_OF_SQUARES; square++)
  {
    king_directions_available[square] = 0xFF;
    knight_directions_available[square] = 0xFF;
  }
  for (rank = RANK_1; rank <= LAST_RANK; rank++)
  {
    king_directions_available[ct_square_make(FILE_A, rank)] &= 0x1F;
    king_directions_available[ct_square_make(LAST_FILE, rank)] &= 0xF1;

    knight_directions_available[ct_square_make(FILE_A, rank)] &= 0x0F;
    knight_directions_available[ct_square_make(FILE_B, rank)] &= 0x9F;
    knight_directions_available[ct_square_make(LAST_FILE - 1, rank)] &= 0xF9;
    knight_directions_available[ct_square_make(LAST_FILE, rank)] &= 0xF0;
  }
  for (file = FILE_A; file <= LAST_FILE; file++)
  {
    king_directions_available[ct_square_make(file, RANK_1)] &= 0xC7;
    king_directions_available[ct_square_make(file, LAST_RANK)] &= 0x7C;

    knight_directions_available[ct_square_make(file, RANK_1)] &= 0xC3;
    knight_directions_available[ct_square_make(file, RANK_2)] &= 0xE7;
    knight_directions_available[ct_square_make(file, LAST_RANK - 1)] &= 0x7E;
    knight_directions_available[ct_square_make(file, LAST_RANK)] &= 0x3C;
  }
  for (square = 0; square < NUMBER_OF_SQUARES; square++)
  {
    rook_directions_available[square] = king_directions_available[square] & ROOK_MASK;
    bishop_directions_available[square] = king_directions_available[square] & BISHOP_MASK;
  }
}

CtRays
ct_rays_new_king(void)
{
  return ct_rays_new(king_directions_available, adjacent_direction_array);
}

CtRays
ct_rays_new_knight(void)
{
  return ct_rays_new(knight_directions_available, knight_direction_array);
}

CtRays
ct_rays_new_queen(void)
{
  /* king and queen move in the same directions */
  return ct_rays_new(king_directions_available, adjacent_direction_array);
}

CtRays
ct_rays_new_rook(void)
{
  return ct_rays_new(rook_directions_available, adjacent_direction_array);
}

CtRays
ct_rays_new_bishop(void)
{
  return ct_rays_new(bishop_directions_available, adjacent_direction_array);
}

CtRays
ct_rays_new(unsigned char *directions_available, CtDirection *direction_array)
{
  CtRays rays;

  rays = ct_malloc(sizeof(CtRaysStruct));
  rays->directions_available = directions_available;
  rays->direction_array = direction_array;
  rays->directions_remaining = 0;
  rays->bit = 0;
  return rays;
}

void
ct_rays_free(CtRays rays)
{
  ct_free(rays);
}

void
ct_rays_from(CtRays rays, CtSquare square)
{
  rays->directions_remaining = rays->directions_available[square];
}

CtDirection
ct_rays_next_direction(CtRays rays)
{
  if (rays->directions_remaining)
  {
    int index = ffs(rays->directions_remaining) - 1;

    rays->bit = 1 << index;
    rays->directions_remaining &= ~rays->bit;
    return rays->direction_array[index];
  }
  return D_NONE;
}

bool
ct_rays_can_continue_through(CtRays rays, CtSquare square)
{
  return (rays->bit & rays->directions_available[square]) != 0;
}
