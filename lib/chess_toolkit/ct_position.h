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

#ifndef CT_POSITION_H
#define CT_POSITION_H

#include "ct_types.h"

CtPosition ct_position_new();
void ct_position_free(CtPosition position);

void ct_position_clear(CtPosition position);
void ct_position_reset(CtPosition position);

void ct_position_copy(CtPosition destination, CtPosition source);

CtPiece ct_position_get_piece(CtPosition position, CtSquare square);
void ct_position_set_piece(CtPosition position, CtSquare square, CtPiece piece);
void ct_position_for_each_piece(CtPosition position, CtPieceCommand piece_command);
void ct_position_for_each_active_piece(CtPosition position, CtPieceCommand piece_command);
CtBitBoard ct_position_get_bit_board(CtPosition position, CtPiece piece);

bool ct_position_is_white_to_move(CtPosition position);
void ct_position_set_white_to_move(CtPosition position);
void ct_position_set_black_to_move(CtPosition position);
void ct_position_change_turns(CtPosition position);

CtFile ct_position_get_en_passant(CtPosition position);
void ct_position_set_en_passant(CtPosition position, CtFile file);
void ct_position_clear_en_passant(CtPosition position);

CtCastleRights ct_position_get_castle(CtPosition position);
void ct_position_set_castle(CtPosition position, CtCastleRights castle_rights);

/* these are defined in ct_position_rules.c */
bool ct_position_is_legal(CtPosition position);
bool ct_position_is_check(CtPosition position);
CtCastleRights ct_position_can_castle(CtPosition position);

/* ct_position_to_s is defined in ct_position_to_s.c */
enum
{
  CT_POSITION_TO_S_MAX_LENGTH = 194
};
char *ct_position_to_s(CtPosition position, char *destination);

/* ct_position_hash is defined in ct_position_hash.c */
int64_t ct_position_hash(CtPosition position);

/* ct_position_to_fen is defined in ct_position_to_fen.c */
enum
{
  CT_FEN_MAX_LENGTH = 82
};
char *ct_position_to_fen(CtPosition position, char *destination);

/* ct_position_from_fen is defined in ct_position_from_fen.c */
CtPosition ct_position_from_fen(CtPosition position, char *fen);

#endif                                /* CT_POSITION_H */
