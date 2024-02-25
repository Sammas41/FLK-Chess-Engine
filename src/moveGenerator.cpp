#include "moveGenerator.h"


Attacks attacks;
Game game;

// is current given square attacked by current given side?
int is_square_attacked(int square, int side){

    // attacked by white pawns
    if((side == white) && (attacks.get_pawn_attack(black,square) & game.get_bitboard(P))) return 1;

    // attacked by black pawns
    if((side == black) && (attacks.get_pawn_attack(white,square) & game.get_bitboard(p))) return 1;

    // attacked by knights
    if(attacks.get_knight_attack(square) & ((side == white) ? game.get_bitboard(N) : game.get_bitboard(n))) return 1;

    // attacked by king
    if(attacks.get_king_attack(square) & ((side == white) ? game.get_bitboard(K) : game.get_bitboard(k))) return 1;

    // attacked by bishops
    if(attacks.get_bishop_attack(square, game.get_occupancy(both)) & ((side == white) ? game.get_bitboard(B) : game.get_bitboard(b))) return 1;

    // attacked by rooks
    if(attacks.get_rook_attack(square, game.get_occupancy(both)) & ((side == white) ? game.get_bitboard(R) : game.get_bitboard(r))) return 1;

    // attacked by queens
    if(attacks.get_queen_attack(square, game.get_occupancy(both)) & ((side == white) ? game.get_bitboard(Q) : game.get_bitboard(q))) return 1;

    // by default return false (square not attacked)
    return 0;
}

// print attacked squares
void print_attacked_squares(int side){

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

// generate moves
void generate_moves(){
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
                            printf("pawn promotion: %c%c q\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %c%c r\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %c%c b\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                            printf("pawn promotion: %c%c n\n", square_to_coordinates[source_square],square_to_coordinates[target_square]);
                
            


                        } 
                        else{
                            // ONE SQUARE AHEAD MOVE

                            // TWO SQUARES AHEAD MOVE
                        }
                    }

                    // pop ls1b from piece bitboard copy
                    pop_bit(bitboard, source_square);
                }
            }

        }
        // generate black pawns & black king castling
        else{

        }

        // generate knight moves


        // generate bishop moves


        // generate rook moves


        // generate queen moves


        // generate king moves

    }

}