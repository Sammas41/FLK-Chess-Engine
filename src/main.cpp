
#include "moveGenerator.h"


int main()
{

	init_all_attacks();


    std::string test = "r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPBqPPP/R3K2R w KQkq - 0 1"; 

	Game game(test);
	game.print_board();	

	MoveGenerator movegen(game);
	movegen.generate_moves();

	moves mov = movegen.getMover().get_move_list();
	int move_count = movegen.getMover().get_move_list().count;
	
	for (int i=0; i<move_count;i++){
		int move = mov.movesArray[i];
		movegen.copyBoardState();

		if(movegen.make_move(move, all_moves)
){
		game.print_board();
		movegen.takeBack();
		game.print_board();
}
		
		


	}
	

	

	return 0;
}
