#ifndef GAME_H
#define GAME_H

#include "general.h"
#include <sstream> // For std::istringstream
#include <algorithm> // For std::count
#include <cstring> // For memset() function

class Game {
    private:

        // array of pieces bitboards (black and white)
        U64 bitboards[12];

        // occupancy bitboards (for black, white and both)
        U64 occupancies[3];

        // side to move
        int side = -1;

        // enpassant squares
        int enpassant = no_sq;

        // castling rights
        int castle = 0;

        // This is the number of halfmoves (or moves by one player) since the last capture or pawn advance.
        // This is used for the fifty-move rule.
        int halfmoveClock;

        // Fullmove Counter: This counts the number of full moves in the game.
        // It starts at 1 and is incremented after Black's move.
        int fullmoveNumber;

    public:


        Game();     // Default constructor
        Game(std::string &);    // Constructior from a FEN
        Game(const Game &);     // Copy constructor


        // castling rights binary encoding

        /*

        bin    dec
            
        0001    1  white king can castle to the king side
        0010    2  white king can castle to the queen side
        0100    4  black king can castle to the king side
        1000    8  black king can castle to the queen side

        examples

        1111       both sides an castle both directions
        1001       black king => queen side
                   white king => king side

        */

        enum { wk = 1, wq = 2, bk = 4, bq = 8 };

        U64 get_bitboard(int);
        U64& get_bitboard_reference(int);
        U64* get_bitboards();
        U64 get_occupancy(int);
        U64& get_occupancy_reference(int);
        U64* get_occupancies();
        int get_side();
        int get_castle();
        int get_enpassant();
        void set_side(int);
        void set_castle(int);
        void set_enpassant(int);
        void set_bitboard(int, U64);
        void set_bitboards(U64 [12]);
        void set_occupancies(U64 [3]);
        void update_occupancy(int,U64);
        void reset_occupancies();
        void parse_fen(const std::string&);
        void print_board();

        bool is_valid(const std::string &);

        void make_move(int);

        // Helper functions for make_move (used also by MoveGenerator)
        int get_source_square(int);
        int get_target_square(int);
        int get_piece_moved(int);
        int get_promoted_piece(int);
        bool is_castling(int);
        bool is_en_passant(int);
        bool is_double_push(int);
        bool is_capture(int);
        bool is_promotion(int);

        Game copyBoardState();
        void take_back_to(Game);
};

#endif