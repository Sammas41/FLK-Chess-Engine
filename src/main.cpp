#include "search.h"
#include "test.h"
#include "engine.h"

#include<chrono>

int main()
{
	init_all_attacks();
	
	// Game game;
	// game.print_board();

	Engine e;
	e.play('b');
	
	/*
	int depth = 6;

	auto t1 = std::chrono::high_resolution_clock::now();
	int score = flk::negamax(game, depth, -50000, 50000);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	std::cout << "BestMove: ";
	flk::pv_table[0][0].print_move();
	std::cout << "\nNumber of nodes searched: " << flk::nodes << "\n";

	std::cout << "Score: " << score << "\n";
	std::cout << "Found in: " << t.count() * 0.001 << " seconds\n";
	
	std::cout << "\nPrincipal variation: ";
	for(int i = 0; i < flk::pv_length[0]; i++)
	{
		flk::pv_table[0][i].print_move();
		std::cout << "  ";
	}
	std::cout << "\n";
	*/
	return 0;
}
