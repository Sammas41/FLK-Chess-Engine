#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "attacks.h"
#include "game.h"
#include "mover.h"
#include <vector>
#include <forward_list>

class MoveGenerator
{
    Mover mover;

    private:

        Game game; // Reference to a Game object

        // Additional state variables for copying
        U64 bitboards_copy[12], occupancies_copy[3];
        int side_copy, enpassant_copy, castle_copy;

        void generate_white_pawns_moves(int ,U64);
        void generate_white_king_castling_moves(int , U64);
        void generate_black_pawns_moves(int ,U64);
        void generate_black_king_castling_moves(int , U64);
        void generate_knights_moves(int, U64, int);
        void generate_bishops_moves(int, U64, int);
        void generate_rooks_moves(int, U64, int);
        void generate_queens_moves(int, U64, int);
        void generate_kings_moves(int, U64, int);

        public:
        // Encode moves in the correct format
        unsigned int encode_move(unsigned int,       // source square
                                unsigned int,       // target square
                                unsigned int,       // piece
                                unsigned int,       // promotion
                                unsigned int,       // capture
                                unsigned int,       // double pawn push
                                unsigned int,       // en passant
                                unsigned int);      // castling

        std::forward_list<int> legal_moves;
        int size;

        MoveGenerator() = delete;   // Cannot initialize MoveGenerator without a game
        MoveGenerator(Game& g) : game(g){};      // Default constructor
        Mover& getMover() {return mover;}

        int is_square_attacked(int, int);
        void print_attacked_squares(int);
        void generate_moves();
        
        bool is_legal(int);
        moves get_capture_move_list();

        int get_move_list_size();
        void print_move_list();
};

#endif