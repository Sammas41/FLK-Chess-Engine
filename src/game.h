#ifndef GAME_H
#define GAME_H

#include "general.h"
#include "attacks.h"
#include <unordered_map>

class Game{
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

    public:

        Game();

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

        // encode pieces
        enum {P, N, B, R, Q, K, p, n, b, r, q, k};

        // convert squares to coordinates
        const char *square_to_coordinates[64] = {
            "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
            "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
            "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
            "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
            "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
            "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
            "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        };

        // ASCII pieces
        char ascii_pieces[13] = "PNBRQKpnbrqk";

        // convert ASCII character pieces to encoded constants
        std::unordered_map<char, int> char_pieces = {
            {'P', P},
            {'N', N},
            {'B', B},
            {'R', R},
            {'Q', Q},
            {'K', K},
            {'p', p},
            {'n', n},
            {'b', b},
            {'r', r},
            {'q', q},
            {'k', k}
        };

        U64 get_bitboard(int);
        void set_bitboard(int, U64);
        void print_board();

};

#endif