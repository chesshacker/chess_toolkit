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

#ifndef CHECK_UTILITIES_H
#define CHECK_UTILITIES_H

#include <stdbool.h>

bool check_long(void);
bool check_fork_no(void);

int check_redirect_stdout(void);
void check_restore_stdout(int redirected_fd);
bool check_read_exactly(int fd, char *destination, int length);

#endif                                /* CHECK_UTILITIES_H */
