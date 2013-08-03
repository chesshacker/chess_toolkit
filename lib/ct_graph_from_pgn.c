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
#include "ct_graph.h"
#include "ct_pgn_reader.h"
#include "ct_game_tags.h"
#include "ct_command.h"
#include "ct_utilities.h"
#include <string.h>
#include <stdio.h>

typedef struct CtPgnReaderStruct
{
  CtGraph graph;
  CtGameTags game_tags;
  void *pgn_scanner;
  char save_tag_key[GAME_TAGS_KEY_MAX_LENGTH];
  int error_line;                /* line 0 indicates no error, the first line of the text is line 1 */
  int error_column;
  char *error_message;
  CtCommand callback;
} CtPgnReaderStruct;

static CtGraph default_graph;
static CtGameTags default_game_tags;

static CtPgnReader ct_pgn_reader_new(CtGraph graph, CtGameTags game_tags, char *error_message);
static void ct_pgn_reader_free(CtPgnReader pgn_reader);
static CtGraph ct_graph_from_pgn_string_or_file(CtGraph graph, CtGameTags game_tags, char *pgn_string, FILE * file, CtCommand command, char *error_message);
static void ct_pgn_reader_reset(CtPgnReader pgn_reader);
static void ct_pgn_reader_update_error_message(CtPgnReader pgn_reader);

void ct_pgn_reader_syntax_error(CtPgnReader pgn_reader, int line, int column);

int yylex_init_extra(CtPgnReader pgn_reader, void *yyscanner);
int yylex_destroy(void *yyscanner);
void *yy_scan_string(const char *yy_str, void *yyscanner);
void yyset_in(FILE * in_str, void *yyscanner);
int yyparse(CtPgnReader pgn_reader);
void ct_pgn_scanner_restart(void *yyscanner);

void
ct_graph_from_pgn_init(void)
{
  default_graph = ct_graph_new();
  default_game_tags = ct_game_tags_new();
}

static CtPgnReader
ct_pgn_reader_new(CtGraph graph, CtGameTags game_tags, char *error_message)
{
  CtPgnReader pgn_reader;

  pgn_reader = ct_malloc(sizeof(CtPgnReaderStruct));
  pgn_reader->graph = graph;
  pgn_reader->game_tags = game_tags;
  pgn_reader->error_message = error_message;
  yylex_init_extra(pgn_reader, &pgn_reader->pgn_scanner);
  ct_pgn_reader_reset(pgn_reader);
  return pgn_reader;
}

static void
ct_pgn_reader_free(CtPgnReader pgn_reader)
{
  yylex_destroy(pgn_reader->pgn_scanner);
  ct_free(pgn_reader);
}

CtGraph
ct_graph_from_pgn(CtGraph graph, CtGameTags game_tags, char *pgn_string, char *error_message)
{
  if (pgn_string)
    return ct_graph_from_pgn_string_or_file(graph, game_tags, pgn_string, 0, 0, error_message);
  return 0;
}

void
ct_graph_from_pgn_file(CtGraph graph, CtGameTags game_tags, FILE * file, CtCommand command, char *error_message)
{
  if (file && command)
    ct_graph_from_pgn_string_or_file(graph, game_tags, 0, file, command, error_message);
}

static CtGraph
ct_graph_from_pgn_string_or_file(CtGraph graph, CtGameTags game_tags, char *pgn_string, FILE * file, CtCommand command, char *error_message)
{
  CtPgnReader pgn_reader;
  bool is_from_string = pgn_string != 0;

  if (graph == 0)
    graph = default_graph;
  if (game_tags == 0)
    game_tags = default_game_tags;
  pgn_reader = ct_pgn_reader_new(graph, game_tags, error_message);
  if (is_from_string)
  {
    pgn_reader->callback = 0;
    yy_scan_string(pgn_string, pgn_reader->pgn_scanner);
  }
  else
  {
    /* from file, using a command to process each game */
    pgn_reader->callback = command;
    yyset_in(file, pgn_reader->pgn_scanner);
  }
  yyparse(pgn_reader);
  ct_pgn_reader_update_error_message(pgn_reader);
  if (pgn_reader->error_line)
    graph = 0;
  ct_pgn_reader_free(pgn_reader);
  return graph;
}

static void
ct_pgn_reader_reset(CtPgnReader pgn_reader)
{
  char *error_message = pgn_reader->error_message;

  ct_graph_reset(pgn_reader->graph);
  ct_game_tags_reset(pgn_reader->game_tags);
  pgn_reader->save_tag_key[0] = 0;
  pgn_reader->error_line = 0;
  if (error_message)
    error_message[0] = 0;
  /* error_column does not need to be changed, since syntax_error changes line and column together */
  /* ct_pgn_scanner_restart would be used if the PgnReader was restarted with a different source (pgn_string or file) */
}

static void
ct_pgn_reader_update_error_message(CtPgnReader pgn_reader)
{
  if (pgn_reader->error_message == 0)
    return;
  if (pgn_reader->error_line == 0)
    pgn_reader->error_message[0] = 0;
  else
    snprintf(pgn_reader->error_message, CT_GRAPH_FROM_PGN_ERROR_MESSAGE_MAX_LENGTH,
             "syntax error on line %d column %d",
             pgn_reader->error_line, pgn_reader->error_column);
}

void *
ct_pgn_reader_scanner(CtPgnReader pgn_reader)
{
  return pgn_reader->pgn_scanner;
}

void
ct_pgn_reader_set_game_termination(CtPgnReader pgn_reader, char *result)
{
  CtCommand callback = pgn_reader->callback;

  ct_game_tags_set(pgn_reader->game_tags, "Result", result);
  if (callback)
  {
    ct_command_execute(callback);
    ct_pgn_reader_reset(pgn_reader);
  }
}

void
ct_pgn_reader_set_tag_key(CtPgnReader pgn_reader, char *key)
{
  int key_length = strlen(key);

  if (key_length < GAME_TAGS_KEY_MAX_LENGTH)
    strcpy(pgn_reader->save_tag_key, key);
}

void
ct_pgn_reader_set_tag_value(CtPgnReader pgn_reader, char *quoted_string)
{
  char unescaped_tag_value[GAME_TAGS_VALUE_MAX_LENGTH];
  char *copy_to = unescaped_tag_value;
  char *stop_at = unescaped_tag_value + GAME_TAGS_VALUE_MAX_LENGTH - 1;

  if (pgn_reader->save_tag_key[0] == 0)
    return;
  quoted_string++;                /* skip the first quotation mark */
  while (*quoted_string != '"' && copy_to < stop_at)
  {
    if (*quoted_string == '\\')
      *quoted_string++;
    *copy_to++ = *quoted_string++;
  }
  *copy_to = 0;
  ct_game_tags_set(pgn_reader->game_tags, pgn_reader->save_tag_key, unescaped_tag_value);
  pgn_reader->save_tag_key[0] = 0;
  /* no error checking required -- invalid keys or Result are ignored, long values are chopped short */
}

CtMove
ct_pgn_reader_make_move(CtPgnReader pgn_reader, char *move_notation, int line, int column)
{
  CtMove move = ct_graph_move_from_san(pgn_reader->graph, move_notation);

  if (move)
    ct_graph_make_move(pgn_reader->graph, move);
  else
    ct_pgn_reader_syntax_error(pgn_reader, line, column);
  return move;
}

void
ct_pgn_reader_syntax_error(CtPgnReader pgn_reader, int line, int column)
{
  if (pgn_reader->error_line == 0)
  {
    pgn_reader->error_line = line;
    pgn_reader->error_column = column;
  }
}
