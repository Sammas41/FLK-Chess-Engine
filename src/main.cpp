#include "search.h"
#include "moveGenerator.h"
#include "evaluation.h"


int main()
{
	init_all_attacks();

	std::string trickyPos = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	std::string endgamePos = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
	std::string checkPos = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	std::string bugPos = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ";
	std::string genericPos = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

	Game game;
	game.print_board();	

	int depth = 4;
	flk::Perft(game, depth);

	return 0;
}
