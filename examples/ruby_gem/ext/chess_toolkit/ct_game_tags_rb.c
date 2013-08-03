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

static VALUE cCtGameTags;

static VALUE ct_game_tags_new_rb(VALUE class);
static VALUE ct_graph_inspect_rb(VALUE self);
static VALUE ct_game_tags_inspect_rb(VALUE self);
static VALUE ct_game_tags_get_rb(VALUE self, VALUE key_value);
static VALUE ct_game_tags_set_rb(VALUE self, VALUE key_value, VALUE value_value);

VALUE
Init_ct_game_tags(void)
{
  cCtGameTags = rb_define_class("CtGameTags", rb_cObject);
  rb_define_singleton_method(cCtGameTags, "new", ct_game_tags_new_rb, 0);
  rb_define_method(cCtGameTags, "inspect", ct_game_tags_inspect_rb, 0);
  rb_define_method(cCtGameTags, "get", ct_game_tags_get_rb, 1);
  rb_define_method(cCtGameTags, "set", ct_game_tags_set_rb, 2);
  return cCtGameTags;
}

CtGameTags
ct_game_tags_from_ruby_game_tags(VALUE game_tags_value)
{
  if (game_tags_value == Qnil)
    return 0;
  if (CLASS_OF(game_tags_value) != cCtGameTags)
  {
    rb_raise(rb_eException, "expecting CtGameTags object");
    return 0;
  }
  return (CtGameTags) DATA_PTR(game_tags_value);
}

static VALUE
ct_game_tags_new_rb(VALUE class)
{
  VALUE result;
  CtGameTags game_tags;

  game_tags = ct_game_tags_new();
  result = Data_Wrap_Struct(cCtGameTags, 0, ct_game_tags_free, game_tags);
  rb_obj_call_init(result, 0, 0);
  return result;
}

static VALUE
ct_game_tags_inspect_rb(VALUE self)
{
  CtGameTags game_tags;
  char *result;
  VALUE result_value;

  game_tags = (CtGameTags) DATA_PTR(self);
  result = ct_graph_to_new_pgn(0, game_tags);
  result_value = rb_str_new2(result);
  free(result);
  return result_value;
}

static VALUE
ct_game_tags_get_rb(VALUE self, VALUE key_value)
{
  CtGameTags game_tags;
  char *key;
  char *result;

  game_tags = (CtGameTags) DATA_PTR(self);
  key = StringValueCStr(key_value);
  result = ct_game_tags_get(game_tags, key);
  return rb_str_new2(result);
}

static VALUE
ct_game_tags_set_rb(VALUE self, VALUE key_value, VALUE value_value)
{
  CtGameTags game_tags;
  char *key;
  char *value;

  game_tags = (CtGameTags) DATA_PTR(self);
  key = StringValueCStr(key_value);
  value = StringValueCStr(value_value);
  ct_game_tags_set(game_tags, key, value);
  return self;
}
