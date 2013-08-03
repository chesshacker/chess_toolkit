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
#include "ct_error.h"
#include <stdio.h>
#include <stdlib.h>

static CtErrorHandler _error_handler = 0;

void
ct_error_set_custom_handler(CtErrorHandler error_handler)
{
  _error_handler = error_handler;
}

void
ct_error(const char *str)
{
  if (_error_handler)
    _error_handler(str);
  else
  {
    printf("Chess Toolkit ERROR: %s\n", str);
    exit(-1);
  }
}
