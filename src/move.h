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
        Move() { move = 0; }

        int get_source_square();
        int get_target_square();
        int get_piece_moved();
        int get_promoted_piece();
        bool is_castling();
        bool is_en_passant();
        bool is_double_push();
        bool is_capture();

        void print_move(bool all_info = false);

        // Overload of the == operator
        bool operator ==(const Move& another_move) {
            if(another_move.move == move)
                return true;
            else return false;
        }

        // Overload of the != operator
        bool operator !=(const Move& another_move) {
            if(another_move.move != move)
                return true;
            else return false;
        }
    private:
        int move;

        int encode_move(int, int, int, int, int, int, int, int);
};

const Move NULL_MOVE(0, 0, 0, 0, 0, 0, 0, 0);

#endif