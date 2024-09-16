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
        int score_move(Move);
        void sort_moves(MoveArray&);

        // Print functions
        void print_move_list(MoveArray);
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

        // This table is used to sort the list of captures such
        // that it searches first the most favourable captures
        // (ie a pawn capturing a piece). In this way it is more
        // likely that alpha-beta pruning cuts off much of the
        // searching tree. The first index represents the attacking
        // piece while the second index represents the piece targeted.
        //
        // Example:
        //  - pawn takes queen: MVV_LVA[P][q] = 505 --> good capture,
        //    high probability to prune other branches, should be searched
        //    first
        //  - queen takes pawn: MVV_LVA[Q][p] = 101 --> bad capture,
        //    low probability to prune other branches, should be searched last                                       
        const int MVV_LVA[12][12] = {
            {105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605},
            {104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604},
            {103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603},
            {102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602},
            {101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601},
            {100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600},

            {105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605},
            {104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604},
            {103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603},
            {102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602},
            {101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601},
            {100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600}
        };
};

#endif