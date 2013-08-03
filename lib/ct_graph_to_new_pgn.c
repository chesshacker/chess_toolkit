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
#include "ct_utilities.h"
#include "ct_game_tags.h"
#include "ct_graph.h"
#include "ct_move_command.h"
#include <string.h>
#include <stdio.h>

enum
{
  MAX_LINE_LENGTH = 79,
  INITIAL_BUFFER_SIZE = 1024
};

typedef struct CtPgnWriterStruct *CtPgnWriter;
typedef struct CtPgnWriterStruct
{
  CtGameTags game_tags;
  CtGraph graph;
  char *buffer;
  char *end_of_buffer;
  char *end_of_string;
  char escaped_string[2 * GAME_TAGS_VALUE_MAX_LENGTH - 1];        /* if every character needed to be escaped with a \ */
  char *movetext_begins;
  CtMoveCommandStruct add_move_struct;
  unsigned int ply;
} CtPgnWriterStruct;

static char *mandatory_keys[] = {"Event", "Site", "Date", "Round", "White", "Black", "Result", 0};
static char *optional_keys[] = {"WhiteElo", "BlackElo", "ECO", 0};

static CtPgnWriter ct_pgn_writer_init(CtPgnWriter pgn_writer, CtGraph graph, CtGameTags game_tags);
static void ct_pgn_writer_reset(CtPgnWriter pgn_writer);
static void ct_pgn_writer_generate_tag_pair_section(CtPgnWriter pgn_writer);
static void ct_pgn_writer_generate_movetext_section(CtPgnWriter pgn_writer);

static void ct_pgn_writer_add_tag(CtPgnWriter pgn_writer, char *key, char *value);
static char *ct_pgn_writer_escape(CtPgnWriter pgn_writer, char *string);
static void ct_pgn_writer_add_move(void *delegate, CtMove move);
static void ct_pgn_writer_add(CtPgnWriter pgn_writer, char *addition);
static void ct_pgn_writer_break_movetext_into_lines(CtPgnWriter pgn_writer);

char *
ct_graph_to_new_pgn(CtGraph graph, CtGameTags game_tags)
{
  CtPgnWriterStruct pgn_writer[1];

  ct_pgn_writer_init(pgn_writer, graph, game_tags);
  if (game_tags)
  {
    ct_pgn_writer_generate_tag_pair_section(pgn_writer);
    if (graph)
      ct_pgn_writer_add(pgn_writer, "\n");
  }
  if (graph)
    ct_pgn_writer_generate_movetext_section(pgn_writer);
  return pgn_writer->buffer;
}

static CtPgnWriter
ct_pgn_writer_init(CtPgnWriter pgn_writer, CtGraph graph, CtGameTags game_tags)
{
  pgn_writer->game_tags = game_tags;
  pgn_writer->graph = graph;
  pgn_writer->buffer = ct_malloc(INITIAL_BUFFER_SIZE);
  pgn_writer->end_of_string = pgn_writer->buffer;
  pgn_writer->end_of_buffer = pgn_writer->buffer + INITIAL_BUFFER_SIZE;
  pgn_writer->add_move_struct = ct_move_command_make(pgn_writer, ct_pgn_writer_add_move);
  pgn_writer->end_of_string = pgn_writer->buffer;
  *pgn_writer->buffer = 0;
  return pgn_writer;
}

static void
ct_pgn_writer_generate_tag_pair_section(CtPgnWriter pgn_writer)
{
  char **key_ptr;
  CtGameTags game_tags = pgn_writer->game_tags;

  for (key_ptr = mandatory_keys; *key_ptr != 0; key_ptr++)
    ct_pgn_writer_add_tag(pgn_writer, *key_ptr, ct_game_tags_get(game_tags, *key_ptr));
  for (key_ptr = optional_keys; *key_ptr != 0; key_ptr++)
  {
    char *tag_value = ct_game_tags_get(game_tags, *key_ptr);

    if (strcmp(tag_value, "?") != 0)
      ct_pgn_writer_add_tag(pgn_writer, *key_ptr, tag_value);
  }
}

static void
ct_pgn_writer_generate_movetext_section(CtPgnWriter pgn_writer)
{
  char *game_result;
  CtGameTags game_tags = pgn_writer->game_tags;

  pgn_writer->movetext_begins = pgn_writer->end_of_string;
  pgn_writer->ply = 0;
  ct_graph_for_each_move_made(pgn_writer->graph, &pgn_writer->add_move_struct);
  if (pgn_writer->end_of_string > pgn_writer->movetext_begins)
    ct_pgn_writer_add(pgn_writer, " ");
  if (game_tags)
    game_result = ct_game_tags_get(game_tags, "Result");
  else
    game_result = "*";
  ct_pgn_writer_add(pgn_writer, game_result);
  ct_pgn_writer_break_movetext_into_lines(pgn_writer);
  ct_pgn_writer_add(pgn_writer, "\n");
}

static void
ct_pgn_writer_add_tag(CtPgnWriter pgn_writer, char *key, char *value)
{
  ct_pgn_writer_add(pgn_writer, "[");
  ct_pgn_writer_add(pgn_writer, key);
  ct_pgn_writer_add(pgn_writer, " \"");
  ct_pgn_writer_add(pgn_writer, ct_pgn_writer_escape(pgn_writer, value));
  ct_pgn_writer_add(pgn_writer, "\"]\n");
}

static char *
ct_pgn_writer_escape(CtPgnWriter pgn_writer, char *string)
{
  char *escaped_string = pgn_writer->escaped_string;
  char *copy_to = escaped_string;

  for (; *string != 0; string++)
  {
    if (*string == '\\' || *string == '"')
      *copy_to++ = '\\';
    *copy_to++ = *string;
  }
  *copy_to = 0;
  return escaped_string;
}

static void
ct_pgn_writer_add_move(void *delegate, CtMove move)
{
  CtPgnWriter pgn_writer = (CtPgnWriter) delegate;
  int ply = pgn_writer->ply++;
  char move_notation[CT_SAN_MAX_LENGTH];

  if (ply != 0)
    ct_pgn_writer_add(pgn_writer, " ");
  if (ply % 2 == 0)
  {
    char move_number_text[13];        /* big enough for a 10 digit number */

    snprintf(move_number_text, sizeof(move_number_text), "%d. ", ply / 2 + 1);
    ct_pgn_writer_add(pgn_writer, move_number_text);
  }
  ct_graph_move_to_san(pgn_writer->graph, move, move_notation);
  ct_pgn_writer_add(pgn_writer, move_notation);
}

static void
ct_pgn_writer_add(CtPgnWriter pgn_writer, char *addition)
{
  int length = strlen(addition);

  /* originally I wrote "if (...)" but thought what if there was a huge addition (such that twice wasn't enough)? so I
     changed to while(...), though I never expect the size to grow more than once for any single addition. */
  while (pgn_writer->end_of_string + length + 1 >= pgn_writer->end_of_buffer)
  {
    int new_size = 2 * (pgn_writer->end_of_buffer - pgn_writer->buffer);
    char *new_buffer = ct_realloc(pgn_writer->buffer, new_size);
    int shift_pointers = new_buffer - pgn_writer->buffer;

    pgn_writer->buffer = new_buffer;
    pgn_writer->end_of_buffer = new_buffer + new_size;
    pgn_writer->end_of_string += shift_pointers;
    pgn_writer->movetext_begins += shift_pointers;
  }
  strcpy(pgn_writer->end_of_string, addition);
  pgn_writer->end_of_string += length;
}

static void
ct_pgn_writer_break_movetext_into_lines(CtPgnWriter pgn_writer)
{
  char *line_end = pgn_writer->movetext_begins + MAX_LINE_LENGTH;
  char *end_of_string = pgn_writer->end_of_string;

  while (line_end < end_of_string)
  {
    while (*line_end != ' ')
      line_end--;
    *line_end = '\n';
    line_end += MAX_LINE_LENGTH + 1;
  }
}
