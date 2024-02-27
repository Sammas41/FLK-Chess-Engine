#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "attacks.h"
#include "game.h"
#include <memory>

class MoveGenerator
{
    Game game;
    std::unique_ptr<Attacks> ptr_attacks;

    private:

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
        MoveGenerator(Game &);      // Default constructor

        int is_square_attacked(int, int);
        void print_attacked_squares(int);

        void generate_moves();

};

#endif