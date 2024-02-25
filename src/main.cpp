#include "general.h"
#include "attacks.h"
#include "game.h"
#include "moveGenerator.h"

int main()
{
	
	Game game;
	
	print_attacked_squares(white);
	generate_moves();
	

	return 0;
}
