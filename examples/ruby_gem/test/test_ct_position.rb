# Chess Toolkit: a software library for creating chess programs
# Copyright (C) 2013 Steve Ortiz
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require 'test/unit'
require 'chess_toolkit'

class CtPositionTest < Test::Unit::TestCase
  def setup
    @position = CtPosition.new
  end
  def test_inspect
    expected_result = \
      " 8  r n b q k b n r\n"\
      " 7  p p p p p p p p\n"\
      " 6  - - - - - - - -\n"\
      " 5  - - - - - - - -\n"\
      " 4  - - - - - - - -\n"\
      " 3  - - - - - - - -\n"\
      " 2  P P P P P P P P\n"\
      " 1  R N B Q K B N R\n"\
      "    a b c d e f g h\n"\
      "White KQkq -"
    assert_equal expected_result, @position.inspect
  end
  def test_to_fen
    assert_equal "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", @position.to_fen
  end
  def test_from_fen
    expected_fen = "8/8/8/8/3KPpk1/8/8/8 b - e3"
    assert @position.from_fen(expected_fen)
    assert_equal expected_fen, @position.to_fen
  end
  def test_hash
    assert_equal 3973843602409076421, @position.hash
  end
  def test_is_white_to_move
    assert_equal true, @position.is_white_to_move
    assert @position.from_fen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -")
    assert_equal false, @position.is_white_to_move
  end
end
