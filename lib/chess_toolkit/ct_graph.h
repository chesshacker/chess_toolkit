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

#ifndef CT_GRAPH_H
#define CT_GRAPH_H

#include "ct_types.h"
#include "ct_position.h"        /* need position.h to define CT_POSITION_TO_S_MAX_LENGTH */
#include <stdio.h>                /* need to define FILE */

CtGraph ct_graph_new(void);
void ct_graph_free(CtGraph graph);

void ct_graph_reset(CtGraph graph);

int ct_graph_ply(CtGraph graph);

void ct_graph_for_each_legal_move(CtGraph graph, CtMoveCommand move_command);
void ct_graph_for_each_move_made(CtGraph graph, CtMoveCommand move_command);

void ct_graph_make_move(CtGraph graph, CtMove move);
CtMove ct_graph_unmake_move(CtGraph graph);

/* ct_graph_move_to_san is defined in ct_move_writer.c */
enum
{
  CT_SAN_MAX_LENGTH = 9
};
char *ct_graph_move_to_san(CtGraph graph, CtMove move, char *destination);

/* ct_graph_move_from_san is defined in ct_move_reader.c */
CtMove ct_graph_move_from_san(CtGraph graph, char *notation);

/* ct_graph_dfs is defined in ct_graph_dfs.c */
void ct_graph_dfs(CtGraph graph, CtCommand command, int depth);

/* the following are defined in ct_graph_position.c */
CtPosition ct_graph_to_position(CtGraph graph, CtPosition position);
CtGraph ct_graph_from_position(CtGraph graph, CtPosition position);
char *ct_graph_to_fen(CtGraph graph, char *destination);
CtGraph ct_graph_from_fen(CtGraph graph, char *fen);
int64_t ct_graph_position_hash(CtGraph graph);
enum
{
  CT_GRAPH_TO_S_MAX_LENGTH = CT_POSITION_TO_S_MAX_LENGTH + 15
};
char *ct_graph_to_s(CtGraph graph, char *destination);

/* ct_graph_to_new_pgn is defined in ct_graph_to_new_pgn.c */
char *ct_graph_to_new_pgn(CtGraph graph, CtGameTags game_tags);

/* ct_graph_from_pgn and ct_graph_from_pgn_file are defined in ct_graph_from_pgn.c */
enum
{
  CT_GRAPH_FROM_PGN_ERROR_MESSAGE_MAX_LENGTH = 60
};
CtGraph ct_graph_from_pgn(CtGraph graph, CtGameTags game_tags, char *pgn_string, char *error_message);
void ct_graph_from_pgn_file(CtGraph graph, CtGameTags game_tags, FILE * file, CtCommand command, char *error_message);

#endif                                /* CT_GRAPH_H */
