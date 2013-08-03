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
#include <check.h>
#include "check_utilities.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

static int save_stout_fds = -1;

static bool check_environment_contains(char *environment_variable_name);
static bool check_environment_string_equals(char *environment_variable_name, char *expected_value);

bool
check_long(void)
{
  return check_environment_contains("CT_CHECK_LONG");
}

bool
check_fork_no(void)
{
  return check_environment_string_equals("CK_FORK", "no");
}

static bool
check_environment_contains(char *environment_variable_name)
{
  return getenv(environment_variable_name) != 0;
}

static bool
check_environment_string_equals(char *environment_variable_name, char *expected_value)
{
  char *env = getenv(environment_variable_name);

  return env != 0 && strcmp(env, expected_value) == 0;
}

/* returns a new file descriptor from which to read stdout (without printing to the screen / file... wherever it was outputing) */
int
check_redirect_stdout(void)
{
  enum
  {
    READ_FDS, WRITE_FDS
  };
  int fds[2];                        /* read, write pair of the pipe respectively */
  int result;

  result = pipe(fds);
  ck_assert(result == 0);
  save_stout_fds = dup(STDOUT_FILENO);
  ck_assert(save_stout_fds > 0);
  result = dup2(fds[WRITE_FDS], STDOUT_FILENO);
  ck_assert(result != -1);
  close(fds[WRITE_FDS]);
  return fds[READ_FDS];
}

void
check_restore_stdout(int redirected_fd)
{
  int result;

  ck_assert(save_stout_fds > 0);
  result = dup2(save_stout_fds, STDOUT_FILENO);
  ck_assert(result != -1);
  close(redirected_fd);
}

/* tries to read exactly length bytes from the fd to the specified destination.  Adds a null to the destination.
   returns true on success, false if there are not exactly length bytes ready to be read */
bool
check_read_exactly(int fd, char *destination, int length)
{
  int bytes_read;
  fd_set rfds;
  struct timeval tv;
  int result;

  fflush(stdout);
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  while (length > 0)
  {
    result = select(fd + 1, &rfds, 0, 0, &tv);
    if (result != 1)
      return false;
    bytes_read = read(fd, destination, length);
    destination += bytes_read;
    length -= bytes_read;
  }
  *destination = 0;
  result = select(fd + 1, &rfds, 0, 0, &tv);
  if (result != 0)
    return false;
  return length == 0;
}
