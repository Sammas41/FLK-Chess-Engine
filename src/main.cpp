#include "general.h"
#include "attacks.h"
#include "magic.h"

int main()
{
	init_precalc_attack_tables();

	// Run and see if the magic numbers coincides
	// with the ones stored in "attacks.h"
	initialize_magic_numbers();

	return 0;
}
