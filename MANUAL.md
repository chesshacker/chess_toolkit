Chess Toolkit Manual
====================

Chess Toolkit is a software library for creating chess programs.  It is written in C with an object-oriented API, so it is straightforward to use on its own or to extend another language such as Ruby.

The Chess Toolkit is a work in progress, but it is quite useful in its current state.  It understands the rules of chess, Forsyth-Edwards Notation (FEN), Short Algebraic Notation (SAN), and the basics of Portable Game Notation (PGN).  With the current features, you can do a lot.  For example, the author has used it to write software that parses through millions of games, collecting statistics on openings and scoring each position based on the game results and player ratings.  Hopefully you will find it useful too.

This document is the Chess Toolkit's primary documentation, though many questions can also be answered by exploring the comprehensive unit test suite in the tests directory, or by reviewing the library's code in the lib directory.  This document is organized into the following sections:

* License
* Installation
* Example
* Data Types
* Memory Management
* Functions
* User Stories
* Future Work

License
-------

Chess Toolkit: a software library for creating chess programs
Copyright (C) 2013 Steve Ortiz

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Installation
------------

The Chess Toolkit uses the standard GNU autoconf tools, so you can build from source by running

    ./configure
    make
    sudo make install

Optionally, if you wish to run the unit tests and have GNU check installed, you can run

    make check

To run the long version of the unit tests, which executes more [perfts](http://chessprogramming.wikispaces.com/Perft), you can first run 

    export CT_CHECK_LONG=1
    make check

See INSTALL for all the usual autconf installation details.

Example
-------

The following example is included in examples/filter_position/filter_position.c.  It's a simple program you might use to filter a PGN file, printing out only those games that include the position reached by the movetext you pass in.  For instance, you can command `./filter_position "e4 c5" < ../../tests/candidates2013.pgn` to see those games where the Sicilian Defense was played.  This program is only meant to demonstrate the basics of the Chess Toolkit API.  If you were trying to filter millions of games, this program would eventually get the job done, but hashing every position from every game is not a computationally efficient approach.

```{.c}
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
```

Data Types
----------

chess_toolkit.h includes stdint.h and stdbool.h since it uses some of these standard data types in function declarations as you'll see in the next section.  See lib/chess_toolkit/ct_types.h for more information.

**CtSquare** is an enum, and defines A1, A2, ... H8.  It also defines SQUARE_NOT_FOUND and NUMBER_OF_SQUARES.

**CtFile** is an enum, and defines FILE_A, FILE_B, ... FILE_H.  It also defines FILE_NOT_FOUND and NUMBER_OF_FILES.  Additionally an anonymous enum defines, LAST_FILE = FILE_H.

**CtRank** is an enum, and defines RANK_1, RANK_2, ... RANK_8.  It also defines RANK_NOT_FOUND and NUMBER_OF_RANKS.  Additionally an anonymous enum defines, LAST_RANK = RANK_8.

**CtDirection** is an enum, and defines the different offsets from any square as compass directions (from white's perspective with North meaning one rank forward and East meaning one file right).  For instance A1 + D_N == A2, and B2 + D_NE == C3.  The directions are: D_NONE, D_N, D_E, D_S, D_W, D_NE, D_SE, D_SW, D_NW, D_NNE, D_ENE, D_ESE, D_SSE, D_SSW, D_WSW, D_WNW, and D_NNW.

**CtPiece** is an enum, and defines values for each piece (WHITE_KING, WHITE_QUEEN, etc.), including EMPTY.  The value for each piece was carefully chosen to help categorize the piece by color or type, useful in move generation.  See ct_types.h for more details.

**CtPieceColor** is an enum, and defines two values WHITE_PIECE and BLACK_PIECE.

**CtBitBoard** is an uint64_t with one bit for each square on the board.  The low order bit (bit 0) represents square A1, and it proceeds across each rank before moving up to the next file.  A1, A2, ... A8, B1, B2, ... H7, H8.  Several macros are defined to create bit boards: BITB_EMPTY, BITB_FULL, and BITB_ONE.

**CtBitBoardArray** is a pointer to an array of CtBitBoard values, indexed by the piece value.  For instance, bit_board_array[WHITE_KING] should return a bit board with one bit set corresponding to the white king's position on the board.  bit_board_array[EMPTY] returns a bit board marking all the empty squares.

**CtPosition** is the first of several abstract data types (ADT) to be mentioned here.  Basically, it is a pointer to a representation of a chess position.  Like most of the ADTs, it reveals little about its internal representation, and you treat it as you would an object in object oriented programming.

**CtCastleRights** is an enum, and defines 16 possible states corresponding to whether or not each side still has the right to castle kingside or queenside.  If no one can castle any more, the castling rights are CASTLE_NONE.  If both players can castle either direction, it is CASTLE_KQkq.  The other 14 possible combinations are represented by removing the corresponding letter from KQkq; for instance, if white can only castle kingside and black can only castle queenside, castle rights are CASTLE_Kq.

**CtMove** is an int16_t, however which bits mean what is not specified in its type definition.  Other than its size being specified, it is treated as an ADT.  Two special moves are defined by an anonymous enum: NULL_MOVE and AMBIGUOUS_MOVE.

**CtMoveType** is an enum, and defines several move types, useful in notating moves and making moves on the board: NORMAL_MOVE, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, EN_PASSANT_POSSIBLE, EN_PASSANT_CAPTURE, and PROMOTION.

**CtMoveStack** is an ADT used to collect moves.

**CtGraph** is an ADT used to navigate from one position to the next given a move.  Each graph starts from a given chess position and saves the moves it is provided as it transitions from one position to the next.  This is the most useful object in the Chess Toolkit.

**CtGameTags** is an ADT used to store tag pairs describing a chess game.  Currently, it is limited to the following tags: Event, Site, Date, Round, White, Black, Result, WhiteElo, BlackElo and ECO.  A future release is likely to add the FEN tag so that graphs from positions other than the standard initial position are easier to work with.

**CtCommand** is a pointer to a **CtCommandStruct**.  The reason CtCommandStruct is available, is there could be cases where you'd rather not manually manage memory for each command; then it's handy to reference the struct version.  The command object follows the command design pattern.  It is provided a delegate and a method which accepts that delegate.  Passing a command object allows a the receiver to easily callback to the delegate of your command.  There are several variants of this command object.

**CtMoveCommand** is a pointer to a **CtMoveCommandStruct**.  It follows the command design pattern, and takes an argument of CtMove.

**CtPieceCommand** is a pointer to a **CtPieceCommandStruct**.  It follows the command design pattern, and takes two arguments: CtPiece and CtSquare.

Memory Management
-----------------

Any of the Chess Toolkit types that are enums (CtSquare, CtFile, etc.) or integers (CtBitBoard and CtMove) are passed by value and do not require any special memory management.  To distinguish their creation from other types that require explicit memory management, their constructors use the word "make" instead of "new".  Here are several examples:

    CtSquare square = ct_square_make(FILE_E, RANK_2);
    CtMove move = ct_move_make(E2, E4);
    CtBitBoard bit_board = ct_bit_board_make();

Those values that are ADTs, do require some care.  They are usually created by a "new" function and must later be released by the corresponding "free" function or your program will leak memory.  For instance, a new CtPosition is created with:

    position = ct_position_new();

This position must later by released with:

    ct_position_free(position);
    
There are similar functions to free other ADTs, including CtMoveStack, CtGraph, CtGameTags, and the commands (CtCommand, CtMoveCommand and CtPieceCommand).

### Special Cases: CtBitBoardArrays

CtBitBoardArrays do not have new or free functions.  They should be defined like so:

    CtBitBoard bit_board_array[CT_BIT_BOARD_ARRAY_LENGTH];

or with your choice of malloc, calloc, etc.

    CtBitBoard *bit_board_array = malloc(CT_BIT_BOARD_ARRAY_LENGTH * sizeof(CtBitBoard));
    /* use the bit board array and then later... */
    free(bit_board_array);

### Special Cases: Commands

If you would rather not use the new and free functions for a command, you can use the make functions instead.

    CtCommandStruct command_struct = ct_command_make(delegate, function);
    CtCommand command = &command_struct; /* or you just remember to use &command_struct where you would use command */

Since a CtCommand is just a pointer to CtCommand struct, you could also write it this way:

    CtCommandStruct command[] = { ct_command_make(delegate, function) };
    
Both of the above are equivalent to:

    CtCommand command = ct_command_new(delegate, function);
    /* use the command and then later... */
    ct_command_free(command);
    
but in the former cases, you never need to free anything, which comes in handy.

### Special Cases: to destination

Several functions write to an object (string, position or graph).  Assuming the caller provides an object to write to, it writes there and returns the result.  However, as a convenience, the caller can pass in zero.  This signifies the caller wants to use a shared object that they don't have to worry about freeing later.  This is handy, but in some ways it's also a double-edged sword.  Freeing the shared object will cause mysterious problems the next time this function is called.  Using the shared object is also not appropriate for multi-threaded applications; but it does make simple programs even simpler to write.  Here is an example using a shared destination:

    printf("Your last move was %s.\n", ct_move_to_s(move, 0));
    
without the shared destination, it could be written like this:

    char move_string[CT_MOVE_TO_S_MAX_LENGTH];
    ct_move_to_s(move, move_string);
    printf("Your last move was %s.\n", move_string);

or

    char move_string[CT_MOVE_TO_S_MAX_LENGTH];
    printf("Your last move was %s.\n", ct_move_to_s(move, move_string));

All three of these will produce the same result, but the first method, using the shared destination, would not work in a multi-threaded application where other threads could also call ct_move_to_s around the same time and unexpectedly change the shared destination to something else.

### Special Cases: to_new

One function in particular is worth noting in regards to memory management, because it is unique.  ct_graph_to_new_pgn returns a newly allocated memory buffer.  It is up to the caller to later free this with free().  Most of the to_xxx functions write directly to a buffer (or a shared buffer).  However, in those cases, the maximum size of the object being written is known at compile time.  In this case, the length of the PGN written depends on how many moves the graph has, so it cannot be known until runtime.  Here is how it should be used:

    char * pgn = ct_graph_to_new_pgn(graph, game_tags);
    /* use the pgn and then later... */
    free(pgn);

Functions
---------

    void chess_toolkit_init(void);
> must be called before using anything else in the toolkit.

### Error handling functions

    void ct_error(const char *str);
> is called if something unexpected and catastrophic occurs (for instance running out of memory), but there is no reason your program can't call it as well.  The default behavior is to print a message to stdout and exit.  You can override this behavior with ct_error_set_custom_handler.

    typedef void (*CtErrorHandler) (const char *);
    void ct_error_set_custom_handler(CtErrorHandler error_handler);
> is used to setup a custom error handler.  You pass it a function that takes const char * as an argument, and this function will be called instead of the default error handling function, should an unrecoverable error occur.

### Square functions

Please note that none of these square functions are designed to handle unexpected inputs.  For instance, ct_square_make assumes that it is being passed a valid file and rank.  If you pass it something unexpected, you will not get a meaningful result.  These are low-level functions written to run as fast as possible; they are not intended to be overly robust.

    CtSquare ct_square_make(CtFile file, CtRank rank);
> returns a CtSquare by value, given a file and rank.  It expects file is between FILE_A and FILE_H.

    CtFile ct_square_file(CtSquare square);
    CtRank ct_square_rank(CtSquare square);
> returns the file or rank of the square.  It expects square is between A1 and H8.

    CtFile ct_file_from_char(char c);
> returns the file corresponding to the character passed in.  It expects c is between 'a' and 'h'.  For instance, ct_file_from_char('a') will return FILE_A.

    CtRank ct_rank_from_char(char c);
> returns the rank corresponding to the character passed in.  It expects c is between '1' and '8'.  For instance, ct_rank_from_char('1') will return RANK_1.

    char ct_file_to_char(CtFile file);
    char ct_rank_to_char(CtRank rank);
> returns the character corresponding to the file or rank.  It expects file to be between FILE_A and FILE_H; rank between RANK_1 and RANK_8.

### Piece functions

    CtPieceColor ct_piece_color(CtPiece piece);
> returns the color of a piece.  Expect a valid piece (excluding EMPTY).

    CtPiece ct_piece_from_char(char piece_char);
> returns a piece corresponding to the character.  If piece_char is either a B, K, N, P, Q, or R, it will return a Bishop, King, Knight, Pawn, Queen or Rook; White for uppercase and Black for lowercase.  For instance, given 'n' returns a BLACK_KNIGHT.  Any other character not corresponding to a chess piece returns EMPTY.

    char ct_piece_to_char(CtPiece piece);
> returns a character corresponding to the piece.  An invalid piece returns 0.  Otherwise, it returns an uppercase letter for white pieces and a lowercase letter for black pieces... B, K, N, P, Q, or R, for a Bishop, King, Knight, Pawn, Queen or Rook, respectively.

    CtPiece ct_piece_to_white(CtPiece piece);
    CtPiece ct_piece_to_black(CtPiece piece);
> returns a white or black version of the piece respectively.  EMPTY remains EMPTY.

    bool ct_piece_is_pawn(CtPiece piece);
> returns true if the piece is a pawn; false otherwise.  Similar functions could test for other piece types, but it seems this was the only one needed thus far.

### Bit Board functions

    CtBitBoard ct_bit_board_make(CtSquare square);
> returns a CtBitBoard with a single bit set corresponding to the square.  It expects square is between A1 and H8.

    CtSquare ct_bit_board_find_first_square(CtBitBoard bit_board);
> finds the first bit set in bit_board and returns the index of that bit which corresponds to a square.  If no bits are set, it returns SQUARE_NOT_FOUND.

    char *ct_bit_board_to_s(CtBitBoard bit_board, char *destination);
> stores a string representation of the bit board in destination and returns the start of that string.  The string will be at most CT_BIT_BOARD_TO_S_MAX_LENGTH characters long, including the null terminator.  If no destination is provided, a shared destination will be used.

    void ct_bit_board_array_reset(CtBitBoardArray bit_board_array);
> zeros all the bit boards except for EMPTY which it fills to represent a completely empty board.

    bool ct_bit_board_array_is_white_attacking(CtBitBoardArray bit_board_array, CtSquare square);
    bool ct_bit_board_array_is_black_attacking(CtBitBoardArray bit_board_array, CtSquare square);
> examines the bit_board_array to determine if white (or black respectively) is attacking the square.  Using the bit board array in combination with some predetermined masks is an extremely fast way to see if the king is in check, and therefore if a position is legal.

### Position functions

    CtPosition ct_position_new();
> returns a new position starting from the initial board position.

    void ct_position_free(CtPosition position);
> frees the position.

    void ct_position_clear(CtPosition position);
> removes all the pieces from the position, sets white to move, no castling rights and no en-passant possible.

    void ct_position_reset(CtPosition position);
> restores the standard starting position with white to move, everyone can castle either direction and no en-passant is possible.

    void ct_position_copy(CtPosition destination, CtPosition source);
> copies the source position to destination.

    CtPiece ct_position_get_piece(CtPosition position, CtSquare square);
> returns the piece on the square.  If no piece is on the square, it returns EMPTY.

    void ct_position_set_piece(CtPosition position, CtSquare square, CtPiece piece);
> sets the piece on the square.  If the piece is EMPTY, it removes the piece.

    void ct_position_for_each_piece(CtPosition position, CtPieceCommand piece_command);
> iterates through each piece in the position that's not EMPTY and calls back to the piece command.

    void ct_position_for_each_active_piece(CtPosition position, CtPieceCommand piece_command);
> iterates similar to ct_position_for_each_piece, but it only calls back for the color pieces whose turn it is to move.

    CtBitBoard ct_position_get_bit_board(CtPosition position, CtPiece piece);
> returns a copy of the bit board.  It expects piece to be any valid piece, including EMPTY.

    bool ct_position_is_white_to_move(CtPosition position);
> returns true if it is white's turn to move in the position; false otherwise.

    void ct_position_set_white_to_move(CtPosition position);
    void ct_position_set_black_to_move(CtPosition position);
    void ct_position_change_turns(CtPosition position);
> sets whose turn it is to move in the position.

    CtFile ct_position_get_en_passant(CtPosition position);
> returns NO_EN_PASSANT if en-passant is not possible or the file of the pawn which can be captured by en-passant

    void ct_position_set_en_passant(CtPosition position, CtFile file);
> sets en-passant of the position to the file.  It expects file to be between FILE_A and FILE_H, but passing a square between A1 and H8 works also, and is equivalent to sending that square's file.  Passing NO_EN_PASSANT for the file will not work; to clear the en-passant state, use ct_position_clear_en_passant.

    void ct_position_clear_en_passant(CtPosition position);
> clears en-passant of the position, so en-passant is not possible.

    CtCastleRights ct_position_get_castle(CtPosition position);
> returns the castling rights of the position.

    void ct_position_set_castle(CtPosition position, CtCastleRights castle_rights);
> sets the castling rights of the position.

    bool ct_position_is_legal(CtPosition position);
> returns true if the the player who does not have the move is not in check since you must not leave yourself in check.

    bool ct_position_is_check(CtPosition position);
    CtCastleRights ct_position_can_castle(CtPosition position);
> returns true if the player who does have the move is in check.

    char *ct_position_to_s(CtPosition position, char *destination);
> stores a string representation of the position in destination and returns the start of that string.  The string will be at most CT_POSITION_TO_S_MAX_LENGTH characters long, including the null terminator.  If no destination is provided, a shared destination will be used.

    int64_t ct_position_hash(CtPosition position);
> calculates and returns a Zobrist hash of the position.

    char *ct_position_to_fen(CtPosition position, char *destination);
> stores a four part FEN representation of the position in destination and returns the start of that string.  The string will be at most CT_FEN_MAX_LENGTH characters long, including the null terminator.  If no destination is provided, a shared destination will be used.

    CtPosition ct_position_from_fen(CtPosition position, char *fen);
> reads the first four parts of the FEN notation and updates the position to match.  If there is a problem with the FEN, the position is cleared and 0 is returned.

### Move functions

    CtMove ct_move_make(CtSquare from, CtSquare to);
> returns a move of type NORMAL_MOVE by value.  There are other move_make functions to return the special move types (CASTLE_KINGSIDE, CASTLE_QUEENSIDE, EN_PASSANT_POSSIBLE, EN_PASSANT_CAPTURE, and PROMOTION).

    CtMove ct_move_make_castle_kingside(CtSquare from);
    CtMove ct_move_make_castle_queenside(CtSquare from);
> returns a move of type CASTLE_KINGSIDE (or CASTLE_QUEENSIDE respectively) by value.  Only the position of the king needs to be passed in.

    CtMove ct_move_make_en_passant_possible(CtSquare from, CtSquare to);
> returns a move of type EN_PASSANT_POSSIBLE by value.

    CtMove ct_move_make_en_passant_capture(CtSquare from, CtSquare to);
> returns a move of type EN_PASSANT_CAPTURE by value.

    CtMove ct_move_make_promotion_Q(CtSquare from, CtSquare to);
    CtMove ct_move_make_promotion_R(CtSquare from, CtSquare to);
    CtMove ct_move_make_promotion_B(CtSquare from, CtSquare to);
    CtMove ct_move_make_promotion_N(CtSquare from, CtSquare to);
    CtMove ct_move_make_promotion_q(CtSquare from, CtSquare to);
    CtMove ct_move_make_promotion_r(CtSquare from, CtSquare to);
    CtMove ct_move_make_promotion_b(CtSquare from, CtSquare to);
    CtMove ct_move_make_promotion_n(CtSquare from, CtSquare to);
> returns a move of type PROMOTION by value.  The piece it promotes to corresponds to the method used.

    CtMoveType ct_move_type(CtMove move);
> returns type of move (NORMAL_MOVE, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, EN_PASSANT_POSSIBLE, EN_PASSANT_CAPTURE, and PROMOTION).  It expects a legitimate move, not NULL_MOVE or AMBIGUOUS_MOVE.

    CtSquare ct_move_from(CtMove move);
    CtSquare ct_move_to(CtMove move);
> returns square the move is from (or to respectively).  It expects a legitimate move, not NULL_MOVE or AMBIGUOUS_MOVE.

    CtPiece ct_move_promotes_to(CtMove move);
> returns the piece that a move of type PROMOTION should promote into.  Expects a move of type PROMOTION, but will return EMPTY for other legitimate moves.

    char *ct_move_to_s(CtMove move, char *destination);
> stores a string representation of the move in destination and returns the start of that string.  The string will be at most CT_MOVE_TO_S_MAX_LENGTH characters long, including the null terminator.  If no destination is provided, a shared destination will be used.
    
### Move Stack functions

    CtMoveStack ct_move_stack_new();
> returns a new move stack.

    void ct_move_stack_free(CtMoveStack move_stack);
> frees the move stack.

    void ct_move_stack_push(CtMoveStack move_stack, CtMove move);
> adds move to the top of the move stack.

    CtMove ct_move_stack_pop(CtMoveStack move_stack);
> takes and returns one move from the top of the stack

    void ct_move_stack_reset(CtMoveStack move_stack);
> empties the stack

    bool ct_move_stack_is_empty(CtMoveStack move_stack);
> returns true if the stack is empty; otherwise returns false.

    unsigned int ct_move_stack_length(CtMoveStack move_stack);
> returns the number of moves on the stack.

    CtMoveCommand ct_move_stack_push_command(CtMoveStack move_stack);
> returns a move command which can be used to push moves on the stack.

    void ct_move_stack_for_each(CtMoveStack move_stack, CtMoveCommand move_command);
> executes the move_command on each move in the stack, starting with the first one added (bottom of the stack) and working up to the top.

### Graph functions

    CtGraph ct_graph_new(void);
> returns a new graph starting from the standard chess starting position.

    void ct_graph_free(CtGraph graph);
> frees the graph.

    void ct_graph_reset(CtGraph graph);
> removes all moves made and returns to the standard chess starting position.

    int ct_graph_ply(CtGraph graph);
> returns the number of moves between the starting position of this graph and its current position.

    void ct_graph_for_each_legal_move(CtGraph graph, CtMoveCommand move_command);
> generates every possible move from the current position and for each legal move it executes the move_command

    void ct_graph_for_each_move_made(CtGraph graph, CtMoveCommand move_command);
> rewinds the graph to its starting position and executes move_command on each each move that was made, making the move before executing the next move_command so the graph appears to be in the state it was when the move was originally made.  The graph should not be modified during execution of the move_command.

    void ct_graph_make_move(CtGraph graph, CtMove move);
> causes graph to save the move and update its position.

    CtMove ct_graph_unmake_move(CtGraph graph);
> causes graph to undo the last move made, reverting to the previous state.  It returns the last move, which was just undone.

    char *ct_graph_move_to_san(CtGraph graph, CtMove move, char *destination);
> stores a string representation of the move (using short algebraic notation) in destination and returns the start of that string.  The string will be at most CT_SAN_MAX_LENGTH characters long, including the null terminator.  If no destination is provided, a shared destination will be used.

    CtMove ct_graph_move_from_san(CtGraph graph, char *notation);
> compares legal moves to the notation to determine if the notation uniquely identifies a legal move, and returns the resulting move.  If no move matches, NULL_MOVE is returned.  If more than one legal move matches, AMBIGUOUS_MOVE is returned.

    void ct_graph_dfs(CtGraph graph, CtCommand command, int depth);
>  perform a depth first search from the graph's current position, executing the given command at every position it reaches that is depth moves away.  It expects depth is at least 1, and the command's execution does not modify graph.

    CtPosition ct_graph_to_position(CtGraph graph, CtPosition position);
> copies the graph's current position to the specified position and returns that position.  If no position is provided, a shared position will be used and returned.

    CtGraph ct_graph_from_position(CtGraph graph, CtPosition position);
> resets the graph, copies the specified position to the graph's starting position, and returns the graph.  If no graph is provided, a shared graph wil be used and returned.

    char *ct_graph_to_fen(CtGraph graph, char *destination);
> stores a four part FEN representation of the graph's current position in destination and returns the start of that string.  The string will be at most CT_FEN_MAX_LENGTH characters long, including the null terminator.  If no destination is provided, a shared destination will be used.

    CtGraph ct_graph_from_fen(CtGraph graph, char *fen);
> reads the first four parts of the FEN notation, resets the graph and sets the graph's starting position to match the fen.  If there is a problem with the FEN, the graph is reset, the position is cleared and 0 is returned.  If no destination is provided, a shared destination will be used.

    int64_t ct_graph_position_hash(CtGraph graph);
> calculates and returns a Zobrist hash of the graph's current position.

    char *ct_graph_to_s(CtGraph graph, char *destination);
> stores a string representation of the graph in destination and returns the start of that string.  The string will be at most CT_GRAPH_TO_S_MAX_LENGTH characters long, including the null terminator.  If no destination is provided, a shared destination will be used.

    char *ct_graph_to_new_pgn(CtGraph graph, CtGameTags game_tags);
> returns a newly allocated buffer of memory with the PGN representation of the graph and game_tags.  If no game_tags object is provided, it returns only the movetext portion of the PGN with a "*" game termination.  If no graph is provided, it returns only the game tags portion of the PGN.

    CtGraph ct_graph_from_pgn(CtGraph graph, CtGameTags game_tags, char *pgn_string, char *error_message);
> reads a PGN string, updating graph and game_tags to match.  If successful, it sets the error_message to an empty string and returns the resulting graph.  If unsuccessful, it stores an error message indicating where the error occurred to the character buffer provided, and returns 0.  The error_message buffer should be at least CT_GRAPH_FROM_PGN_ERROR_MESSAGE_MAX_LENGTH characters long.  If no error_message buffer is provided, it still returns 0, but the caller will not have any more information on the error.  If no graph is provided, a shared graph will be used.  If no game_tags are provided, game tag information will not be available.  It expects a valid PGN string for a single game.

    void ct_graph_from_pgn_file(CtGraph graph, CtGameTags game_tags, FILE * file, CtCommand command, char *error_message);
> reads a file of PGN games, updating graph and game_tags as each game is read, and executing command when the game is successfully loaded.  If there is an error reading the PGN, it does not execute command for that game.  When the function returns, error_message will be empty to indicate no errors, or it will fill error_message with a description of where the error occurred.  The error_message buffer should be at least CT_GRAPH_FROM_PGN_ERROR_MESSAGE_MAX_LENGTH characters long.  Providing an error_message buffer is optional, but there is no indication of errors without one.  If no graph is provided, a shared graph will be used.  If no game_tags are provided, game tag information will not be available.  It expects a FILE stream to be provided containing PGN for one or more games.

### Game Tag functions

    CtGameTags ct_game_tags_new(void);
> returns a new game tags object.  The Result tag is "*" and everything else is unknown.

    void ct_game_tags_free(CtGameTags game_tags);
> frees the game tags object.

    void ct_game_tags_reset(CtGameTags game_tags);
> restores the game tags object like new, with Result = "*" and everything else unknown.

    char *ct_game_tags_get(CtGameTags game_tags, char *key);
> looks up the key for the specified value.  If the key is found, it returns its associated value; otherwise it returns a pointer to "?" which indicates the value is unknown and can be used in accordance with the PGN specification for any tag value (except for Result).  Result will always have a value assigned to it; "*" indicates in progress, abandoned or unknown.

    void ct_game_tags_set(CtGameTags game_tags, char *key, char *value);
> looks up the key, and if it is one of the valid game tag keys, it sets its value.  The maximum length for a game tag value is GAME_TAGS_VALUE_MAX_LENGTH (including the null terminator), and any string longer than that is truncated to that length.  Non-printable characters are also converted to spaces, to be in compliance with the PGN specification.

### Command, Move Command, and Piece Command functions

    CtCommand ct_command_new(void *delegate, CtCommandMethod method);
    CtMoveCommand ct_move_command_new(void *delegate, CtMoveCommandMethod method);
    CtPieceCommand ct_piece_command_new(void *delegate, CtPieceCommandMethod method);
> returns a new command object.

    void ct_command_free(CtCommand command);
    void ct_move_command_free(CtMoveCommand command);
    void ct_piece_command_free(CtPieceCommand command);
> frees the command object.

    CtCommandStruct ct_command_make(void *delegate, CtCommandMethod method);
    CtMoveCommandStruct ct_move_command_make(void *delegate, CtMoveCommandMethod method);
    CtPieceCommandStruct ct_piece_command_make(void *delegate, CtPieceCommandMethod method);
> returns a command struct by value.

    void ct_command_execute(CtCommand command);
    void ct_move_command_execute(CtMoveCommand command, CtMove move);
    void ct_piece_command_execute(CtPieceCommand command, CtPiece piece, CtSquare square);
> executes the command.  Notice the different types of commands are distinguished by what the pass along with their delegate when they are executed.

User Stories
------------

These are some of the key user stories that were written to guide the development of the Chess Toolkit.  With each user story is a small sample of code showing how the user story could be accomplished.  The user stories are framed, "As a developer, I want to... so that..."

1. create a chess position from FEN so I can interpret positions.

        result = ct_position_from_fen(position, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");

2. generate legal moves from a chess position so I can observe the rules of the game.

        ct_graph_for_each_legal_move(graph, move_command);

3.  update a chess position given a legal move so I can play a game.

        ct_graph_make_move(graph, move);

4.  keep track of a chess game as moves are made so I can undo moves or replay the game.

        move = ct_graph_unmake_move(graph);
        ct_graph_for_each_move_made(graph, move_command);

5.  search through legal moves from a position so I can explore the game space.

        ct_graph_dfs(graph, command, depth);

6.  convert a chess position to FEN so I can express it clearly.

        fen_string = ct_position_to_fen(position, fen_string);

7.  convert a chess move given a position to algebraic notation so I can express it clearly.

        notation = ct_move_to_s(move, notation);
        notation = ct_graph_move_to_san(graph, move, notation);

8.  create a chess move from its algebraic notation given a position so I can interpret moves.

        move = ct_graph_move_from_san(graph, "e4");

9.  manage additional information about the game and players from a PGN so I can access it.

        ct_game_tags_set(game_tags, "White", "Fischer, Robert James");
        white_string = ct_game_tags_get(game_tags, "White");

10. convert a chess game to PGN so I can express it clearly.

        ct_graph_to_new_pgn(graph, game_tags);

11. create a chess game from PGN so I can replay the game.

        result = ct_graph_from_pgn(graph, game_tags, pgn_string, error_message);
        ct_graph_from_pgn_file(graph, game_tags, file, command, error_message);

12. convert a chess position to a Zobrist hash so I can quickly find positions in a database.

        hash = ct_position_hash(position);

Future Work
-----------

The next major feature I plan to work on is an API to support the Universal Chess Interface (UCI) protocol.  This protocol allows chess programs to communicate with a chess interface.  This would make the Chess Toolkit very useful in writing chess GUIs to many of the most popular chess engines.

A relatively simple and useful enhancement would be to add the FEN tag to the PGN related functions, so that the CtGraph object works better with PGN.  Right now, a PGN is assumed to start at the initial board position, but a CtGraph can start at any position.  This means that currently ct_graph_to_new_pgn and ct_graph_from_pgn will not work as intended for graphs / PGNs that start at positions other than the standard board setup.  This limitation / defect shouldn't be too difficult to correct.

Several minor changes and enhancements are being considered.  Under consideration are the following:

* changing the command's execute method to return a boolean which could indicate whether or not to continue iterating.
* adding methods to position to copy the bit board array and/or piece array to a specified destination.
* adding the ability for a game tag to store and return arbitrary keys/value pairs (rather than its current implementation which is limited to the seven tag roster and player ELOs).
* adding copy functions to all the objects (not just position).
* adding a function for graph to dump all its moves to a newly allocated CtMove array, and a complimentary function to load from a move array.
* lessening the restriction on modifying a graph during ct_graph_for_each_move_made and ct_graph_for_each_legal_move; however, the graph would need to protect itself to some extent against certain changes.
* creating a mutable CtStringBuffer class and passing it to ct_graph_to_pgn, rather than requiring a new character buffer be created every time a PGN is written.

Another interface change for consideration is the use of the command design pattern.  For instance, rather than passing a command to ct_graph_from_pgn_file, there could be a class that reads pgn files, and has methods like next_game and/or has_more_games.  This class could also store the error messages and scanner.  The original design had readers and writers for everything, which made for a good design in many regards, but an awkward API.  Most of these readers and writers are still in use (behind the scenes) because they help to keep the design clean.  Whether or not to expose and enhance some of the more useful readers such as CtPgnReader is something I will continue to question.

Adding some more scaffolding to turn the chess toolkit into a chess engine, capable of playing a game is something to consider.  It was never part of the plan, since there are already many great chess engines available.  But it would be fun, and educational; plus I could use it to test both sides of the UCI protocol feature.

A more complicated enhancement would be to enhance the PGN reader so it understands and does something useful with comments and/or recursive annotation.  This would allow the toolkit to support game annotation.  This could also be useful, but it's low on the priority list.

Another possible extension is to make other (smaller) board size options.  For instance, QuickChess is 5 files x 6 ranks.  In most places where files or ranks are iterated, I referenced LAST_FILE and LAST_RANK, with this enhancement in mind.  Along those lines, adding supporting Chess960 would also be interesting.  These are entertaining ideas, and perhaps not too difficult, but they are also low on the priority list.
