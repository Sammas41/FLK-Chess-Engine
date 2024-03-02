#include "search.h"
#include "moveGenerator.h"
#include "evaluation.h"


int main()
{
	init_all_attacks();

	Game game;
	game.print_board();	
	
	int debug = 0;
	if(debug == 0)
	{
		MoveGenerator m(game);
		m.generate_moves();

		m.copyBoardState();

		std::cout << "Game bitboards before a3 move:\n";
        for(int piece = P; piece <= k; piece++)
        {
            std::cout << "  [" << piece << "]: " << game.get_bitboard(piece) << "\n";
        }
        system("PAUSE");
		
		m.make_move(m.getMover().moveList.movesArray[0], all_moves);

		std::cout << "Game bitboards after a3 move:\n";
        for(int piece = P; piece <= k; piece++)
        {
            std::cout << "  [" << piece << "]: " << game.get_bitboard(piece) << "\n";
        }
        system("PAUSE");

		m.takeBack();
	}
	else
	{
		int best_move = 0;
		int b = flk::negamax(&game, 1, -50000, 50000, best_move);

		std::cout << "Best move: ";
		//m.getMover().print_move(best_move);
		std::cout << "\n";
	}
	
	return 0;
}
