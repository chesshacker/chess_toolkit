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
#include "ct_utilities.h"
#include "ct_error.h"
#include <stdlib.h>

void *
ct_malloc(int size)
{
  void *result;

  result = malloc(size);
  if (!result)
    ct_error("malloc failed");
  return result;
}

void *
ct_realloc(void *ptr, int size)
{
  void *result;

  result = realloc(ptr, size);
  if (!result)
    ct_error("realloc failed");
  return result;
}

void
ct_free(void *ptr)
{
  free(ptr);
}
