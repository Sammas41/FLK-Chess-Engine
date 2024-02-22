#include "general.h"
#include "attacks.h"

int main()
{
	/* Attacks* attacks = new Attacks(); */
	Attacks attacks;

	
/*  	init_precalc_attack_tables();
	initialize_magic_numbers();  */

	U64 bitboard = 0ULL;

	bitboard = set_bit(bitboard, g6);
	bitboard = set_bit(bitboard, d3);

	std::cout << "Bitboard\n";
	print_bitboard(bitboard);

	std::cout << "Bishop attack on the current bitboard\n";
	print_bitboard(attacks.get_bishop_attack(e4, bitboard));

	std::cout << "Rook attack on the current bitboard\n";
	print_bitboard(attacks.get_rook_attack(d6, bitboard));

	/* delete attacks; */
	
	return 0;
}
