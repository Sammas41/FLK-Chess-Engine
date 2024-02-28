
#include "mover.h"


int main()
{

	init_all_attacks();

    Mover mover;

    Mover::moves move_list;

    std::string test = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2"; 

	Game game(test);
	game.print_board();	

	MoveGenerator move_gen(game);
	move_gen.print_attacked_squares(white);

	move_gen.generate_moves();

	int move = Mover::encodeMove(d7,d8,B,R,0,0,0,1);
	
   
    mover.print_move( move);
    mover.add_move(move_list, move);
    mover.print_move_list(move_list);


	


	return 0;
}
