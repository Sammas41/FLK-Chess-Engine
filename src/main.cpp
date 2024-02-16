#include "general.h"
#include "attacks.h"

int main()
{
	init_precalc_attack_tables();
	print_bitboard(knight_attacks[12]);
	return 0;
}
