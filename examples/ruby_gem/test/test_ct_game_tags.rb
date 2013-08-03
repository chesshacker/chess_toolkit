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

class CtGameTagsTest < Test::Unit::TestCase
  def setup
    @game_tags = CtGameTags.new
  end
  def test_inspect
    expected_result = "[Event \"?\"]\n"\
      "[Site \"?\"]\n"\
      "[Date \"?\"]\n"\
      "[Round \"?\"]\n"\
      "[White \"?\"]\n"\
      "[Black \"?\"]\n"\
      "[Result \"*\"]\n"
    assert_equal expected_result, @game_tags.inspect
  end
  def test_get
    assert_equal "*", @game_tags.get("Result")
  end
  def test_set
    @game_tags.set("White", "Morphy, Paul")
    white = @game_tags.get("White")
    assert_equal "Morphy, Paul", white
  end
end
