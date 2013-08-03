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

/* This is the only header you need to include for access to the Chess Toolkit API. */
#include "chess_toolkit.h"

#include <stdio.h>
#include <stdlib.h>

/* Notice, there are a few variable types pulled in from chess_toolkit.h here... */
typedef struct MyDataStruct
{
  int64_t hash;                        /* chess_toolkit.h includes stdint.h, which defines int64_t among others */
  CtGraph graph;                /* CtGraph is probably the most useful data type provided by the Chess Toolkit */
  CtGameTags game_tags;                /* CtGameTags stores PGN tag pairs */
  bool is_a_match;                /* chess_toolkit.h also includes stdbool.h which defines bool */
} MyDataStruct;

void filter_game(void *delegate);
void filter_position(void *delegate, CtMove move);

int
main(int argc, char **argv)
{
  char *move_text = argv[1];
  char *filename = argv[2];        /* optional argument */
  CtCommand pgn_command;        /* CtCommand, CtMoveCommand, CtPieceCommand all follow the Command design pattern. */
  MyDataStruct my_data_struct;
  CtGraph temp_graph;
  FILE *file = NULL;
  char error_message[CT_GRAPH_FROM_PGN_ERROR_MESSAGE_MAX_LENGTH];        /* space to store an error message */
  int exit_status = 0;

  if (argc < 2 || argc > 3)
  {
    printf("Usage: filter_position \"<move text>\" [filename]\n"
           "  for example: ./filter_position \"e4 c5\" < ../../tests/candidates2013.pgn\n"
           "  or           ./filter_position \"1. e4 c5\" ../../tests/candidates2013.pgn\n");
    exit(-1);
  }

  /* You must initialize the chess_toolkit before doing anything with it. */
  chess_toolkit_init();

  /* A graph lets you navigate from one position to the next, storing the chess moves as you go. */
  /* ct_graph_from_pgn will fill in a graph you provide, or give give you a shared graph (as shown here). Be careful if
     you use the shared graph; this graph is shared by any code calling the same function. So it's not a good idea for
     multi-threaded applications; but it is convenient for simple programs because it does not have to be free'd. In
     fact, free'ing it causes problems, so please don't.  Only free things created with "new" as you'll see later. */
  temp_graph = ct_graph_from_pgn(0, 0, move_text, 0);
  if (temp_graph == 0)
  {
    /* ct_graph_from_pgn will return 0 if there was an error with the PGN such as an invalid move. */
    fprintf(stderr, "Error: could not parse move_text.\n");
    exit(-1);
  }

  if (argc == 3)
  {
    file = fopen(filename, "r");
    if (file == 0)
    {
      fprintf(stderr, "Error: could not open %s\n", filename);
      exit(-1);
    }
  }
  else
    file = stdin;

  /* You could get a copy of the CtPosition object from graph, and the call ct_position_hash, but for convenience, the
     there is a ct_graph_position_hash method which returns the hash of the graph's current position. */
  my_data_struct.hash = ct_graph_position_hash(temp_graph);

  /* Earlier, you saw ct_graph_from_pgn can return a shared graph.  Next time it's called, it will be with a dedicated
     graph object, created like so: */
  my_data_struct.graph = ct_graph_new();

  /* To store the PGN game tags requires a dedicated object. */
  my_data_struct.game_tags = ct_game_tags_new();

  /* One last thing before the program is ready to start parsing the PGN file.  For each game, it will callback to a
     command object, which is simply a pointer to some data and a function name that takes a void pointer.  There are
     two ways to build a command object, here it is done with ct_command_new: */
  pgn_command = ct_command_new(&my_data_struct, filter_game);

  /* This call is similar to the ct_graph_from_pgn, but rather than taking a string, it takes a FILE pointer and a
     command to be run after each game has been read.  Note, the last parameter is an optional buffer to store an error
     message. */
  ct_graph_from_pgn_file(my_data_struct.graph, my_data_struct.game_tags, file, pgn_command, error_message);

  /* If there was an error, the error message describes it.  If there was no error, the error message is set to an
     empty string. */
  if (error_message[0] != 0)
  {
    fprintf(stderr, "Error parsing pgn: %s\n", error_message);
    exit_status = -1;
  }
  /* Since the program is exiting next, free'ing memory isn't necessary, but here is how it would work. */
  ct_command_free(pgn_command);
  ct_game_tags_free(my_data_struct.game_tags);
  ct_graph_free(my_data_struct.graph);
  if (file)
    fclose(file);
  exit(exit_status);
}

void
filter_game(void *delegate)
{
  MyDataStruct *my_data = (MyDataStruct *) delegate;
  char *pgn;

  /* Earlier ct_command_new was demonstrated.  Here is the other way to build a command, directly in a local variable.
     Another difference is that this command takes CtMove as an argument.  You'll see this is used to navigate all the
     moves in a game.  Notice, this is a "Command Struct."  A "Command" is just a pointer to a Command Struct. */
  CtMoveCommandStruct move_command_struct = ct_move_command_make(my_data, filter_position);

  my_data->is_a_match = false;

  /* This function will rewind a graph and play through each move, updating the position as it goes.  After each move,
     it calls back to the CtMoveCommand we pass it. */
  ct_graph_for_each_move_made(my_data->graph, &move_command_struct);

  /* We need to check the final position too, since the above function provides the position BEFORE each move. */
  filter_position(my_data, NULL_MOVE);

  /* If you peak ahead to filter_position, you'll see that the hash for each position was calculated and
     my_data->is_a_match was updated if the position's hash matched one we were looking for. */
  if (my_data->is_a_match)
  {
    /* For those games that had a matching position, we calculate their PGN and write it to stdout.  Notice that
       ct_graph_to_new_pgn returns a newly malloc'd character string.  It's up to the caller to free it. Anything "new"
       needs to eventually be freed or your program will leak memory. */
    pgn = ct_graph_to_new_pgn(my_data->graph, my_data->game_tags);
    printf("%s\n", pgn);
    free(pgn);
  }
}

void
filter_position(void *delegate, CtMove move)
{
  MyDataStruct *my_data = (MyDataStruct *) delegate;

  /* This is the same method used above to hash the current position. */
  int64_t current_hash = ct_graph_position_hash(my_data->graph);

  if (current_hash == my_data->hash)
    my_data->is_a_match = true;
}
