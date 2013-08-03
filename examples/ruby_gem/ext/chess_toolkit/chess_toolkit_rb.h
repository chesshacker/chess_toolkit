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

VALUE Init_ct_position(void);
VALUE Init_ct_move(void);
VALUE Init_ct_game_tags(void);
VALUE Init_ct_graph(void);

VALUE ruby_move_from_ct_move(CtMove move);
CtMove ct_move_from_ruby_move(VALUE move_value);

VALUE ruby_position_from_ct_position(CtPosition position);

CtGameTags ct_game_tags_from_ruby_game_tags(VALUE game_tags_value);
