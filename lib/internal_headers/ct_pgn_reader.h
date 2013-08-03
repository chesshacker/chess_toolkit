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

#ifndef CT_PGN_READER_H
#define CT_PGN_READER_H

/* this header file is for use by the parser and scanner */

#include "ct_types.h"

typedef struct CtPgnReaderStruct *CtPgnReader;

/* used by the scanner */
void ct_pgn_reader_set_game_termination(CtPgnReader pgn_reader, char *result);
void ct_pgn_reader_set_tag_key(CtPgnReader pgn_reader, char *key);
void ct_pgn_reader_set_tag_value(CtPgnReader pgn_reader, char *quoted_string);
CtMove ct_pgn_reader_make_move(CtPgnReader pgn_reader, char *move_notation, int line, int column);

/* used by the parser */
void *ct_pgn_reader_scanner(CtPgnReader pgn_reader);
void ct_pgn_reader_syntax_error(CtPgnReader pgn_reader, int line, int column);

#endif                                /* CT_PGN_READER_H */
