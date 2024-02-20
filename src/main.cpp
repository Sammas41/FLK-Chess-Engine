#include "general.h"
#include "attacks.h"

int main()
{
	init_precalc_attack_tables();

	U64 bitboard = 0ULL;
	bitboard = set_bit(bitboard, 28);
	bitboard = set_bit(bitboard, h8);
	
	// TODO: change ordering of bits, a1 = 0 ---> h8
	// TODO: check set_bit function

	// Set a blocker on e5
/*     bitboard |= (1ULL << 28);
	// Set a blocker on a7
	bitboard |= (1ULL << 8);
	// Set a blocker on c5
	bitboard |= (1ULL << 26); 
	// Set a blocker on f7
	bitboard |= (1ULL << 13);  */

	print_bitboard(bitboard);
	print_bitboard(generate_rook_attacks_with_blockers(f5,bitboard)); 
	print_bitboard(generate_bishop_attacks_with_blockers(f6,bitboard)); 

	print_bitboard(FENtoBitboard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));
	
	
	return 0;
}
