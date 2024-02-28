#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "attacks.h"
#include "game.h"
#include "mover.h"
#include <memory>

class MoveGenerator
{

    Mover mover;

    private:

        Game& game; // Reference to a Game object

        // Additional state variables for copying
        U64 bitboards_copy[12], occupancies_copy[3];
        int side_copy, enpassant_copy, castle_copy;

        void generate_white_pawns_moves(int ,U64&);
        void generate_white_king_castling_moves(int , U64&);
        void generate_black_pawns_moves(int ,U64&);
        void generate_black_king_castling_moves(int , U64&);
        void generate_knights_moves(int, U64&, int);
        void generate_bishops_moves(int, U64&, int);
        void generate_rooks_moves(int, U64&, int);
        void generate_queens_moves(int, U64&, int);
        void generate_kings_moves(int, U64&, int);

    public:
        MoveGenerator() = delete;   // Cannot initialize MoveGenerator without a game
        MoveGenerator(Game& g) : game(g){};      // Default constructor
        Mover& getMover() {return mover;}
        int is_square_attacked(int, int);
        void print_attacked_squares(int);
        void copyBoardState();
        void takeBack();
        void generate_moves();
        int make_move(int, int);


};

#endif