#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "attacks.h"
#include "game.h"
#include <vector>
#include <array>

/* This set up allows for faster move generation:
    - all possible moves are generated and stored in this
      structure (both legal and illegal moves)
    - then only legal moves are selected from this array
      and stored in the MoveGenerator vector 
*/
struct MoveArray
{
    std::array<int, 80> move_list;
    unsigned int count = 0;

    void add_move(int);
};

class MoveGenerator
{
    private:
        Game game; // Reference to a Game object

        // Additional state variables for copying
        U64 bitboards_copy[12], occupancies_copy[3];
        int side_copy, enpassant_copy, castle_copy;

        // Move generation
        void generate_white_pawns_moves(int ,U64);
        void generate_white_king_castling_moves(int , U64);
        void generate_black_pawns_moves(int ,U64);
        void generate_black_king_castling_moves(int , U64);
        void generate_knights_moves(int, U64, int);
        void generate_bishops_moves(int, U64, int);
        void generate_rooks_moves(int, U64, int);
        void generate_queens_moves(int, U64, int);
        void generate_kings_moves(int, U64, int);

        // Move encoding and decoding
        unsigned int encode_move(unsigned int,       // source square
                                unsigned int,       // target square
                                unsigned int,       // piece
                                unsigned int,       // promotion
                                unsigned int,       // capture
                                unsigned int,       // double pawn push
                                unsigned int,       // en passant
                                unsigned int);      // castling

        // Print functions
        void print_move_pretty(int);

    public:
        MoveArray possible_moves;
        MoveArray legal_moves;

        MoveGenerator() = delete;   // Cannot initialize MoveGenerator without a game
        MoveGenerator(Game& g) : game(g) { } // Default constructor

        // Move generation
        void generate_moves();
        MoveArray generate_captures();

        // Move logic
        int is_square_attacked(int, int);
        bool is_legal(int);

        // Print functions
        void print_move(int);
        void print_move_list();
        void print_attacked_squares(int);
};

#endif