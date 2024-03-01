#include "moveGenerator.h"
#include "evaluation.h"

int main()
{
	init_all_attacks();

	//test FEN
	std::string test = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2"; 

	std::string test2 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1";

	Game game(test2);
	
	game.print_board();
	std::cout << "Evaluation: " << evaluate(game) << "\n";

	return 0;
}
