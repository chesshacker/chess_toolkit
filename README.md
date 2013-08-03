Chess Toolkit
=============

Chess Toolkit is a software library for creating chess programs.  It is written in C with an object-oriented API, so it is straightforward to use on its own or to extend another language such as Ruby.

The Chess Toolkit is a work in progress, but it is quite useful in its current state.  It understands the rules of chess, Forsyth-Edwards Notation (FEN), Short Algebraic Notation (SAN), and the basics of Portable Game Notation (PGN).  With the current features, you can do a lot.  For example, the author has used it to write software that parses through millions of games, collecting statistics on openings and scoring each position based on the game results and player ratings.  Hopefully you will find it useful too.

License
-------

Chess Toolkit: a software library for creating chess programs
Copyright (C) 2013 Steve Ortiz

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Files
-----

* COPYING contains the Apache License, Version 2.0 under which this software and its manual are distributed.

* MANUAL.md is the Chess Toolkit's primary documentation.  It is written in Github Flavored Markdown (GFM), and also available as MANUAL.html.

* INSTALL contains the usual autconf installation details.

* AUTHORS and THANKS list those who worked on this software or are otherwise appreciated.

* NEWS records a list of changes visible to the user.

* lib directory contains all the Chess Toolkit's library code.

* tests directory contain comprehensive unit tests for the Chess Toolkit.

* examples/find_position is the example shown in the MANUAL to illustrate the basic functionality of the Chess Toolkit.

* examples/ruby_gem is a Ruby Gem which provides an interface to the high level parts of the Chess Toolkit.

* examples/add_eco is an example program using the aforementioned Ruby Gem.
