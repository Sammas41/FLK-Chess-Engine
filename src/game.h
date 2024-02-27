#ifndef GAME_H
#define GAME_H

#include "general.h"
#include <sstream> // For std::istringstream
#include <algorithm> // For std::count

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

        int activeColor;

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
        U64 get_occupancy(int);
        int get_side();
        int get_castle();
        int get_enpassant();
        void set_side(int);
        void set_bitboard(int, U64);
        void initialize_pieces_bitboards(const std::string&);
        void print_board();

        bool is_valid(const std::string &);

};

#endif