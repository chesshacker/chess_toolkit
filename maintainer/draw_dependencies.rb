#!/usr/bin/env ruby

#../maintainer/draw_dependencies.rb | dot -Tpdf -o ../maintainer/dependencies.pdf

#ignore = %w{types types_internal error utilities}
ignore = %w{types types_internal error utilities square piece move rays bit_board position_private graph_private
   position command move_command piece_command debug_utilities graph_position graph_dfs}

dependencies = `grep ^#include *.c *.h`.split("\n")

puts "digraph G {"
dependencies.each do |dependency|
  if dependency =~ /ct_(\w+).([ch]):#include[ \t]+"ct_(\w+).h"/
    from = $1
    type = $2
    to = $3
    edge_modifier = type == 'h' ? "[color=red]" : ""
    if from != to and not ignore.include?(from) and not ignore.include?(to)
      puts "  \"#{from}\" -> \"#{to}\" #{edge_modifier};"
    end
  end
end
puts "}"