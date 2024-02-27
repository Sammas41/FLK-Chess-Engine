
#include "mover.h"


int main()
{

	int move = Mover::encodeMove(d7,d8,P,R,1,0,0,0);
	// exract move items
    int source_square = Mover::getMoveSource(move);
    int target_square = Mover::getMoveTarget(move);
    int piece = Mover::getMovePiece(move);
    int promoted_piece = Mover::getMovePromoted(move);
    // print move items
    printf("source square: %s\n", square_to_coordinates[source_square]);
    printf("target square: %s\n", square_to_coordinates[target_square]);

    printf("capture flag: %d\n", Mover::getMoveCapture(move) ? 1 : 0);
    printf("double pawn push flag: %d\n", Mover::getMoveDouble(move) ? 1 : 0);
    printf("enpassant flag: %d\n", Mover::getMoveEnpassant(move) ? 1 : 0);
    printf("castling flag: %d\n", Mover::getMoveCastling(move) ? 1 : 0);

/* 	//test FEN
	std::string test = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2"; 

	Game game(test);
	game.print_board();	
	
	MoveGenerator move_gen(game);
	move_gen.print_attacked_squares(white);

	
	move_gen.generate_moves(); */

	return 0;
}
