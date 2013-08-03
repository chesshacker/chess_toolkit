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

class CtGraphTest < Test::Unit::TestCase
  @@morphy_sans = %w(e4 c5 d4 cxd4 Nf3 Nc6 Nxd4 e6 Nb5 d6 Bf4 e5 Be3 f5 N1c3 f4 Nd5 fxe3 Nbc7+
    Kf7 Qf3+ Nf6 Bc4 Nd4 Nxf6+ d5 Bxd5+ Kg6 Qh5+ Kxf6 fxe3 Nxc2+ Ke2)
  @@morphy_tags = {
    "Event" => "Morphy-Anderssen", 
    "Site" => "Paris FRA", 
    "Date" => "1858.12.27", 
    "Round" => "9", 
    "White" => "Morphy, Paul", 
    "Black" => "Anderssen, Adolf", 
    "Result" => "1-0"
  }
  @@morphy_pgn = 
    "[Event \"Morphy-Anderssen\"]\n" +
    "[Site \"Paris FRA\"]\n" +
    "[Date \"1858.12.27\"]\n" +
    "[Round \"9\"]\n" +
    "[White \"Morphy, Paul\"]\n" +
    "[Black \"Anderssen, Adolf\"]\n" +
    "[Result \"1-0\"]\n" +
    "\n" +
    "1. e4 c5 2. d4 cxd4 3. Nf3 Nc6 4. Nxd4 e6 5. Nb5 d6 6. Bf4 e5 7. Be3 f5 8. N1c3\n" +
    "f4 9. Nd5 fxe3 10. Nbc7+ Kf7 11. Qf3+ Nf6 12. Bc4 Nd4 13. Nxf6+ d5 14. Bxd5+\n" +
    "Kg6 15. Qh5+ Kxf6 16. fxe3 Nxc2+ 17. Ke2 1-0\n"
  def setup
    @graph = CtGraph.new
    @game_tags = CtGameTags.new
  end
  def load_sans(sans)
    sans.each do |san|
      move = @graph.move_from_san(san)
      @graph.make_move(move)
    end
  end
  def sans_from_graph
    sans = []
    @graph.for_each_move_made do |move|
      sans << @graph.move_to_san(move)
    end
    return sans
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
      "White KQkq -\n"\
      "Ply 0"
    assert_equal expected_result, @graph.inspect
  end
  def test_ply
    assert_equal 0, @graph.ply
  end
  def test_to_fen
    assert_equal "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", @graph.to_fen
  end
  def test_from_fen
    expected_fen = "8/8/8/8/3KPpk1/8/8/8 b - e3"
    assert @graph.from_fen(expected_fen)
    assert_equal expected_fen, @graph.to_fen
  end
  def test_position_hash
    assert_equal 3973843602409076421, @graph.position_hash
  end
  def test_legal_moves
    moves = @graph.legal_moves
    assert_equal 20, moves.length
  end
  def test_make_and_unmake_move
    move = @graph.legal_moves.first
    @graph.make_move(move)
    assert_equal 1, @graph.ply
    unmade_move = @graph.unmake_move
    assert_equal 0, @graph.ply
    assert_equal move, unmade_move
  end
  def test_move_from_san
    move = @graph.move_from_san("e4")
    @graph.make_move(move)
    assert_equal "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq -", @graph.to_fen
  end
  def test_move_to_san
    move = @graph.move_from_san("e4")
    san = @graph.move_to_san(move)
    assert_equal "e4", san
  end
  def test_for_each_move_made
    load_sans @@morphy_sans
    sans = sans_from_graph
    assert_equal @@morphy_sans, sans
  end
  def test_to_pgn
    @@morphy_tags.each_pair { |key, value| @game_tags.set(key, value) }
    load_sans @@morphy_sans
    pgn = @graph.to_pgn(@game_tags)
    assert_equal @@morphy_pgn, pgn
  end
  def test_from_pgn
    @graph.from_pgn(@@morphy_pgn)
    sans = sans_from_graph
    assert_equal @@morphy_sans, sans
  end
  def test_pgn_without_game_tags
    @graph.from_pgn("d4 d5 c4")
    pgn = @graph.to_pgn
    assert_equal "1. d4 d5 2. c4 *\n", pgn
  end
  def test_from_pgn_file
    players = ["Abrahamyan, Tatev", "Foisor, Sabina", "Goletiani, Rusudan", "Krush, Irina", "Zatonskih, Anna"]
    scores = players.inject(Hash.new) do |result, player| 
      result[player] = 0
      result
    end
    file = File.open("test/us_women_in_2012_olympiad.pgn")
    assert file
    top_rated_win = 0
    top_rated_win_pgn = nil
    @graph.from_pgn(file, @game_tags) do
      white = @game_tags.get("White")
      black = @game_tags.get("Black")
      result = @game_tags.get("Result")
      white_elo = @game_tags.get("WhiteElo").to_i
      black_elo = @game_tags.get("BlackElo").to_i
      score = other_elo = nil
      if players.include? white
        usa = white
        score = result == "1-0" ? 1.0 : (result == "1/2-1/2" ? 0.5 : 0.0)
        other_elo = black_elo
      else
        assert false, @game_tags.inspect unless (players.include? black)
        usa = black
        score = result == "0-1" ? 1.0 : (result == "1/2-1/2" ? 0.5 : 0.0)
        other_elo = white_elo
      end
      scores[usa] += score
      if score == 1.0 && other_elo > top_rated_win
        top_rated_win = other_elo
        top_rated_win_pgn = @graph.to_pgn(@game_tags)
      end
    end
    expected_scores = {
      "Abrahamyan, Tatev"=>5.0,
      "Foisor, Sabina"=>5.0,
      "Goletiani, Rusudan"=>4.5,
      "Krush, Irina"=>8.5,
      "Zatonskih, Anna"=>6.5
    }
    assert_equal expected_scores, scores
    expected_pgn = "[Event \"Olympiad\"]\n" +
    "[Site \"Istanbul, TUR\"]\n" +
    "[Date \"2012.08.30\"]\n" +
    "[Round \"03\"]\n" +
    "[White \"Zatonskih, Anna\"]\n" +
    "[Black \"Muminova, Nafisa\"]\n" +
    "[Result \"1-0\"]\n" +
    "[WhiteElo \"2512\"]\n" +
    "[BlackElo \"2329\"]\n" +
    "\n" +
    "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nge7 5. Bb3 Ng6 6. d4 exd4 7. Nxd4 Qf6 8.\n" +
    "Nf3 h6 9. O-O d6 10. Nc3 Be6 11. Nd5 Bxd5 12. Bxd5 Be7 13. Be3 Nf4 14. Bb3\n" +
    "O-O-O 15. c3 g5 16. Nd4 Ne5 17. Kh1 Qg6 18. f3 h5 19. Qd2 Rdg8 20. Nf5 Bd8 21.\n" +
    "Rad1 h4 22. Bxf4 gxf4 23. h3 Rh5 24. Rf2 Bg5 25. a4 Rhh8 26. Ba2 Rd8 27. Qc2\n" +
    "Rhg8 28. b4 Rd7 29. a5 Kb8 30. c4 Bd8 31. b5 c5 32. Qc3 Qf6 33. bxa6 Ng4 34.\n" +
    "Qc2 Nxf2+ 35. Qxf2 bxa6 36. Bb3 Rg6 37. Qd2 Rb7 38. Bc2 Bc7 39. Qxf4 Qb2 40.\n" +
    "Qd2 Bxa5 41. Qxa5 Qxc2 42. Rg1 Ka7 43. Qd8 Rb1 44. Qc7+ Rb7 45. Qd8 Rb1 46.\n" +
    "Qe7+ Rb7 47. Qxh4 Qxc4 48. Ne7 Re6 49. Nc6+ Kb6 50. Nd8 Qb3 51. Nxe6 fxe6 52.\n" +
    "Qd8+ Kc6 53. e5 dxe5 54. Rd1 Kb5 55. Rc1 Qb2 56. Qd3+ Kc6 57. Qxa6+ Qb6 58.\n" +
    "Qa4+ Kd6 59. Rd1+ Ke7 60. Qh4+ Ke8 61. Qh8+ Ke7 62. Qxe5 Qc7 63. Qg5+ Ke8 64.\n" +
    "Rc1 c4 65. Qg4 Qe5 66. Qxc4 1-0\n"
    assert_equal expected_pgn, top_rated_win_pgn
  end
end
