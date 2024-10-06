#include "search.h"
#include "test.h"
#include "engine.h"

#include<chrono>

int main()
{
	init_all_attacks();
	flk::init_evaluation_masks();
	Engine e;
	e.play('w');
	
/* 	U64 dp_bb = 0ULL;
	set_bit(dp_bb, e2);
	set_bit(dp_bb, e3); */

/* 	U64 wp_bb = FENtoBitboard("K7/P7/P7/8/8/8/8/8");
	U64 bp_bb = FENtoBitboard("8/8/8/8/8/p7/p7/8"); */

/* 	Game game(flk::test::empty_board);
	game.set_bitboard(P,wp_bb );
	game.set_bitboard(p, bp_bb);
	game.print_board();
	 */
/* 	int score = flk::eval_position(game);
	std::cout << "Score: " << score << std::endl; */

	

	/* int depth = 8;
	
	auto t1 = std::chrono::high_resolution_clock::now();
	Move move = flk::iterative_search(game, depth);
	auto t2 = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double, std::milli> t = t2 - t1;

	std::cout << "Search took: " << t.count() * 0.001 << " seconds\n";*/
		
	return 0; 
}
