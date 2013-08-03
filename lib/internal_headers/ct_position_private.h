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

#ifndef CT_POSITION_PRIVATE_H
#define CT_POSITION_PRIVATE_H

#include "ct_types.h"

typedef struct CtPositionStruct
{
  CtPiece pieces[NUMBER_OF_SQUARES];
  bool is_white_to_move;
  CtFile en_passant;
  CtCastleRights castle;
  CtBitBoard bit_board_array[CT_BIT_BOARD_ARRAY_LENGTH];
} CtPositionStruct;

#endif                                /* CT_POSITION_PRIVATE_H */
