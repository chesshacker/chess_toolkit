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

#ifndef CT_TYPES_INTERNAL_H
#define CT_TYPES_INTERNAL_H

/* These types are not part of the public API but are used internally */

typedef struct CtRaysStruct *CtRays;

typedef struct CtUndoPositionStruct *CtUndoPosition;
typedef struct CtMoveMakerStruct *CtMoveMaker;

typedef struct CtPawnStruct *CtPawn;
typedef struct CtSliderStruct *CtSlider;
typedef struct CtSteperStruct *CtSteper;
typedef struct CtMoveGeneratorStruct *CtMoveGenerator;

#endif                                /* CT_TYPES_INTERNAL_H */
