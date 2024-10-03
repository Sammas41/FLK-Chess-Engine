#include "search.h"
#include "test.h"
#include "engine.h"

#include<chrono>

int main()
{
	init_all_attacks();

	// Engine e("4k3/R7/8/8/8/8/8/4K2R w K - 0 1");
	// e.play('w');
	
	Game game(flk::test::trickyPos);
	game.print_board();

	int depth = 7;

	auto t1 = std::chrono::high_resolution_clock::now();
	Move move = flk::iterative_search(game, depth);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	std::cout << "Search took: " << t.count() * 0.001 << " seconds\n";
		
	return 0;
}
