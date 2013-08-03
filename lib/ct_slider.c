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
#include "ct_slider.h"
#include "ct_utilities.h"
#include "ct_position.h"
#include "ct_piece.h"
#include "ct_move_command.h"
#include "ct_move.h"
#include "ct_rays.h"
#include <string.h>

typedef struct CtSliderStruct
{
  CtMoveCommand move_command;
  CtPieceColor piece_color;
  CtRays rays;
} CtSliderStruct;

static CtSlider ct_slider_new(CtMoveCommand move_command, CtPieceColor piece_color);

CtSlider
ct_queen_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtSlider slider = ct_slider_new(move_command, piece_color);

  slider->rays = ct_rays_new_queen();
  return slider;
}

CtSlider
ct_rook_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtSlider slider = ct_slider_new(move_command, piece_color);

  slider->rays = ct_rays_new_rook();
  return slider;
}

CtSlider
ct_bishop_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtSlider slider = ct_slider_new(move_command, piece_color);

  slider->rays = ct_rays_new_bishop();
  return slider;
}

static CtSlider
ct_slider_new(CtMoveCommand move_command, CtPieceColor piece_color)
{
  CtSlider slider;

  slider = ct_malloc(sizeof(CtSliderStruct));
  slider->move_command = move_command;
  slider->piece_color = piece_color;
  return slider;
}

void
ct_slider_free(CtSlider slider)
{
  ct_rays_free(slider->rays);
  ct_free(slider);
}

void
ct_slider_move(CtSlider slider, CtPosition position, CtSquare from)
{
  CtMoveCommand move_command = slider->move_command;
  CtSquare to;
  CtPiece to_piece;
  CtRays rays = slider->rays;
  CtDirection direction;

  ct_rays_from(rays, from);
  while (direction = ct_rays_next_direction(rays))
  {
    to = from;
    do
    {
      to += direction;
      to_piece = ct_position_get_piece(position, to);
      if (to_piece == EMPTY)
        ct_move_command_execute(move_command, ct_move_make(from, to));
      else
      {
        if (ct_piece_color(to_piece) != slider->piece_color)
          ct_move_command_execute(move_command, ct_move_make(from, to));
        break;
      }
    } while (ct_rays_can_continue_through(rays, to));
  }
}
