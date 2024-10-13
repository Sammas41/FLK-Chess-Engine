#include "search.h"
#include "test.h"
#include "engine.h"

int main()
{
	init_all_attacks();
	flk::init_evaluation_masks();

/* 	Engine e;
	e.play(); */
	

	Game game(flk::test::openfiles_board);
	int score = flk::eval_position(game);
	std::cout << "Score: " << score << std::endl;

	/* int depth = 8;
	
	auto t1 = std::chrono::high_resolution_clock::now();
	Move move = flk::iterative_search(game, depth);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	std::cout << "Search took: " << t.count() * 0.001 << " seconds\n";*/
		
	return 0; 
}
