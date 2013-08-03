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
#include "ct_steper.h"
#include "ct_utilities.h"
#include "ct_piece.h"
#include "ct_position.h"
#include "ct_move_command.h"
#include "ct_move.h"
#include "ct_rays.h"
#include <string.h>                /* memset */

typedef struct CtSteperStruct
{
  CtMoveCommand move_command;
  CtPieceColor piece_color;
  CtRays rays;
} CtSteperStruct;

static CtSteper ct_steper_new(CtMoveCommand move_command, CtPieceColor piece_color);

CtSteper
ct_king_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtSteper result = ct_steper_new(move_command, piece_color);

  result->rays = ct_rays_new_king();
  return result;
}

CtSteper
ct_knight_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtSteper result = ct_steper_new(move_command, piece_color);

  result->rays = ct_rays_new_knight();
  return result;
}

static CtSteper
ct_steper_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtSteper steper;

  steper = ct_malloc(sizeof(CtSteperStruct));
  steper->move_command = move_command;
  steper->piece_color = piece_color;
  return steper;
}

void
ct_steper_free(CtSteper steper)
{
  ct_rays_free(steper->rays);
  ct_free(steper);
}

void
ct_steper_move(CtSteper steper, CtPosition position, CtSquare from)
{
  CtRays rays = steper->rays;
  CtDirection direction;
  CtSquare to;
  CtPiece to_piece;

  ct_rays_from(rays, from);
  while (direction = ct_rays_next_direction(rays))
  {
    to = from + direction;
    to_piece = ct_position_get_piece(position, to);
    if (to_piece == EMPTY || ct_piece_color(to_piece) != steper->piece_color)
      ct_move_command_execute(steper->move_command, ct_move_make(from, to));
  }
}
