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
  #include "ct_pgn_parser.h"

  static void count(YYLTYPE * llocp, char * text);
  #define YY_USER_ACTION  count(yyget_lloc(yyscanner), yytext);
  #define YY_EXTRA_TYPE CtPgnReader
%}

%option reentrant bison-bridge bison-locations noyywrap

%x EXPECTING_SYMBOL

DIGIT   [0-9]
LETTER  [a-zA-Z0-9_]
FILE    [a-h]
RANK    [1-8]
PIECE   [KQNRBP]
PROMOTE [QNRB]
SUFFIX  [!?+#]

%%

;[^\n]*                      /* eat up rest of line comments */
\{[^}]*\}                    /* eat up inline comments */
{DIGIT}+"."                  /* eat up move numbers */
[ \t\n\r]+                   /* eat up whitespace */
"[" {
      BEGIN(EXPECTING_SYMBOL);
      return *yytext;
    }
<EXPECTING_SYMBOL>{LETTER}+ {
      CtPgnReader pgn_reader = yyget_extra(yyscanner);
      ct_pgn_reader_set_tag_key(pgn_reader, yytext);
      BEGIN(INITIAL);
      return SYMBOL_TOKEN;
    }
\"(\\.|[^\\"])*\" {
      CtPgnReader pgn_reader = yyget_extra(yyscanner);
      ct_pgn_reader_set_tag_value(pgn_reader, yytext);
      return STRING_TOKEN;
    }
{PIECE}?{FILE}?{RANK}?[-x]?{FILE}{RANK}(={PROMOTE})?{SUFFIX}* |
("O-O-O"|"O-O"|"0-0-0"|"0-0"){SUFFIX}* {
      CtPgnReader pgn_reader = yyget_extra(yyscanner);
      YYLTYPE * llocp = yyget_lloc(yyscanner);
      CtMove move = ct_pgn_reader_make_move(pgn_reader, yytext, llocp->first_line, llocp->first_column);
      return move ? MOVE_NOTATION : INVALID_MOVE;
    }
"1-0"|"0-1"|"1/2-1/2"|"*" {
      CtPgnReader pgn_reader = yyget_extra(yyscanner);
      ct_pgn_reader_set_game_termination(pgn_reader, yytext);
      return GAME_TERMINATION;
    }
.   return *yytext;

%%

static void
count(YYLTYPE * llocp, char * text)
{
  char * c;

  llocp->first_line = llocp->last_line;
  llocp->first_column = llocp->last_column;
  for (c = text; *c != '\0'; c++)
  {
    if (*c == '\n')
    {
      llocp->last_column = 1;
      llocp->last_line++;
    }
    else
      llocp->last_column++;
  }
}

void
ct_pgn_scanner_restart(yyscan_t yyscanner)
{
  struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
  BEGIN(INITIAL); /* INITIAL */
}
