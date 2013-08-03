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

#ifndef CT_RAYS_H
#define CT_RAYS_H

#include "ct_types.h"
#include "ct_types_internal.h"

CtRays ct_rays_new_king(void);
CtRays ct_rays_new_knight(void);
CtRays ct_rays_new_queen(void);
CtRays ct_rays_new_rook(void);
CtRays ct_rays_new_bishop(void);
void ct_rays_free(CtRays rays);

void ct_rays_from(CtRays rays, CtSquare square);
CtDirection ct_rays_next_direction(CtRays rays);
bool ct_rays_can_continue_through(CtRays rays, CtSquare square);

#endif                                /* CT_RAYS_H */
