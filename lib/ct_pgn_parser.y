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

%{
  #include "ct_pgn_reader.h"
  #include <stdio.h>
%}

%define api.pure
%lex-param {void * scanner}
%parse-param {CtPgnReader pgn_reader}
%{
  #define scanner (ct_pgn_reader_scanner(pgn_reader))
%}

%locations

%union {
  char * str;
  char cval;
}

%{  /* this must appear below the %union so that YYLTYPE is defined first */
  int yylex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param, void * yyscanner);
  void yyerror (YYLTYPE * llocp, CtPgnReader pgn_reader, char const *);
%}

%token <str>  SYMBOL_TOKEN
%token <str>  STRING_TOKEN
%token <str>  MOVE_NOTATION
%token <str>  GAME_TERMINATION
%token <str>  INVALID_MOVE
%token <cval> UNEXPECTED_CHARACTER
%token END 0 "end of file"

%%

pgn:
  complete_games
| partial_game
;

complete_games:
  complete_game
| complete_games complete_game

complete_game:
  tag_pair_section movetext_section
;

partial_game:
  tag_pair_section END
| tag_pair_section movetext_section_moves END
;

tag_pair_section:
  /* empty */
| tag_pair_section_tags
;

tag_pair_section_tags:
  tag_pair
| tag_pair_section_tags tag_pair

tag_pair:
  '[' SYMBOL_TOKEN STRING_TOKEN ']'

movetext_section:
  GAME_TERMINATION
| movetext_section_moves GAME_TERMINATION
;

movetext_section_moves:
  MOVE_NOTATION
| movetext_section_moves MOVE_NOTATION
;

%%

#include <stdio.h>

void
yyerror(YYLTYPE * llocp, CtPgnReader pgn_reader, char const *s)
{
  ct_pgn_reader_syntax_error(pgn_reader, llocp->first_line, llocp->first_column);
}
