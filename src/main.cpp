#include "search.h"
#include "test.h"
#include "engine.h"

#include<chrono>

int main()
{
	init_all_attacks();
	
	Game game(flk::test::trickyPos);
	game.print_board();

	Engine e;
	e.play('w');
	
	/*
	int depth = 6;

	auto t1 = std::chrono::high_resolution_clock::now();
	Move move = flk::iterative_search(game, depth);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	std::cout << "Search took: " << t.count() * 0.001 << " seconds\n";
	*/
	return 0;
}
