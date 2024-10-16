#include "search.h"
#include "test.h"
#include "engine.h"

int main()
{
	init_all_attacks();
	flk::init_evaluation_masks();

	Engine e;
	e.run();

	return 0; 
}
