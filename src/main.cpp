#include "general.h"
#include "attacks.h"
#include "magic.h"

int main()
{
	init_precalc_attack_tables();
	// initialize_magic_numbers();

	U64 bitboard = 0ULL;

	bitboard = set_bit(bitboard, g6);
	bitboard = set_bit(bitboard, d3);

	std::cout << "Bitboard\n";
	print_bitboard(bitboard);

	std::cout << "Bishop attack on the current bitboard\n";
	print_bitboard(get_bishop_attacks(e4, bitboard));

	std::cout << "Rook attack on the current bitboard\n";
	print_bitboard(get_rook_attacks(d6, bitboard));

	return 0;
}
