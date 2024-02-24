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