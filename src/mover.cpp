#include "mover.h"
// move encoding

/*            
          binary move bits                               hexidecimal 
    
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/



unsigned int Mover::encodeMove(unsigned int source, unsigned int target, unsigned int piece, unsigned int promoted, unsigned int capture, unsigned int doublePush, unsigned int enpassant, unsigned int castling) {
    return source |
           (target << 6) |
           (piece << 12) |
           (promoted << 16) |
           (capture << 20) |
           (doublePush << 21) |
           (enpassant << 22) |
           (castling << 23);
}

unsigned int Mover::getMoveSource(unsigned int move) {
    return move & 0x3f;
}

unsigned int Mover::getMoveTarget(unsigned int move) {
    return (move & 0xfc0) >> 6;
}

unsigned int Mover::getMovePiece(unsigned int move) {
    return (move & 0xf000) >> 12;
}

unsigned int Mover::getMovePromoted(unsigned int move) {
    return (move & 0xf0000) >> 16;
}

unsigned int Mover::getMoveCapture(unsigned int move) {
    return move & 0x100000;
}

unsigned int Mover::getMoveDouble(unsigned int move) {
    return move & 0x200000;
}

unsigned int Mover::getMoveEnpassant(unsigned int move) {
    return move & 0x400000;
}

unsigned int Mover::getMoveCastling(unsigned int move) {
    return move & 0x800000;
}

// Add a move to the move list
void Mover::add_move(int move) {
    moveList.movesArray[moveList.count] = move;  // Store move
    moveList.count++;  // Increment move count
}


// Print move 
void Mover::print_move(int move) {
    printf("%s%s%c\n", square_to_coordinates[getMoveSource(move)],
                       square_to_coordinates[getMoveTarget(move)],
                       promoted_pieces[getMovePromoted(move)]);
}

moves Mover::get_move_list(){
    return moveList;
}


void Mover::print_move_list(){

    // do nothing on empty move list
    if (!moveList.count)
    {
        printf("\n     No move in the move list!\n");
        return;
    }

    printf("\n    move    piece   capture   double    enpass    castling\n\n");
    
    // loop over moves within a move list
    for (int move_count = 0; move_count < moveList.count; move_count++)
    {
        // init move
        int move = moveList.movesArray[move_count];

        // print move
        printf("    %s%s%c   %c       %d         %d         %d         %d\n", square_to_coordinates[Mover::getMoveSource(move)],
                                                                                square_to_coordinates[Mover::getMoveTarget(move)],
                                                                                Mover::getMovePromoted(move) ? promoted_pieces[Mover::getMovePromoted(move)] : ' ',
                                                                                ascii_pieces[Mover::getMovePiece(move)],
                                                                                Mover::getMoveCapture(move) ? 1 : 0,
                                                                                Mover::getMoveDouble(move) ? 1 : 0,
                                                                                Mover::getMoveEnpassant(move) ? 1 : 0,
                                                                                Mover::getMoveCastling(move) ? 1 : 0);
    
        
    }
    // print total number of moves
    printf("\n\n    Total number of moves: %d\n\n", moveList.count);
}

