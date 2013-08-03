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

static VALUE ct_position_new_rb(VALUE class);
static VALUE ct_position_inspect_rb(VALUE self);
static VALUE ct_position_to_fen_rb(VALUE self);
static VALUE ct_position_from_fen_rb(VALUE self, VALUE fen_value);
static VALUE ct_position_hash_rb(VALUE self);
static VALUE ct_position_is_white_to_move_rb(VALUE self);

VALUE
Init_ct_position(void)
{
  cCtPosition = rb_define_class("CtPosition", rb_cObject);
  rb_define_singleton_method(cCtPosition, "new", ct_position_new_rb, 0);
  rb_define_method(cCtPosition, "inspect", ct_position_inspect_rb, 0);
  rb_define_method(cCtPosition, "to_fen", ct_position_to_fen_rb, 0);
  rb_define_method(cCtPosition, "from_fen", ct_position_from_fen_rb, 1);
  rb_define_method(cCtPosition, "hash", ct_position_hash_rb, 0);
  rb_define_method(cCtPosition, "is_white_to_move", ct_position_is_white_to_move_rb, 0);
  return cCtPosition;
}

VALUE
ruby_position_from_ct_position(CtPosition position)
{
  VALUE result;

  result = Data_Wrap_Struct(cCtPosition, 0, ct_position_free, position);
  rb_obj_call_init(result, 0, 0);
  return result;
}

static VALUE
ct_position_new_rb(VALUE class)
{
  CtPosition position = ct_position_new();

  return ruby_position_from_ct_position(position);
}

static VALUE
ct_position_inspect_rb(VALUE self)
{
  CtPosition position;
  char result[CT_POSITION_TO_S_MAX_LENGTH];

  position = (CtPosition) DATA_PTR(self);
  ct_position_to_s(position, result);
  return rb_str_new2(result);
}

static VALUE
ct_position_to_fen_rb(VALUE self)
{
  CtPosition position;
  char result[CT_FEN_MAX_LENGTH];

  position = (CtPosition) DATA_PTR(self);
  ct_position_to_fen(position, result);
  return rb_str_new2(result);
}

static VALUE
ct_position_from_fen_rb(VALUE self, VALUE fen_value)
{
  CtPosition position;
  char *fen;

  position = (CtPosition) DATA_PTR(self);
  fen = StringValueCStr(fen_value);
  position = ct_position_from_fen(position, fen);
  if (!position)
  {
    rb_raise(rb_eException, "could not parse fen");
    return Qnil;
  }
  return self;
}

static VALUE
ct_position_hash_rb(VALUE self)
{
  CtPosition position;
  int64_t hash;

  position = (CtPosition) DATA_PTR(self);
  hash = ct_position_hash(position);
  return LONG2FIX(hash);
}

static VALUE
ct_position_is_white_to_move_rb(VALUE self)
{
  CtPosition position;
  bool is_wtm;

  position = (CtPosition) DATA_PTR(self);
  is_wtm = ct_position_is_white_to_move(position);
  return is_wtm ? Qtrue : Qfalse;
}
