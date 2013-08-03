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

#ifndef CT_PIECE_H
#define CT_PIECE_H

#include "ct_types.h"

CtPieceColor ct_piece_color(CtPiece piece);

CtPiece ct_piece_from_char(char piece_char);
char ct_piece_to_char(CtPiece piece);

CtPiece ct_piece_to_white(CtPiece piece);
CtPiece ct_piece_to_black(CtPiece piece);
bool ct_piece_is_pawn(CtPiece piece);

#endif                                /* CT_PIECE_H */
