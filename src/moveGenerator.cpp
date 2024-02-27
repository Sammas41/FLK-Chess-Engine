#include "moveGenerator.h"


MoveGenerator::MoveGenerator(Game & g)
{
    game = Game(g);
}

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
    // define source and target square
    int source_square;
    int target_square;

    // define current pieces bitboard copy and relative attacks
    U64 bitboard;
    U64 attacks;
 
    // loop through all bitboards
    for(int piece = P; piece < k; piece++){
        // init piece bitboard copy
        bitboard = game.get_bitboard(piece);

        // generate white pawns & white king castling
        // not relying on precalculated attack tables
        if(game.get_side() == white){
            if (piece == P){
                // loop over white pawns (while theres bits(pieces) available)
                while (bitboard){
                    source_square = get_ls1b_index(bitboard);
                    // move white pawn forward
                    target_square = source_square - 8;

                    // generate quite pawn moves
                    if(!(target_square < 0) && !get_bit(game.get_occupancy(both), target_square)){
                        // three type of moves
                        // PAWN PROMOTION
                        if (source_square >= a7 && source_square <= h7){
                            printf("pawn promotion: %s%s q\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%s r\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%s b\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%s n\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                
                        } 
                        else{
                            // ONE SQUARE AHEAD MOVE
                            printf("pawn push: %s%s\n",square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            // TWO SQUARES AHEAD MOVE
                            if((source_square >= a2 && source_square <= h2) && !get_bit(game.get_occupancy(both), target_square - 8)){
                                  printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square - 8]);                              
                            }

                        }
                    }

                    attacks = get_pawn_attack(white,source_square) & game.get_occupancy(black);

                    // generate pawn captures
                    while(attacks){

                        target_square = get_ls1b_index(attacks);

                        // CAPTURE AND PROMOTION
                        if (source_square >= a7 && source_square <= h7){
                            printf("pawn promotion capture: %s%s q\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion capture promotion: %s%s r\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion capture promotion: %s%s b\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion capture promotion: %s%s n\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                
                        } 
                        else{
                            // CAPTURE
                            printf("pawn capture: %s%s\n",square_to_coordinates[source_square],square_to_coordinates[target_square]);
                        }
                        pop_bit(attacks, target_square);

                        // ENPASSANT
                        if( game.get_enpassant() != no_sq){

                            // look pawn attacks and bitwise and with enpassant square
                            U64 enpassant_attacks = get_pawn_attack(game.get_side(),source_square) & (1ULL << game.get_enpassant());

                            // make sure enpassant capture available
                            if (enpassant_attacks){
                                //init enpassant capture square
                                int target_enpassant = get_ls1b_index(enpassant_attacks);
                                    printf("pawn enpassant capture: %s%s\n",square_to_coordinates[source_square],square_to_coordinates[target_enpassant]);
                            }
                        }
                    }


                    // pop ls1b from piece bitboard copy
                    pop_bit(bitboard, source_square);

                }
            }

            // CASTLING
            if(piece == K){
                // king side castling available
                if (game.get_castle() & game.wk){
                    // check no pieces between K and R
                    if(!get_bit(game.get_occupancy(both), f1) && !get_bit(game.get_occupancy(both), g1)){
                        // king and f1 squares not attacked
                        if(!is_square_attacked(e1,black) && !is_square_attacked(f1,black)){
                            printf("castling king side: %s\n","e1g1\n");
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
                            printf("castling queen side: e1c1\n");
                    }
                    
                }
            }


        }
        // generate black pawns & black king castling
        else{

            if (piece == p){
                // loop over black pawns (while theres bits(pieces) available)
                while (bitboard){
                    source_square = get_ls1b_index(bitboard);
                    // move black pawn forward
                    target_square = source_square + 8;

                    // generate quite pawn moves
                    if(!(target_square > h1) && !get_bit(game.get_occupancy(both), target_square)){
                        // three type of moves
                        // PAWN PROMOTION
                        if (source_square >= a2 && source_square <= h2){
                            printf("pawn promotion: %s%s q\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%s r\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%s b\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%s n\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                
                        } 
                        else{
                            // ONE SQUARE AHEAD MOVE
                            printf("pawn push: %s%s\n",square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            // TWO SQUARES AHEAD MOVE
                            if((source_square >= a7 && source_square <= h7) && !get_bit(game.get_occupancy(both), target_square + 8)){
                                  printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square + 8]);                              
                            }

                        }
                    }

                    attacks = get_pawn_attack(black,source_square) & game.get_occupancy(white);

                    // generate pawn captures
                    while(attacks){

                        target_square = get_ls1b_index(attacks);

                        // CAPTURE AND PROMOTION
                        if (source_square >= a2 && source_square <= h2){
                            printf("pawn promotion capture: %s%s q\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion capture promotion: %s%s r\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion capture promotion: %s%s b\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion capture promotion: %s%s n\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                
                        } 
                        else{
                            // CAPTURE
                            printf("pawn capture: %s%s\n",square_to_coordinates[source_square],square_to_coordinates[target_square]);
                        }
                        pop_bit(attacks, target_square);

                        // ENPASSANT
                        if( game.get_enpassant() != no_sq){

                            // look pawn attacks and bitwise and with enpassant square
                            U64 enpassant_attacks = get_pawn_attack(game.get_side(),source_square) & (1ULL << game.get_enpassant());

                            // make sure enpassant capture available
                            if (enpassant_attacks){
                                //init enpassant capture square
                                int target_enpassant = get_ls1b_index(enpassant_attacks);
                                    printf("pawn enpassant capture: %s%s\n",square_to_coordinates[source_square],square_to_coordinates[target_enpassant]);
                            }
                        }
                    }

                    // pop ls1b from piece bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }
            // castling moves
            if (piece == k)
            {
                // king side castling is available
                if (game.get_castle() & game.bk)
                {
                    // make sure square between king and king's rook are empty
                    if (!get_bit(game.get_occupancy(both), f8) && !get_bit(game.get_occupancy(both), g8))
                    {
                        // make sure king and the f8 squares are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
                            printf("castling king side: e8g8\n");
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
                            printf("castling queen side: e8c8\n");
                    }
                }
            }

        }

        // generate knight moves
        if(piece == N)
        {
            while(bitboard)
            {
                source_square = get_ls1b_index(bitboard);
                attacks = get_knight_attack(source_square);
                
                while(attacks)
                {
                    target_square = get_ls1b_index(attacks);

                    if(!get_bit(game.get_occupancy(both), target_square))
                        std::cout << "knight move: " << square_to_coordinates[source_square] 
                                                     << square_to_coordinates[target_square] << "\n";
                    if(get_bit(game.get_occupancy(black), target_square))
                        std::cout << "knight capture: " << square_to_coordinates[source_square] 
                                                        << square_to_coordinates[target_square] << "\n";

                    pop_bit(attacks, target_square);
                }

                pop_bit(bitboard, source_square);
            }

        }

        // generate bishop moves


        // generate rook moves


        // generate queen moves


        // generate king moves

    }

}
