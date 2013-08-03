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

static VALUE cCtGraph;

static VALUE ct_graph_new_rb(VALUE class);
static VALUE ct_graph_inspect_rb(VALUE self);
static VALUE ct_graph_ply_rb(VALUE self);
static VALUE ct_graph_legal_moves_rb(VALUE self);
static void push_move_to_ruby_array(void *delegate, CtMove move);
static VALUE ct_graph_make_move_rb(VALUE self, VALUE move_value);
static VALUE ct_graph_unmake_move_rb(VALUE self);
static VALUE ct_graph_to_fen_rb(VALUE self);
static VALUE ct_graph_from_fen_rb(VALUE self, VALUE fen_value);
static VALUE ct_graph_position_hash_rb(VALUE self);
static VALUE ct_graph_move_from_san_rb(VALUE self, VALUE san_value);
static VALUE ct_graph_move_to_san_rb(VALUE self, VALUE move_value);
static VALUE ct_graph_for_each_move_made_rb(VALUE self);
static void ct_graph_replay_moves_yield_to(void *ignore, CtMove move);
static VALUE ct_graph_position_rb(VALUE self);
static VALUE ct_graph_to_pgn_rb(int argc, VALUE * argv, VALUE self);
static VALUE ct_graph_from_pgn_rb(int argc, VALUE * argv, VALUE self);
static void ct_yield_to_ruby(void *ignore);

VALUE
Init_ct_graph(void)
{
  cCtGraph = rb_define_class("CtGraph", rb_cObject);
  rb_define_singleton_method(cCtGraph, "new", ct_graph_new_rb, 0);
  rb_define_method(cCtGraph, "inspect", ct_graph_inspect_rb, 0);
  rb_define_method(cCtGraph, "ply", ct_graph_ply_rb, 0);
  rb_define_method(cCtGraph, "legal_moves", ct_graph_legal_moves_rb, 0);
  rb_define_method(cCtGraph, "make_move", ct_graph_make_move_rb, 1);
  rb_define_method(cCtGraph, "unmake_move", ct_graph_unmake_move_rb, 0);
  rb_define_method(cCtGraph, "to_fen", ct_graph_to_fen_rb, 0);
  rb_define_method(cCtGraph, "from_fen", ct_graph_from_fen_rb, 1);
  rb_define_method(cCtGraph, "position_hash", ct_graph_position_hash_rb, 0);
  rb_define_method(cCtGraph, "move_from_san", ct_graph_move_from_san_rb, 1);
  rb_define_method(cCtGraph, "move_to_san", ct_graph_move_to_san_rb, 1);
  rb_define_method(cCtGraph, "for_each_move_made", ct_graph_for_each_move_made_rb, 0);
  rb_define_method(cCtGraph, "position", ct_graph_position_rb, 0);
  rb_define_method(cCtGraph, "to_pgn", ct_graph_to_pgn_rb, -1);
  rb_define_method(cCtGraph, "from_pgn", ct_graph_from_pgn_rb, -1);
  return cCtGraph;
}

static VALUE
ct_graph_new_rb(VALUE class)
{
  VALUE result;
  CtGraph graph;

  graph = ct_graph_new();
  result = Data_Wrap_Struct(cCtGraph, 0, ct_graph_free, graph);
  rb_obj_call_init(result, 0, 0);
  return result;
}

static VALUE
ct_graph_inspect_rb(VALUE self)
{
  CtGraph graph;
  char result[CT_GRAPH_TO_S_MAX_LENGTH];

  graph = (CtGraph) DATA_PTR(self);
  ct_graph_to_s(graph, result);
  return rb_str_new2(result);
}

static VALUE
ct_graph_ply_rb(VALUE self)
{
  CtGraph graph;
  int result;

  graph = (CtGraph) DATA_PTR(self);
  result = ct_graph_ply(graph);
  return INT2FIX(result);
}

static VALUE
ct_graph_legal_moves_rb(VALUE self)
{
  CtGraph graph;
  VALUE result = rb_ary_new();
  CtMoveCommandStruct move_command = ct_move_command_make(&result, push_move_to_ruby_array);

  graph = (CtGraph) DATA_PTR(self);
  ct_graph_for_each_legal_move(graph, &move_command);
  return result;
}

static void
push_move_to_ruby_array(void *delegate, CtMove move)
{
  VALUE *result = (VALUE *) delegate;
  VALUE move_value = ruby_move_from_ct_move(move);

  rb_ary_push(*result, move_value);
}

VALUE
ct_graph_make_move_rb(VALUE self, VALUE move_value)
{
  CtGraph graph;
  CtMove move;

  if (move_value == Qnil)
    return self;
  graph = (CtGraph) DATA_PTR(self);
  move = ct_move_from_ruby_move(move_value);
  if (move == NULL_MOVE)
    return Qnil;
  ct_graph_make_move(graph, move);
  return self;
}

VALUE
ct_graph_unmake_move_rb(VALUE self)
{
  CtGraph graph;
  CtMove move;
  VALUE move_value;

  graph = (CtGraph) DATA_PTR(self);
  move = ct_graph_unmake_move(graph);
  move_value = ruby_move_from_ct_move(move);
  return move_value;
}

static VALUE
ct_graph_to_fen_rb(VALUE self)
{
  CtGraph graph;
  char result[CT_FEN_MAX_LENGTH];

  graph = (CtGraph) DATA_PTR(self);
  ct_graph_to_fen(graph, result);
  return rb_str_new2(result);
}

static VALUE
ct_graph_from_fen_rb(VALUE self, VALUE fen_value)
{
  CtGraph graph;
  char *fen;

  graph = (CtGraph) DATA_PTR(self);
  fen = StringValueCStr(fen_value);
  graph = ct_graph_from_fen(graph, fen);
  if (!graph)
  {
    rb_raise(rb_eException, "could not parse fen");
    return Qnil;
  }
  return self;
}

static VALUE
ct_graph_position_hash_rb(VALUE self)
{
  CtGraph graph;
  int64_t hash;

  graph = (CtGraph) DATA_PTR(self);
  hash = ct_graph_position_hash(graph);
  return LONG2FIX(hash);
}

static VALUE
ct_graph_move_from_san_rb(VALUE self, VALUE san_value)
{
  CtGraph graph;
  char *san;
  CtMove move;

  graph = (CtGraph) DATA_PTR(self);
  san = StringValueCStr(san_value);
  move = ct_graph_move_from_san(graph, san);
  return ruby_move_from_ct_move(move);
}

static VALUE
ct_graph_move_to_san_rb(VALUE self, VALUE move_value)
{
  CtGraph graph;
  CtMove move;
  char san[CT_SAN_MAX_LENGTH];

  if (move_value == Qnil)
    return Qnil;
  graph = (CtGraph) DATA_PTR(self);
  move = ct_move_from_ruby_move(move_value);
  if (move == NULL_MOVE)
    return Qnil;
  ct_graph_move_to_san(graph, move, san);
  return rb_str_new2(san);
}

static VALUE
ct_graph_for_each_move_made_rb(VALUE self)
{
  CtGraph graph;
  CtMoveCommandStruct move_command;

  graph = (CtGraph) DATA_PTR(self);
  move_command = ct_move_command_make(0, ct_graph_replay_moves_yield_to);

  ct_graph_for_each_move_made(graph, &move_command);
  return self;
}

static void
ct_graph_replay_moves_yield_to(void *ignore, CtMove move)
{
  rb_yield(ruby_move_from_ct_move(move));
}

static VALUE
ct_graph_position_rb(VALUE self)
{
  CtPosition position;
  CtGraph graph;

  graph = (CtGraph) DATA_PTR(self);
  position = ct_position_new();
  ct_graph_to_position(graph, position);
  return ruby_position_from_ct_position(position);
}

/* CtGameTags argument is optional */
static VALUE
ct_graph_to_pgn_rb(int argc, VALUE * argv, VALUE self)
{
  CtGraph graph;
  CtGameTags game_tags = 0;
  char *pgn;
  VALUE result;

  graph = (CtGraph) DATA_PTR(self);
  if (argc > 1)
  {
    rb_raise(rb_eException, "too many arguments to pgn");
    return Qnil;
  }
  if (argc == 1)
    game_tags = ct_game_tags_from_ruby_game_tags(argv[0]);
  pgn = ct_graph_to_new_pgn(graph, game_tags);
  result = rb_str_new2(pgn);
  free(pgn);
  return result;
}

/* Expects either a string or file to read, then optionally a CtGameTags object to use.
   If reading from a file, you can also pass a block which will be yielded to after every game is loaded. */
static VALUE
ct_graph_from_pgn_rb(int argc, VALUE * argv, VALUE self)
{
  CtGraph graph;
  CtGameTags game_tags = 0;
  VALUE pgn_value;
  char *pgn = 0;
  FILE *file = 0;
  rb_io_t *fptr;
  char error_message[CT_GRAPH_FROM_PGN_ERROR_MESSAGE_MAX_LENGTH] = "from_pgn expects either a string or file to read";
  VALUE result;

  graph = (CtGraph) DATA_PTR(self);
  if (argc > 2 || argc < 1)
  {
    rb_raise(rb_eException, "from_pgn expects 1 or 2 arguments");
    return Qnil;
  }
  pgn_value = argv[0];

  if (TYPE(pgn_value) == T_FILE)
  {
    GetOpenFile(pgn_value, fptr);
    file = rb_io_stdio_file(fptr);
  }
  else
    pgn = StringValueCStr(pgn_value);
  if (argc == 2)
    game_tags = ct_game_tags_from_ruby_game_tags(argv[1]);
  if (pgn)
    ct_graph_from_pgn(graph, game_tags, pgn, error_message);
  else if (file)
  {
    CtCommandStruct command_struct = ct_command_make(0, ct_yield_to_ruby);
    ct_graph_from_pgn_file(graph, game_tags, file, &command_struct, error_message);
  }
  if (error_message[0] != 0)
  {
    rb_raise(rb_eException, "from_pgn failed: %s", error_message);
    return Qnil;
  }
  return self;
}

static void
ct_yield_to_ruby(void *ignore)
{
  if (rb_block_given_p())
    rb_yield(Qnil);
}
