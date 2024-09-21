#include "search.h"
#include "test.h"
#include "engine.h"

#include<chrono>

int main()
{
	init_all_attacks();
	
	Game game(flk::test::killer_test);
	game.print_board();

	int depth = 7;
	Move bestMove;
	MoveGenerator m(game);

	/*
	auto t1 = std::chrono::high_resolution_clock::now();
	flk::Perft(game, depth);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> t = t2 - t1;
	std::cout << "Time elapsed: " << t.count() * 0.001 << " seconds\n";
	*/
	
	auto t1 = std::chrono::high_resolution_clock::now();
	int score = flk::negamax(game, depth, -50000, 50000, bestMove);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	std::cout << "BestMove: ";
	bestMove.print_move();
	std::cout << "\nNumber of nodes searched: " << flk::nodes << "\n";

	std::cout << "Score: " << score << "\n";
	std::cout << "Found in: " << t.count() * 0.001 << " seconds\n";
	
	return 0;
}
