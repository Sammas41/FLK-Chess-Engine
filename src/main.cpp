#include "search.h"
#include "moveGenerator.h"
#include "evaluation.h"


int main()
{
	init_all_attacks();

	std::string test = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	std::string test2 = "r3k2r/p1ppqpb1/bn1Ppnp1/4N3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1";
	//std::string per = "rnbqkbnr/1ppppppp/p7/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1";

	Game game;
	game.print_board();	

	int depth = 3, total_moves = 0, total_captures = 0;

	Perft(game, depth, total_moves, total_captures);

	std::cout << "Nodes: " << total_moves - 420 << "\n";
	std::cout << "Nodes: " << total_captures << "\n";

	return 0;
}
