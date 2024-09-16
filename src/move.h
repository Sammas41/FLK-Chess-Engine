#ifndef MOVE_H
#define MOVE_H

#include "general.h"

class Move {
    public:
        Move(int,       // source square
            int,       // target square
            int,       // piece
            int,       // promotion
            int,       // capture
            int,       // double pawn push
            int,       // en passant
            int);      // castling
        Move(std::string, int);
        Move() { }

        int get_source_square();
        int get_target_square();
        int get_piece_moved();
        int get_promoted_piece();
        bool is_castling();
        bool is_en_passant();
        bool is_double_push();
        bool is_capture();

        void print_move(bool all_info = false);

    private:
        int move;

        int encode_move(int, int, int, int, int, int, int, int);
};

#endif