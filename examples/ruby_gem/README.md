Chess Toolkit Ruby Gem
======================

Chess Toolkit Ruby Gem is an example of how you can use the Chess Toolkit with the Ruby programming language.  It is not meant to be a full-featured version of the Chess Toolkit in Ruby.  Only parts of the Chess Toolkit were included in the gem's API, and there are several things that could be done differently to take better advantage of the Ruby language.  That said, if you do use it, you will find there's a lot you can do with it already.  Adding more features is also straightforward; the code defining Chess Toolkit classes is in ct/examples/ext/chess_toolit/, and the tests are in ct/examples/test/.  There is an example using the Chess Toolkit Ruby Gem in ct/examples/add_eco.

License
-------

Chess Toolkit: a software library for creating chess programs
Copyright (C) 2013 Steve Ortiz

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Installation
------------

Compilation of this gem requires the rake-compile gem.  So, before you run rake, if you don't already have rake-compile, get it with `gem install rake-compile`.

From the ct/examples/ruby_gem directory, run `rake` to build and test the gem.  To install the gem, run:

    rake gem
    cd pkg
    gem install chess_toolkit-0.0.0.gem

Example
-------

Now that you have installed the Chess Toolkit Gem, you can try it out.  Here is a really quick example.  Notice the first step is to require the gem:

    $ irb
    irb(main):001:0> require 'chess_toolkit'
    => true
    irb(main):002:0> graph = CtGraph.new
    =>  8  r n b q k b n r
     7  p p p p p p p p
     6  - - - - - - - -
     5  - - - - - - - -
     4  - - - - - - - -
     3  - - - - - - - -
     2  P P P P P P P P
     1  R N B Q K B N R
        a b c d e f g h
    White KQkq -
    Ply 0
    irb(main):003:0> move = graph.move_from_san("e4")
    => e2e4
    irb(main):004:0> graph.make_move(move)
    =>  8  r n b q k b n r
     7  p p p p p p p p
     6  - - - - - - - -
     5  - - - - - - - -
     4  - - - - P - - -
     3  - - - - - - - -
     2  P P P P - P P P
     1  R N B Q K B N R
        a b c d e f g h
    Black KQkq -
    Ply 1
    irb(main):005:0> graph.legal_moves
    => [a7a6, a7a5, b7b6, b7b5, c7c6, c7c5, d7d6, d7d5, e7e6, e7e5, f7f6, f7f5, g7g6, g7g5, h7h6, h7h5, b8c6, b8a6, g8h6, g8f6]

API
---

Documentation of the Ruby Gem is intentionally sparse because this gem is only meant to be an example, not a full featured Ruby Gem of the Chess Toolkit.  Only four classes were created: CtGraph, CtGameTags, CtPosition and CtMove.  These allow the user to access the higher-level functionality of the Chess Toolkit, but the lower-level data types (Square, Piece, etc.) are not included.  In most cases, the Ruby API is very similar to the ANSI-C API, so here I will only list the names of the methods and mention a few differences.  For more information, I suggest you look through the Chess Toolkit MANUAL, Ruby Gem tests, the add_eco example, or the Ruby Gem's source code.

**CtGraph** methods: new, inspect, ply, legal_moves, make_move, unmake_move, to_fen, from_fen, position_hash, move_from_san, move_to_san, for_each_move_made, position, to_pgn, from_pgn

**CtGameTags** methods: new, inspect, get, set

**CtPosition** methods: new, inspect, to_fen, from_fen, hash, is_white_to_move

**CtMove** methods: inspect, <=>

All the classes except for CtMove have a new method.  To create CtMoves, you must rely on CtGraph.  All the objects have an inspect method providing a human readable string representation of the object.

CtGraph provides a legal_moves method which returns an Array of legal moves, rather than providing a for_each_legal_move method.  Either approach would work, but using Ruby's Array object seemed more practical than yielding to a command for each legal move.  Note, there is no need for any of the Command objects since Ruby provides the ability to yield to a block of code.  There is also no need to provide buffers for writing to, since the Ruby will return objects which support automatic garbage collection.  Because of Ruby's garbage collection, CtGraph can provide a simpler to_pgn method vice ct_graph_to_new_pgn in C.  There are a couple methods that use optional argument:

    graph.to_pgn(game_tags)
> returns a ruby string with the PGN representation of the graph and game_tags.  The game_tags object is optional.  If omitted, it returns only the movetext portion of the PGN with a "*" game termination.

    graph.from_pgn(pgn_string, game_tags)
    graph.from_pgn(pgn_file˜, game_tags) { yields_to_code_block }
> tries to load the provided pgn_string and returns the graph if successful.  If unsuccessful, it returns nil and raises an exception with a message describing where in the PGN in found a syntax error.  game_tags is optional, and will be populated with information from the PGN if provided.  If using the pgn_file, it does the same thing, but it will yield to a block of code provided.

The best way to figure out the API is to simply try it out using an interactive ruby shell.
