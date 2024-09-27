#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "attacks.h"
#include "game.h"
#include <array>

/* This set up allows for faster move generation:
    - all possible moves are generated and stored in this
      structure (both legal and illegal moves)
    - then only legal moves are selected from this array
      and stored in the MoveGenerator vector 
*/
struct MoveArray
{
    std::array<Move, 80> move_list;
    unsigned int count = 0;

    void add_move(Move);
};

class MoveGenerator
{
    public:
        MoveGenerator() = delete;   // Cannot initialize MoveGenerator without a game
        MoveGenerator(Game& g) : game(g) { } // Default constructor

        // Move generation
        MoveArray generate_moves(int);

        // Move logic
        int is_square_attacked(int, int);
        bool is_legal(Move);

        // Print functions
        void print_move_list(MoveArray&);
        void print_attacked_squares(int);

    private:
        Game game; // Reference to a Game object
        MoveArray possible_moves;
        
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

        // Print functions
        void print_move_pretty(Move);
};

#endif