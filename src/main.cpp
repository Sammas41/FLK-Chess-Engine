#include "general.h"
#include "attacks.h"

int main()
{
	init_precalc_attack_tables();
	
	U64 bitboard = knight_attacks[e4];

	std::cout << "Number of bits: " << count_bits(knight_attacks[e4]) << "\n";
	std::cout << "Index of the 1st non zero bit: " << get_ls1b_index(knight_attacks[e4]) << "\n";

	// Answers: Number of bits: 8, Index: 19

	return 0;
}
