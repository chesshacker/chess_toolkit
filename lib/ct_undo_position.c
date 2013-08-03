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
#include "ct_undo_position.h"
#include "ct_position.h"
#include "ct_utilities.h"

enum
{
  CT_UNDO_STACK_DEFAULT_SIZE = 128
};

typedef enum CtOperation
{
  START_OPERATION = -2,
  DO_NOTHING,
  SET_A1, SET_B1, SET_C1, SET_D1, SET_E1, SET_F1, SET_G1, SET_H1,
  SET_A2, SET_B2, SET_C2, SET_D2, SET_E2, SET_F2, SET_G2, SET_H2,
  SET_A3, SET_B3, SET_C3, SET_D3, SET_E3, SET_F3, SET_G3, SET_H3,
  SET_A4, SET_B4, SET_C4, SET_D4, SET_E4, SET_F4, SET_G4, SET_H4,
  SET_A5, SET_B5, SET_C5, SET_D5, SET_E5, SET_F5, SET_G5, SET_H5,
  SET_A6, SET_B6, SET_C6, SET_D6, SET_E6, SET_F6, SET_G6, SET_H6,
  SET_A7, SET_B7, SET_C7, SET_D7, SET_E7, SET_F7, SET_G7, SET_H7,
  SET_A8, SET_B8, SET_C8, SET_D8, SET_E8, SET_F8, SET_G8, SET_H8,
  SET_EN_PASSANT,
  CLEAR_EN_PASSANT,
  SET_CASTLE,
} CtOperation;

typedef struct CtUndoStruct *CtUndo;
typedef struct CtUndoStruct
{
  CtOperation operation;
  union
  {
    int value;
    CtPiece piece;
    CtFile file;
    CtCastleRights castle;
  };
} CtUndoStruct;

typedef struct CtUndoPositionStruct
{
  CtPosition position;
  CtUndoStruct *stack;
  CtUndo stack_pointer;
  CtUndo end_of_stack;
  bool was_clear;
} CtUndoPositionStruct;

static CtUndoStruct nothing_to_undo_struct = {DO_NOTHING, 0};
static CtUndo nothing_to_undo = &nothing_to_undo_struct;

static void ct_undo_position_clear_en_passant(CtUndoPosition undo_position);

static void ct_undo_position_push(CtUndoPosition undo_position, CtOperation operation, int value);
static CtUndo ct_undo_position_pop(CtUndoPosition undo_position);

CtUndoPosition
ct_undo_position_new(CtPosition position)
{
  CtUndoPosition undo_position;

  undo_position = ct_malloc(sizeof(CtUndoPositionStruct));
  undo_position->stack = ct_malloc(sizeof(CtUndoStruct) * CT_UNDO_STACK_DEFAULT_SIZE);
  undo_position->position = position;
  undo_position->end_of_stack = undo_position->stack + CT_UNDO_STACK_DEFAULT_SIZE;
  ct_undo_position_reset(undo_position);
  return undo_position;
}

void
ct_undo_position_free(CtUndoPosition undo_position)
{
  ct_free(undo_position);
}

void
ct_undo_position_reset(CtUndoPosition undo_position)
{
  undo_position->stack_pointer = undo_position->stack;
  undo_position->was_clear = true;
}

void
ct_undo_position_start(CtUndoPosition undo_position)
{
  ct_position_change_turns(undo_position->position);
  ct_undo_position_push(undo_position, START_OPERATION, 0);
  ct_undo_position_clear_en_passant(undo_position);
}

void
ct_undo_position_undo(CtUndoPosition undo_position)
{
  CtUndo undo;
  CtOperation undo_operation;
  CtPosition position = undo_position->position;
  bool done = false;

  while (!done)
  {
    undo = ct_undo_position_pop(undo_position);
    undo_operation = undo->operation;
    switch (undo_operation)
    {
    case START_OPERATION:
      ct_position_change_turns(position);
      /* fall through to DO_NOTHING */
    case DO_NOTHING:
      done = true;
      break;
    case CLEAR_EN_PASSANT:
      ct_position_clear_en_passant(position);
      break;
    case SET_EN_PASSANT:
      ct_position_set_en_passant(position, undo->file);
      break;
    case SET_CASTLE:
      ct_position_set_castle(position, undo->castle);
      break;
    default:
      ct_position_set_piece(position, undo_operation, undo->piece);
    }
  }
}

void
ct_undo_position_set_piece(CtUndoPosition undo_position, CtSquare square, CtPiece new_value)
{
  CtPiece old_value;
  CtPosition position = undo_position->position;

  old_value = ct_position_get_piece(position, square);
  ct_undo_position_push(undo_position, square, old_value);
  ct_position_set_piece(position, square, new_value);
}

static void
ct_undo_position_clear_en_passant(CtUndoPosition undo_position)
{
  CtFile old_value;
  CtPosition position = undo_position->position;

  old_value = ct_position_get_en_passant(position);
  if (old_value == NO_EN_PASSANT)
    undo_position->was_clear = true;
  else
  {
    undo_position->was_clear = false;
    ct_undo_position_push(undo_position, SET_EN_PASSANT, old_value);
    ct_position_clear_en_passant(position);
  }
}

void
ct_undo_position_set_en_passant(CtUndoPosition undo_position, CtFile file)
{
  CtPosition position = undo_position->position;

  if (undo_position->was_clear)
    ct_undo_position_push(undo_position, CLEAR_EN_PASSANT, 0);
  ct_position_set_en_passant(position, file);
}

void
ct_undo_position_set_castle(CtUndoPosition undo_position, CtCastleRights new_value)
{
  CtCastleRights old_value;
  CtPosition position = undo_position->position;

  old_value = ct_position_get_castle(position);
  ct_undo_position_push(undo_position, SET_CASTLE, old_value);
  ct_position_set_castle(position, new_value);
}

void
ct_undo_position_move_piece(CtUndoPosition undo_position, CtSquare from, CtSquare to)
{
  CtPiece from_piece, to_piece;
  CtPosition position = undo_position->position;

  from_piece = ct_position_get_piece(position, from);
  to_piece = ct_position_get_piece(position, to);
  ct_undo_position_push(undo_position, from, from_piece);
  ct_undo_position_push(undo_position, to, to_piece);
  ct_position_set_piece(position, to, from_piece);
  ct_position_set_piece(position, from, EMPTY);
}

static void
ct_undo_position_push(CtUndoPosition undo_position, CtOperation operation, int value)
{
  CtUndo undo = undo_position->stack_pointer;

  if (undo == undo_position->end_of_stack)
  {
    int new_size = 2 * (undo_position->end_of_stack - undo_position->stack);
    CtUndo new_stack = ct_realloc(undo_position->stack, new_size * sizeof(CtUndoStruct));

    undo_position->stack_pointer += new_stack - undo_position->stack;
    undo_position->end_of_stack = new_stack + new_size;
    undo_position->stack = new_stack;
    undo = undo_position->stack_pointer;
  }
  undo->operation = operation;
  undo->value = value;
  undo_position->stack_pointer++;
}

static CtUndo
ct_undo_position_pop(CtUndoPosition undo_position)
{
  CtUndo result;

  if (undo_position->stack_pointer > undo_position->stack)
  {
    undo_position->stack_pointer--;
    result = undo_position->stack_pointer;
  }
  else
    result = nothing_to_undo;

  return result;
}
