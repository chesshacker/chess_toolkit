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

#ifndef CT_GAME_TAGS_H
#define CT_GAME_TAGS_H

#include "ct_types.h"

enum
{
  GAME_TAGS_KEY_MAX_LENGTH = 9,
  GAME_TAGS_VALUE_MAX_LENGTH = 256        /* includes null termination */
};

CtGameTags ct_game_tags_new(void);
void ct_game_tags_free(CtGameTags game_tags);

void ct_game_tags_reset(CtGameTags game_tags);

char *ct_game_tags_get(CtGameTags game_tags, char *key);
void ct_game_tags_set(CtGameTags game_tags, char *key, char *value);

#endif                                /* CT_GAME_TAGS_H */
