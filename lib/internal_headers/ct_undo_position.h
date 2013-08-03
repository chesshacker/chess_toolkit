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

#ifndef CT_UNDO_POSITION_H
#define CT_UNDO_POSITION_H

#include "ct_types.h"
#include "ct_types_internal.h"

CtUndoPosition ct_undo_position_new(CtPosition position);
void ct_undo_position_free(CtUndoPosition undo_position);

void ct_undo_position_reset(CtUndoPosition undo_position);
void ct_undo_position_start(CtUndoPosition undo_position);
void ct_undo_position_undo(CtUndoPosition undo_position);

void ct_undo_position_set_piece(CtUndoPosition undo_position, CtSquare square, CtPiece value);
void ct_undo_position_set_en_passant(CtUndoPosition undo_position, CtFile file);
void ct_undo_position_set_castle(CtUndoPosition undo_position, CtCastleRights value);
void ct_undo_position_move_piece(CtUndoPosition undo_position, CtSquare from, CtSquare to);

#endif                                /* CT_UNDO_POSITION_H */
