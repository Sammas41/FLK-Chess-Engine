#include "general.h"
#include "attacks.h"
#include "game.h"

int main()
{
	Attacks attacks;
	Game game;
	U64 bitboard = FENtoBitboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
	
	print_bitboard(attacks.get_queen_attack(e4, bitboard));
	game.print_board();

	return 0;
}
