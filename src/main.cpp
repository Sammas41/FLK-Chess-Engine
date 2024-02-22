#include "general.h"
#include "attacks.h"
#include "game.h"

int main()
{
	Game game;
	
	game.set_bitboard(Game::P , FENtoBitboard("8/8/8/8/8/8/PPPPPPPP/8"));
	
	print_bitboard(game.get_bitboard(Game::P));
	printf("piece %c\n", game.ascii_pieces[Game::P]);
	return 0;
}
