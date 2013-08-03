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
#include "ct_move_stack.h"
#include "ct_utilities.h"
#include "ct_move.h"
#include "ct_move_command.h"

enum
{
  CT_MOVE_STACK_DEFAULT_SIZE = 256
};

typedef struct CtMoveStackStruct
{
  CtMove *move_stack;
  CtMove *move_sp;
  CtMove *end_of_move_stack;
  CtMoveCommand push_command;
} CtMoveStackStruct;

CtMoveStack
ct_move_stack_new(void)
{
  CtMoveStack move_stack;

  move_stack = ct_malloc(sizeof(CtMoveStackStruct));
  move_stack->move_stack = ct_malloc(sizeof(CtMove) * CT_MOVE_STACK_DEFAULT_SIZE);
  move_stack->end_of_move_stack = move_stack->move_stack + CT_MOVE_STACK_DEFAULT_SIZE;
  move_stack->push_command = 0;
  ct_move_stack_reset(move_stack);
  return move_stack;
}

void
ct_move_stack_free(CtMoveStack move_stack)
{
  if (move_stack->push_command != 0)
    ct_move_command_free(move_stack->push_command);
  ct_free(move_stack->move_stack);
  ct_free(move_stack);
}

void
ct_move_stack_push(CtMoveStack move_stack, CtMove move)
{
  if (move_stack->move_sp == move_stack->end_of_move_stack)
  {
    int new_size = 2 * (move_stack->end_of_move_stack - move_stack->move_stack);
    CtMove *new_move_stack = ct_realloc(move_stack->move_stack, new_size * sizeof(CtMove));

    move_stack->move_sp += new_move_stack - move_stack->move_stack;
    move_stack->end_of_move_stack = new_move_stack + new_size;
    move_stack->move_stack = new_move_stack;
  }
  *move_stack->move_sp = move;
  move_stack->move_sp++;
}

CtMove
ct_move_stack_pop(CtMoveStack move_stack)
{
  CtMove result;

  if (move_stack->move_sp > move_stack->move_stack)
  {
    move_stack->move_sp--;
    result = *move_stack->move_sp;
  }
  else
    result = NULL_MOVE;
  return result;
}

void
ct_move_stack_reset(CtMoveStack move_stack)
{
  move_stack->move_sp = move_stack->move_stack;
}

bool
ct_move_stack_is_empty(CtMoveStack move_stack)
{
  return move_stack->move_sp == move_stack->move_stack;
}

unsigned int
ct_move_stack_length(CtMoveStack move_stack)
{
  return move_stack->move_sp - move_stack->move_stack;
}

CtMoveCommand
ct_move_stack_push_command(CtMoveStack move_stack)
{
  if (move_stack->push_command == 0)
    move_stack->push_command = ct_move_command_new(move_stack, (CtMoveCommandMethod) ct_move_stack_push);
  /* small cheat with the casting, but there's no point in writing a separate method that takes void * instead of
     CtMoveStack */
  return move_stack->push_command;
}

void
ct_move_stack_for_each(CtMoveStack move_stack, CtMoveCommand move_command)
{
  CtMove *move_ptr;
  CtMove *stop_before = move_stack->move_sp;

  for (move_ptr = move_stack->move_stack; move_ptr < stop_before; move_ptr++)
    ct_move_command_execute(move_command, *move_ptr);
}
