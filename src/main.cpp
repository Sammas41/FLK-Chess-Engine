#include "general.h"
#include "attacks.h"
#include "magic.h"
#include <memory>

int main()
{
	std::unique_ptr<Attacks> ptr_attacks = std::make_unique<Attacks>();
	
	// Test for magic numbers generation:
	initialize_magic_numbers(*ptr_attacks);

	// Test for move retrievement
	U64 bitboard = 0ULL;

	bitboard = set_bit(bitboard, g6);
	bitboard = set_bit(bitboard, d3);

	std::cout << "Bitboard\n";
	print_bitboard(bitboard);

	std::cout << "Bishop attack on the current bitboard\n";
	print_bitboard(ptr_attacks->get_bishop_attack(e4, bitboard));

	std::cout << "Rook attack on the current bitboard\n";
	print_bitboard(ptr_attacks->get_rook_attack(d6, bitboard));

	return 0;
}
