#include "general.h"
#include "attacks.h"
#include "game.h"
#include "moveGenerator.h"

int main()
{
	
	Game game;
	game.print_board();	
	
	print_attacked_squares(black);
	generate_moves();
	

	return 0;
}
