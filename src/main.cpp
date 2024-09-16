#include "search.h"
#include "test.h"
#include "engine.h"

#include<chrono>

int main()
{
	init_all_attacks();
	
	Game game(flk::test::quiescence_test);
	game.print_board();
	
	int depth = 5;
	Move bestMove;
	/*
	auto t1 = std::chrono::high_resolution_clock::now();
	flk::Perft(game, depth);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> t = t2 - t1;
	std::cout << "Time elapsed: " << t.count() * 0.001 << " seconds\n";
	*/
	
	auto t1 = std::chrono::high_resolution_clock::now();
	int score = flk::negamax(game, depth, -10000, 10000, bestMove);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	MoveGenerator m(game);

	std::cout << "BestMove: ";
	bestMove.print_move();
	std::cout << "\nNumber of nodes searched: " << flk::nodes << "\n";

	std::cout << "Score: " << score << "\n";
	std::cout << "Found in: " << t.count() * 0.001 << " seconds\n";
	
	return 0;
}
