#include "general.h"
#include "attacks.h"
#include "game.h"
#include "moveGenerator.h"

int main()
{
	std::string fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"; 

	Game game(fen);


	game.print_board();
	print_attacked_squares(black);
	generate_moves();
	

	return 0;
}
