#include "moveGenerator.h"

int main()
{

	//test FEN
	std::string test = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQK11R w KQkq - 1 2"; 

	Game game(test);
	game.print_board();	
	
	MoveGenerator move_gen(game);
	move_gen.print_attacked_squares(white);

	
	move_gen.generate_moves();

	return 0;
}
