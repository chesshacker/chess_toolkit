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

#include "ruby.h"
#include "ruby/io.h"
#include "chess_toolkit.h"
#include "chess_toolkit_rb.h"

static VALUE cCtPosition;
static VALUE cCtMove;
static VALUE cCtGameTags;
static VALUE cCtGraph;

void
Init_chess_toolkit()
{
  chess_toolkit_init();
  cCtPosition = Init_ct_position();
  cCtMove = Init_ct_move();
  cCtGameTags = Init_ct_game_tags();
  cCtGraph = Init_ct_graph();
}
