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

#ifndef CT_MOVE_H
#define CT_MOVE_H

#include "ct_types.h"

CtMove ct_move_make(CtSquare from, CtSquare to);

CtMove ct_move_make_castle_kingside(CtSquare from);
CtMove ct_move_make_castle_queenside(CtSquare from);
CtMove ct_move_make_en_passant_possible(CtSquare from, CtSquare to);
CtMove ct_move_make_en_passant_capture(CtSquare from, CtSquare to);

CtMove ct_move_make_promotion_Q(CtSquare from, CtSquare to);
CtMove ct_move_make_promotion_R(CtSquare from, CtSquare to);
CtMove ct_move_make_promotion_B(CtSquare from, CtSquare to);
CtMove ct_move_make_promotion_N(CtSquare from, CtSquare to);
CtMove ct_move_make_promotion_q(CtSquare from, CtSquare to);
CtMove ct_move_make_promotion_r(CtSquare from, CtSquare to);
CtMove ct_move_make_promotion_b(CtSquare from, CtSquare to);
CtMove ct_move_make_promotion_n(CtSquare from, CtSquare to);

CtMoveType ct_move_type(CtMove move);
CtSquare ct_move_from(CtMove move);
CtSquare ct_move_to(CtMove move);
CtPiece ct_move_promotes_to(CtMove move);

/* ct_move_to_s is defined in ct_move_writer.c */
enum
{
  CT_MOVE_TO_S_MAX_LENGTH = 9
};
char *ct_move_to_s(CtMove move, char *destination);

#endif                                /* CT_MOVE_H */
