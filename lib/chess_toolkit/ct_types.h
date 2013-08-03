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

#ifndef CT_TYPES_H
#define CT_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* Square related */

typedef enum CtSquare
{
  SQUARE_NOT_FOUND = -1,
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  NUMBER_OF_SQUARES
} CtSquare;

typedef enum CtFile
{
  FILE_NOT_FOUND = -1, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, NUMBER_OF_FILES
} CtFile;

typedef enum CtRank
{
  RANK_NOT_FOUND = -1, RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, NUMBER_OF_RANKS
} CtRank;

enum
{
  LAST_FILE = FILE_H
};

enum
{
  LAST_RANK = RANK_8
};

typedef enum CtDirection
{
  D_SSW = -17, D_SSE = -15, D_WSW = -10, D_SW = -9, D_S = -8, D_SE = -7, D_ESE = -6, D_W = -1, D_NONE = 0,
  D_E = 1, D_WNW = 6, D_NW = 7, D_N = 8, D_NE = 9, D_ENE = 10, D_NNW = 15, D_NNE = 17
} CtDirection;

/* Piece related */

typedef enum CtPiece
{
  EMPTY = 0,
  WHITE_PAWN = 1, WHITE_KING, WHITE_KNIGHT, WHITE_QUEEN, WHITE_ROOK, WHITE_BISHOP,
  BLACK_PAWN = 9, BLACK_KING, BLACK_KNIGHT, BLACK_QUEEN, BLACK_ROOK, BLACK_BISHOP,
} CtPiece;

typedef enum CtPieceColor
{
  WHITE_PIECE = 0,
  BLACK_PIECE = 8
} CtPieceColor;

 /* CtPiece = 14 is really the last piece value used, but 7, 8 and 15 are reserved for future use */
 /* 7 and 15 might be used in the future by bit_board for generic white and black pieces respectively */
enum
{
  PIECE_MAX_VALUE = 15,
  PIECE_MASK_VALID_BITS = 0xF
};

/* Piece values were chosen to simplify the categorization into steper, slider or pawn. */
enum
{
  STEPER_MASK = 1,
  STEPER_BIT = 2,
  SLIDER_MASK = 3,
  SLIDER_BIT = 4,
  COLOR_BIT = 8
};

/* Bit Board related */

enum
{
  CT_BIT_BOARD_ARRAY_LENGTH = PIECE_MAX_VALUE + 1
};

typedef uint64_t CtBitBoard;

/* CtBitBoardArray has a BitBoard for every piece (including Empty) and is indexed by the piece value */
typedef CtBitBoard *CtBitBoardArray;

/* Position related */

typedef struct CtPositionStruct *CtPosition;

typedef enum CtCastleRights
{
  CASTLE_NONE, CASTLE_K, CASTLE_Q, CASTLE_KQ, CASTLE_k, CASTLE_Kk, CASTLE_Qk, CASTLE_KQk,
  CASTLE_q, CASTLE_Kq, CASTLE_Qq, CASTLE_KQq, CASTLE_kq, CASTLE_Kkq, CASTLE_Qkq, CASTLE_KQkq,
} CtCastleRights;

enum
{
  NO_EN_PASSANT = FILE_NOT_FOUND
};

/* Move related */

typedef int16_t CtMove;

typedef enum CtMoveType
{
  NORMAL_MOVE, CASTLE_KINGSIDE, CASTLE_QUEENSIDE, EN_PASSANT_POSSIBLE, EN_PASSANT_CAPTURE, PROMOTION
} CtMoveType;

enum
{
  AMBIGUOUS_MOVE = -1,
  NULL_MOVE = 0
};

/* Move Stack, Graph, Game Tags and PGN Reader are all straightforward... abstract data types */

typedef struct CtMoveStackStruct *CtMoveStack;
typedef struct CtGraphStruct *CtGraph;
typedef struct CtGameTagsStruct *CtGameTags;

/* Commands are simply a delegate and a method.  The structure is exposed so they can don't have to be allocated like
   an abstract data type.  There are commands that take no arguments and others that take an argument or two. */

typedef void (*CtCommandMethod) (void *delegate);
typedef struct CtCommandStruct *CtCommand;
typedef struct CtCommandStruct
{
  void *delegate;
  CtCommandMethod method;
} CtCommandStruct;

typedef void (*CtMoveCommandMethod) (void *delegate, CtMove move);
typedef struct CtMoveCommandStruct *CtMoveCommand;
typedef struct CtMoveCommandStruct
{
  void *delegate;
  CtMoveCommandMethod method;
} CtMoveCommandStruct;

typedef void (*CtPieceCommandMethod) (void *delegate, CtPiece piece, CtSquare square);
typedef struct CtPieceCommandStruct *CtPieceCommand;
typedef struct CtPieceCommandStruct
{
  void *delegate;
  CtPieceCommandMethod method;
} CtPieceCommandStruct;

#endif                                /* CT_TYPES_H */
