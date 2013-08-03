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
#include "ct_move_command.h"
#include "ct_utilities.h"

CtMoveCommand
ct_move_command_new(void *delegate, CtMoveCommandMethod method)
{
  CtMoveCommand command;

  command = ct_malloc(sizeof(CtMoveCommandStruct));
  command->delegate = delegate;
  command->method = method;
  return command;
}

void
ct_move_command_free(CtMoveCommand command)
{
  ct_free(command);
}

CtMoveCommandStruct
ct_move_command_make(void *delegate, CtMoveCommandMethod method)
{
  CtMoveCommandStruct result = {delegate, method};
  return result;
}

void
ct_move_command_execute(CtMoveCommand command, CtMove move)
{
  command->method(command->delegate, move);
}
