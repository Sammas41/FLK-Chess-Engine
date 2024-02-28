#include "moveGenerator.h"

int MoveGenerator::is_square_attacked(int square, int side){
    
    // attacked by white pawns
    if((side == white) && 
       (get_pawn_attack(black,square) & game.get_bitboard(P))) return 1;
    
    // attacked by black pawns
    if((side == black) && 
       (get_pawn_attack(white,square) & game.get_bitboard(p))) return 1;
    
    // attacked by knights
    if(get_knight_attack(square) & 
      ((side == white) ? game.get_bitboard(N) : game.get_bitboard(n))) return 1;
    
    // attacked by king
    if(get_king_attack(square) & 
      ((side == white) ? game.get_bitboard(K) : game.get_bitboard(k))) return 1;
    
    // attacked by bishops
    if(get_bishop_attack(square, game.get_occupancy(both)) & 
      ((side == white) ? game.get_bitboard(B) : game.get_bitboard(b))) return 1;
    
    // attacked by rooks
    if(get_rook_attack(square, game.get_occupancy(both)) & 
      ((side == white) ? game.get_bitboard(R) : game.get_bitboard(r))) return 1;

    // attacked by queens
    if(get_queen_attack(square, game.get_occupancy(both)) & 
      ((side == white) ? game.get_bitboard(Q) : game.get_bitboard(q))) return 1;
    
    // by default return false (square not attacked)
    return 0;
}

// print attacked squares
void MoveGenerator::print_attacked_squares(int side){

    printf("\n");

    for (int rank = 0; rank < 8; rank ++){

        for (int file = 0; file < 8; file++){

            int square = rank * 8 + file;

            // print ranks
            if(!file) {
                printf("  %d   ", 8 - rank);
            }   

            // check wether current square is attacked  
            printf("%d ",is_square_attacked(square, side) ? 1 : 0);

        }

        // print new line every rank
        printf("\n");
    }

    // print files
    printf("\n      a b c d e f g h\n\n");
}

// Generate pawn moves (push, double push, captures and en passant)
void MoveGenerator::generate_moves(){

    // loop through all bitboards
    for(int piece = P; piece < k; piece++){
        // init piece bitboard copy
        U64 bitboard = game.get_bitboard(piece);

        // generate white pawns & white king castling
        // not relying on precalculated attack tables
        if(game.get_side() == white){
            
            generate_white_pawns_moves(piece, bitboard);
            generate_white_king_castling_moves(piece, bitboard);
            
        }
        // generate black pawns & black king castling
        else{

            generate_black_pawns_moves(piece, bitboard);
            generate_black_king_castling_moves(piece, bitboard);
            
        }


        // generate knight moves
        generate_knights_moves(piece,bitboard,game.get_side());

        // generate bishop moves
        generate_bishops_moves(piece,bitboard,game.get_side());

        // generate rook moves
        generate_rooks_moves(piece,bitboard,game.get_side());

        // generate queen moves
        generate_queens_moves(piece,bitboard,game.get_side());

        // generate king moves
        generate_kings_moves(piece,bitboard,game.get_side());

    }

}

void MoveGenerator::generate_white_pawns_moves(int piece, U64 & bitboard){
    if (piece == P){
        // loop over white pawns (while theres bits(pieces) available)
        while (bitboard){
            int source_square = get_ls1b_index(bitboard);
            // move white pawn forward
            int target_square = source_square - 8;

            // generate quite pawn moves
            if(!(target_square < 0) && !get_bit(game.get_occupancy(both), target_square)){
                // three type of moves
                // PAWN PROMOTION
                if (source_square >= a7 && source_square <= h7){
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, Q, 0,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, R, 0,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, B, 0,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, N, 0,0,0,0));    
                } 
                else{
                    // ONE SQUARE AHEAD MOVE
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 0,0,0,0));

                    // TWO SQUARES AHEAD MOVE
                    if((source_square >= a2 && source_square <= h2) && !get_bit(game.get_occupancy(both), target_square - 8)){
                        mover.add_move(mover.encodeMove(source_square,target_square - 8,piece, 0, 0,1,0,0));
                    }

                }
            }

            U64 attacks =  get_pawn_attack(white,source_square) & game.get_occupancy(black);

            // generate pawn captures
            while(attacks){

                target_square = get_ls1b_index(attacks);

                // CAPTURE AND PROMOTION
                if (source_square >= a7 && source_square <= h7){
                    mover.add_move(mover.encodeMove(source_square, target_square, piece, Q, 1, 0, 0, 0));
                    mover.add_move(mover.encodeMove(source_square, target_square, piece, R, 1, 0, 0, 0));
                    mover.add_move(mover.encodeMove(source_square, target_square, piece, B, 1, 0, 0, 0));
                    mover.add_move(mover.encodeMove(source_square, target_square, piece, N, 1, 0, 0, 0));

                } 
                else{
                    // CAPTURE
                    mover.add_move(mover.encodeMove(source_square, target_square, piece, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, target_square);

                // ENPASSANT
                if( game.get_enpassant() != no_sq){

                    // look pawn attacks and bitwise and with enpassant square
                    U64 enpassant_attacks =  get_pawn_attack(game.get_side(),source_square) & (1ULL << game.get_enpassant());

                    // make sure enpassant capture available
                    if (enpassant_attacks){
                        //init enpassant capture square
                        int target_enpassant = get_ls1b_index(enpassant_attacks);
                        mover.add_move(mover.encodeMove(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
                    }
                }
            }

            // pop ls1b from piece bitboard copy
            pop_bit(bitboard, source_square);

        }
    }
}


// castling moves
void MoveGenerator::generate_white_king_castling_moves(int piece, U64 & bitboard){
    if(piece == K){
        // king side castling available
        if (game.get_castle() & game.wk){
            // check no pieces between K and R
            if(!get_bit(game.get_occupancy(both), f1) && !get_bit(game.get_occupancy(both), g1)){
                // king and f1 squares not attacked
                if(!is_square_attacked(e1,black) && !is_square_attacked(f1,black)){
                    mover.add_move(mover.encodeMove(e1, g1, piece, 0, 0, 0, 0, 1));
                }
            }
        }

        // queen side available
        if (game.get_castle() & game.wq){
            // make sure square between king and queen's rook are empty
            if (!get_bit(game.get_occupancy(both), d1) && !get_bit(game.get_occupancy(both), c1) && !get_bit(game.get_occupancy(both), b1))
            {
                // make sure king and the d1 squares are not under attacks
                if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
                    mover.add_move(mover.encodeMove(e1, c1, piece, 0, 0, 0, 0, 1));
            }
            
        }
    }
}

void MoveGenerator::generate_black_pawns_moves(int piece, U64 & bitboard){

    if (piece == p){
        // loop over black pawns (while theres bits(pieces) available)
        while (bitboard){
            int source_square = get_ls1b_index(bitboard);
            // move black pawn forward
            int target_square = source_square + 8;

            // generate quite pawn moves
            if(!(target_square > h1) && !get_bit(game.get_occupancy(both), target_square)){
                // three type of moves
                // PAWN PROMOTION
                if (source_square >= a2 && source_square <= h2){
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, q, 0,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, r, 0,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, b, 0,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, n, 0,0,0,0));
        
                } 
                else{
                    // ONE SQUARE AHEAD MOVE
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 0,0,0,0));
                    // TWO SQUARES AHEAD MOVE
                    if((source_square >= a7 && source_square <= h7) && !get_bit(game.get_occupancy(both), target_square + 8)){
                        mover.add_move(mover.encodeMove(source_square,target_square + 8,piece,0, 0,1,0,0));
                    }

                }
            }

            U64 attacks =  get_pawn_attack(black,source_square) & game.get_occupancy(white);

            // generate pawn captures
            while(attacks){

                target_square = get_ls1b_index(attacks);

                // CAPTURE AND PROMOTION
                if (source_square >= a2 && source_square <= h2){
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, q, 1,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, r, 1,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, b, 1,0,0,0));
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, n, 1,0,0,0));
        
                } 
                else{
                    // CAPTURE
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 1,0,0,0));
                }
                pop_bit(attacks, target_square);

                // ENPASSANT
                if( game.get_enpassant() != no_sq){

                    // look pawn attacks and bitwise and with enpassant square
                    U64 enpassant_attacks =  get_pawn_attack(game.get_side(),source_square) & (1ULL << game.get_enpassant());

                    // make sure enpassant capture available
                    if (enpassant_attacks){
                        //init enpassant capture square
                        int target_enpassant = get_ls1b_index(enpassant_attacks);
                        mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 1,0,1,0));
                    }
                }
            }

            // pop ls1b from piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}



void MoveGenerator::generate_black_king_castling_moves(int piece, U64 & bitboard){
    // castling moves
    if (piece == k){
        // king side castling is available
        if (game.get_castle() & game.bk)
        {
            // make sure square between king and king's rook are empty
            if (!get_bit(game.get_occupancy(both), f8) && !get_bit(game.get_occupancy(both), g8))
            {
                // make sure king and the f8 squares are not under attacks
                if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
                    mover.add_move(mover.encodeMove(e8,g8,piece, 0, 0,0,0,1));
            }
        }
        
        // queen side castling is available
        if (game.get_castle() & game.bq)
        {
            // make sure square between king and queen's rook are empty
            if (!get_bit(game.get_occupancy(both), d8) && !get_bit(game.get_occupancy(both), c8) && !get_bit(game.get_occupancy(both), b8))
            {
                // make sure king and the d8 squares are not under attacks
                if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
                    mover.add_move(mover.encodeMove(e8,c8,piece, 0, 0,0,0,1));
            }
        }
    }
}

void MoveGenerator::generate_knights_moves(int piece, U64 & bitboard, int side){
     // genarate knight moves
    if ((side == white) ? piece == N : piece == n)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_knight_attack(source_square) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}

void MoveGenerator::generate_bishops_moves(int piece, U64& bitboard, int side){
    // generate bishop moves
    if ((side == white) ? piece == B : piece == b)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_bishop_attack(source_square, game.get_occupancy(both)) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}




void MoveGenerator::generate_rooks_moves(int piece, U64& bitboard, int side){

    // generate rook moves
    if ((side == white) ? piece == R : piece == r)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_rook_attack(source_square, game.get_occupancy(both)) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
        
}
void MoveGenerator::generate_queens_moves(int piece, U64& bitboard, int side){

    // generate queen moves
    if ((side == white) ? piece == Q : piece == q)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_queen_attack(source_square, game.get_occupancy(both)) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}

void MoveGenerator::generate_kings_moves(int piece, U64& bitboard, int side){

    // generate king moves
    if ((side == white) ? piece == K : piece == k)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_king_attack(source_square) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    mover.add_move(mover.encodeMove(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }

            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}


void MoveGenerator::copyBoardState(){

    memcpy(bitboards_copy, game.get_bitboards(), 96); // 96=sizeof(U64) * 12
    memcpy(occupancies_copy, game.get_occupancies(), 24); // 24 = sizeof(U64)*3
    side_copy = game.get_side();
    enpassant_copy = game.get_enpassant();
    castle_copy = game.get_castle();

};

void MoveGenerator::takeBack() {
    game.set_bitboards(bitboards_copy);
    game.set_occupancies(occupancies_copy);
    game.set_side(side_copy);
    game.set_enpassant(enpassant_copy);
    game.set_castle(castle_copy);
}

int MoveGenerator::make_move(int move, int move_flag){
    //quite moves
    if (move_flag == all_moves){
        // preserve board state
        copyBoardState();
        // parse move
        int source_square = Mover::getMoveSource(move);
        int target_square = Mover::getMoveTarget(move);
        int piece = Mover::getMovePiece(move);
        int promoted_piece = Mover::getMovePromoted(move);
        int capture = Mover::getMoveCapture(move);
        int double_push = Mover::getMoveDouble(move);
        int enpassant_flag = Mover::getMoveEnpassant(move);
        int castling_flag = Mover::getMoveCastling(move);

        // move piece
        pop_bit(game.get_bitboard_reference(piece),source_square);
        set_bit(game.get_bitboard_reference(piece),target_square);

        // handling captures
        if (capture){
            // pick up bitboard of opposide side to move
            int start_piece, end_piece;

            if (game.get_side() == white ){
                // black pieces range
                start_piece = p;
                end_piece = k;
            } else {
                // white pieces range
                start_piece = P;
                end_piece = K;               
            }

            for (int bb_piece = start_piece; bb_piece<=end_piece; bb_piece++){
                // check if there's piece on target square
                if (get_bit(game.get_bitboard_reference(bb_piece), target_square)){
                    // remove from correspondant bitboard
                    pop_bit(game.get_bitboard_reference(bb_piece), target_square);
                    break;
                }
            }

        }

        // handling pawn promotion
        if (promoted_piece){
            // erase pawn from target square
            pop_bit(game.get_bitboard_reference(game.get_side()==white?P:p), target_square);

            // create new selected piece on target square
            set_bit(game.get_bitboard_reference(promoted_piece), target_square);
        }   

        // handling enpassant
        if (enpassant_flag){
            // erase opposed pawn
            if (game.get_side()==white){
                pop_bit(game.get_bitboard_reference(p), target_square + 8);
            } else {
                 pop_bit(game.get_bitboard_reference(P), target_square - 8);
               
            }
        }

        // reset enpassant square
        game.set_enpassant(no_sq);

        // handle double pawn push
        if (double_push){
            // set enpassant square 
            if (game.get_side()== white){
                game.set_enpassant(target_square + 8);
            } else {
                game.set_enpassant(target_square - 8);
            }
        } 

        // handle castling
        if (castling_flag){
            
            switch(target_square){
                // white kingside
                case (g1):
                    pop_bit(game.get_bitboard_reference(R),h1);
                    set_bit(game.get_bitboard_reference(R),f1);
                    break;
                // white queenside
                case (c1):
                    pop_bit(game.get_bitboard_reference(R),a1);
                    set_bit(game.get_bitboard_reference(R),d1);
                    break;
                // black kingside
                case (g8):
                    pop_bit(game.get_bitboard_reference(R),h8);
                    set_bit(game.get_bitboard_reference(R),f8);
                    break;
                // black queenside
                case (c8):
                    pop_bit(game.get_bitboard_reference(R),a8);
                    set_bit(game.get_bitboard_reference(R),d8);
                    break;
            }
        }

        return 1;

    }
    //capture moves
    else{
        //make sure move is capture
        if(Mover::getMoveCapture(move)){
            make_move(move, all_moves);
        }
        // otherwise move is not a capture
        else{
            return 0; // don't make the move
        }
        
    }  
}