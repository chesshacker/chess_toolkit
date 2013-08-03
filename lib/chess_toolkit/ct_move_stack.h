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

#ifndef CT_MOVE_STACK_H
#define CT_MOVE_STACK_H

#include "ct_types.h"

CtMoveStack ct_move_stack_new();
void ct_move_stack_free(CtMoveStack move_stack);

void ct_move_stack_push(CtMoveStack move_stack, CtMove move);
CtMove ct_move_stack_pop(CtMoveStack move_stack);

void ct_move_stack_reset(CtMoveStack move_stack);
bool ct_move_stack_is_empty(CtMoveStack move_stack);
unsigned int ct_move_stack_length(CtMoveStack move_stack);

CtMoveCommand ct_move_stack_push_command(CtMoveStack move_stack);
void ct_move_stack_for_each(CtMoveStack move_stack, CtMoveCommand move_command);

#endif                                /* CT_MOVE_STACK_H */
