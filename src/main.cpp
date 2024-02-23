#include "general.h"
#include "attacks.h"
#include "game.h"

int main()
{
	
	Game game;
	
	std::string fen = "r3kb1r/3n1pp1/p6p/2pPp2q/Pp2N3/3B2PP/1PQ2P2/R3K2R w KQkq -";

	game.initialize_pieces_bitboards(fen);

	game.print_board();

	return 0;
}
