#include "general.h"
#include "attacks.h"

int main()
{
	init_precalc_attack_tables();
	print_bitboard(rook_attacks[d8]);
	return 0;
}
