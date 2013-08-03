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
#include "ct_game_tags.h"
#include "ct_utilities.h"
#include <string.h>
#include <ctype.h>

/* For now, the implementation is very simple, it only stores 9 possible tags and doesn't
   use a hash table... it brute force compares each of the possible keys. */

enum
{
  NUMBER_OF_TAGS = 10                /* The PGN Seven Tag Roster, White and Black ELOs, and ECO */
};

static const char *valid_keys[] = {"Event", "Site", "Date", "Round", "White", "Black", "Result", "WhiteElo", "BlackElo", "ECO"};

typedef struct CtGameTagsStruct
{
  char values[NUMBER_OF_TAGS][GAME_TAGS_VALUE_MAX_LENGTH];
  char *result_destination;
} CtGameTagsStruct;

static char *ct_game_tags_get_value_ptr(CtGameTags game_tags, char *key);

CtGameTags
ct_game_tags_new(void)
{
  CtGameTags game_tags;

  game_tags = ct_malloc(sizeof(CtGameTagsStruct));
  game_tags->result_destination = ct_game_tags_get_value_ptr(game_tags, "Result");
  ct_game_tags_reset(game_tags);
  return game_tags;
}

void
ct_game_tags_free(CtGameTags game_tags)
{
  ct_free(game_tags);
}

void
ct_game_tags_reset(CtGameTags game_tags)
{
  int index;

  for (index = 0; index < NUMBER_OF_TAGS; index++)
    game_tags->values[index][0] = 0;
  strcpy(game_tags->result_destination, "*");
}

char *
ct_game_tags_get(CtGameTags game_tags, char *key)
{
  char *result = ct_game_tags_get_value_ptr(game_tags, key);

  if (!result || result[0] == 0)
    result = "?";
  return result;
}

void
ct_game_tags_set(CtGameTags game_tags, char *key, char *value)
{
  char *destination = ct_game_tags_get_value_ptr(game_tags, key);
  int length;

  if (destination == 0)
    return;
  if (value == 0)
  {
    *destination = 0;
    return;
  }
  if (destination == game_tags->result_destination)
  {
    /* only accept valid results */
    if (strcmp(value, "1-0") == 0 || strcmp(value, "0-1") == 0 || strcmp(value, "1/2-1/2") == 0 || strcmp(value, "*") == 0)
      strcpy(destination, value);
  }
  else
  {
    length = strlen(value) + 1;
    if (length <= GAME_TAGS_VALUE_MAX_LENGTH)
      strncpy(destination, value, length);
    else
    {
      strncpy(destination, value, GAME_TAGS_VALUE_MAX_LENGTH - 1);
      destination[GAME_TAGS_VALUE_MAX_LENGTH - 1] = 0;
    }
    while (*destination != 0)
    {
      /* tag values cannot contain non-printable characters (like newline, tab, delete, etc.), so replace them with
         spaces */
      if (!isprint(*destination))
        *destination = ' ';
      destination++;
    }
  }
}

static char *
ct_game_tags_get_value_ptr(CtGameTags game_tags, char *key)
{
  int index;

  for (index = 0; index < NUMBER_OF_TAGS; index++)
  {
    if (strcmp(valid_keys[index], key) == 0)
      return game_tags->values[index];
  }
  return 0;
}
