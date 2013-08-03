TESTS = check_ct
check_PROGRAMS = check_ct
check_ct_SOURCES = \
    at_algebraic_notation.c ut_graph_to_new_pgn.c ut_position_from_fen.c at_perft.c \
    ut_move.c ut_position_hash.c check_main.c ut_move_command.c ut_position_rules.c \
    check_mg_piece.c ut_move_generator.c ut_position_to_fen.c check_utilities.c \
    ut_move_maker.c ut_position_to_s.c ut_bit_board.c ut_move_reader.c ut_rays.c \
    ut_command.c ut_move_stack.c ut_slider.c ut_debug_utilities.c ut_move_writer.c \
    ut_square.c ut_error.c ut_pawn.c ut_steper.c ut_game_tags.c ut_graph_from_pgn.c \
    ut_undo_position.c ut_graph.c ut_piece.c ut_utilities.c ut_graph_dfs.c \
    ut_piece_command.c ut_graph_position.c ut_position.c check_mg_piece.h \
    check_utilities.h ut_bit_board_to_s.c
check_ct_CFLAGS = @CHECK_CFLAGS@ -I../lib -I../lib/chess_toolkit -I../lib/internal_headers
check_ct_LDADD = $(top_builddir)/lib/libchess_toolkit.la @CHECK_LIBS@
