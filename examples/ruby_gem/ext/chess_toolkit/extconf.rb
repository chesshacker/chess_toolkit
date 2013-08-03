require 'mkmf'
dir_config('chess_toolkit')
have_library('chess_toolkit')
create_makefile("chess_toolkit")
