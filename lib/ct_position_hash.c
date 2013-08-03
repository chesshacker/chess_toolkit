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
#include "ct_position.h"
#include "ct_position_private.h"
#include "ct_utilities.h"
#include <stdlib.h>

/* (PIECE_MAX_VALUE + 1 = 16) * (NUMBER_OF_SQUARES = 64) + (2 possible turns) + (16 possible castling states) + (9
   possible en passant states) */

enum
{
  NUMBER_OF_PIECE_KEYS = NUMBER_OF_SQUARES * (PIECE_MAX_VALUE + 1),
  NUMBER_OF_KEYS = NUMBER_OF_PIECE_KEYS + 2 + 16 + 9
};

static int64_t zobrist_keys[NUMBER_OF_KEYS];
static int64_t *keys_for_pieces = &zobrist_keys[0];
static int64_t *keys_for_turns = &zobrist_keys[NUMBER_OF_PIECE_KEYS];
static int64_t *keys_for_castling = &zobrist_keys[NUMBER_OF_PIECE_KEYS + 2];
static int64_t *keys_for_en_passant = &zobrist_keys[NUMBER_OF_PIECE_KEYS + 2 + 16];

void
ct_position_hash_init(void)
{
/* puts Array.new(256){rand(256)}.collect{|x|"x"<<x.to_s(16).upcase.rjust(2,"0")}.join.gsub(/(x..){32,32}/){|m|"\"#{m}\"\n"} */
  char random_state[] =
  "x20xECx7Cx39x02x89x63x61xDEx36x32x58xDFx16x6Cx6Cx48x9BxAEx2Fx6Ex3Ax2Bx4Cx70x46x05xEEx1Ex40xBAx47"
  "x91xF9xAEx83xCFxFFx0AxCBx04xB9x82xBAxBCxD8xA8x66xC8x56xBFx7Bx02x0Bx94x87x18xBBxEAx22x2Bx6Fx5CxCE"
  "xE0x45xAFxAEx37x5Bx71xF0xB3x2CxE1xCCx6Ax66xC2x2Ex63x52x18xA5xB7x60x78xB8x60xDAx13x3ExAAxD8x81x25"
  "x5Ax1Ax66x0CxBDxFCx78x87x43x47x20x9Dx20x0BxA6x73xEEx2Dx04x57xDBxB8xFBx1FxA5x15x41x6Fx01x80x7Ax81"
  "xF0x6Dx93xF7x58xB0x4Cx8Bx12xE5xF8x8AxACxB2x83xA2x83x8Cx49xB7xAExC7x33xDAx66x9CxB7x0BxA1x3AxE9x5A"
  "xBFxA5x52xEFxF3x38x70x3CxEFx39x5Bx71xFExFCxE4xC7xB5x76x5ExFFxADx23x87xE5xD0xCCx7BxD2x6Ax2Fx7DxA2"
  "x70xBAx5Ax8Fx1Ex50xEAx98x59x5BxDFx09xCFx82x4AxB3x56x2Ex32xD2x04x27x10x56x12x5Ax8Cx5Dx35x6Dx85x6A"
  "xEBx4Dx94x61xEDxA2x41x95x4Cx36xA8xCExE5xE8x79x34x7DxFEx87x25x8BxC7xCDxD5xC8x6Ex80x4FxEDx67xB1xB0";
  int key_index;
  int64_t new_key;

  initstate(1, random_state, 256);
  for (key_index = 0; key_index < NUMBER_OF_KEYS; key_index++)
  {
    new_key = random();                /* random returns a long between 0 and (2^31 - 1) */
    new_key <<= 31;
    new_key |= random();        /* new_key is now a 62-bit random number */
    zobrist_keys[key_index] = new_key;
  }
}

/* uses a Zobrist hash */
int64_t
ct_position_hash(CtPosition position)
{
  int64_t result = 0;
  CtPiece *pieces = position->pieces;
  bool is_wtm = ct_position_is_white_to_move(position);
  CtCastleRights castle = ct_position_get_castle(position);
  CtFile en_passant = ct_position_get_en_passant(position);
  CtSquare square;

  for (square = A1; square <= H8; square++, pieces++)
  {
    if (*pieces != EMPTY)
      result ^= keys_for_pieces[*pieces * NUMBER_OF_SQUARES + square];
  }
  result ^= keys_for_turns[is_wtm];
  result ^= keys_for_castling[castle];
  if (en_passant == NO_EN_PASSANT)
    result ^= keys_for_en_passant[0];
  else
    result ^= keys_for_en_passant[en_passant + 1];
  return result;
}
