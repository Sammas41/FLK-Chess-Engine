#include "general.h"
#include "attacks.h"
#include "game.h"

int main()
{
	
	Game game;
/* 	
	std::string fen = "r3kb1r/3n1pp1/p6p/2pPp2q/Pp2N3/3B2PP/1PQ2P2/R3K2R w KQkq - 0 1";

	game.initialize_pieces_bitboards(fen); */
	print_bitboard(game.get_bitboard(P));
	print_bitboard(game.get_bitboard(k));
	print_bitboard(game.get_bitboard(Q));
	game.print_board();

	return 0;
}
