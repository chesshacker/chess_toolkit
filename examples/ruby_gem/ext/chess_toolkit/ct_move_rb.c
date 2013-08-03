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

static VALUE cCtMove;

static VALUE ct_move_inspect_rb(VALUE self);
static VALUE ct_move_compare_rb(VALUE self, VALUE other);

VALUE
Init_ct_move(void)
{
  cCtMove = rb_define_class("CtMove", rb_cObject);
  rb_include_module(cCtMove, rb_mComparable);
  rb_define_method(cCtMove, "inspect", ct_move_inspect_rb, 0);
  rb_define_method(cCtMove, "<=>", ct_move_compare_rb, 1);
  return cCtMove;
}

VALUE
ruby_move_from_ct_move(CtMove move)
{
  VALUE result;
  CtMove *move_ptr;

  if (move == NULL_MOVE || move == AMBIGUOUS_MOVE)
    return Qnil;
  move_ptr = malloc(sizeof(*move_ptr));
  result = Data_Make_Struct(cCtMove, CtMove, 0, free, move_ptr);
  *move_ptr = move;
  rb_obj_call_init(result, 0, 0);
  return result;
}

CtMove
ct_move_from_ruby_move(VALUE move_value)
{
  CtMove *move_ptr;
  if (CLASS_OF(move_value) != cCtMove)
  {
    rb_raise(rb_eException, "expected CtMove");
    return Qnil;
  }
  Data_Get_Struct(move_value, CtMove, move_ptr);
  return *move_ptr;
}

static VALUE
ct_move_inspect_rb(VALUE self)
{
  CtMove *move;
  char result[CT_POSITION_TO_S_MAX_LENGTH];

  Data_Get_Struct(self, CtMove, move);
  ct_move_to_s(*move, result);
  return rb_str_new2(result);
}

static VALUE
ct_move_compare_rb(VALUE self, VALUE other)
{
  CtMove *move, *other_move;

  if (other == Qnil)
    return INT2NUM(1);
  Data_Get_Struct(self, CtMove, move);
  Data_Get_Struct(other, CtMove, other_move);
  if (CLASS_OF(other) != cCtMove)
    rb_raise(rb_eException, "<=> requires a CtMove");
  if (*move == *other_move)
    return INT2NUM(0);
  return (*move > *other_move) ? INT2NUM(1) : INT2NUM(-1);
}
