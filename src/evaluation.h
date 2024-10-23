#ifndef EVAL_H
#define EVAL_H

#include "game.h"

namespace flk {

    extern U64 file_masks[64];
    extern U64 rank_masks[64];
    extern U64 isolated_masks[64];
    extern U64 black_passed_masks[64];
    extern U64 white_passed_masks[64];

    // Evaluation functions
    int evaluate(Game &);
    int lazy_evaluation(Game &);
    int eval_material(Game &);
    int eval_position(Game &);

    // Initialization
    void init_evaluation_masks();
    U64 set_file_rank_mask(int , int);
    
    const int piece_score[12] = {
        // white pieces:
        100,    // pawn
        300,    // knight
        350,    // bishop
        500,    // rook
        1000,    // queen
        100000, // king

        // black pieces:
        -100,
        -300,
        -350,
        -500,
        -1000,
        -100000
    };

    // pawn positional table
    const int pawn_positional_score[SQUARES] = {
        90,  90,  90,  90,  90,  90,  90,  90,
        30,  30,  30,  40,  40,  30,  30,  30,
        20,  20,  20,  30,  30,  30,  20,  20,
        10,  10,  10,  25,  25,  10,  10,  10,
        5,   5,   10,  25,  25,   5,   5,   5,
        0,   0,   0,    5,   5,   0,   0,   0,
        0,   0,   0,  -10, -10,   0,   0,   0,
        0,   0,   0,    0,   0,   0,   0,   0 };

    // knight positional table
    const int knight_positional_score[SQUARES] = {
        -5,   0,   0,   0,   0,   0,   0,   -5,
        -5,   0,   0,  10,  10,   0,   0,   -5,
        -5,   5,  20,  20,  20,  20,   5,   -5,
        -5,  10,  20,  30,  30,  20,  10,   -5,
        -5,  10,  20,  30,  30,  20,  10,   -5,
        -5,   5,  20,  10,  10,  20,   5,   -5,
        -5,   0,   0,   0,   0,   0,   0,   -5,
        -5, -10,   0,   0,   0,   0, -10,   -5 };

    // bishop positional table
    const int bishop_positional_score[SQUARES] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,  10,  10,   0,   0,   0,
        0,   0,  10,  20,  20,  10,   0,   0,
        0,   0,  10,  20,  20,  10,   0,   0,
        0,  10,   0,   0,   0,   0,  10,   0,
        0,  30,   0,   0,   0,   0,  30,   0,
        0,   0, -10,   0,   0, -10,   0,   0 };

    // rook positional table
    const int rook_positional_score[SQUARES] = {
        50, 50,  50,  50,  50,  50,  50,  50,
        50, 50,  50,  50,  50,  50,  50,  50,
         0,  0,  10,  20,  20,  10,   0,   0,
         0,  0,  10,  20,  20,  10,   0,   0,
         0,  0,  10,  20,  20,  10,   0,   0,
         0,  0,  10,  20,  20,  10,   0,   0,
         0,  0,  10,  20,  20,  10,   0,   0,
         0,  0,   0,  20,  20,   0,   0,   0 };
 
    // king positional table
    const int king_positional_score[SQUARES] = {
        0,  0,   0,   0,   0,   0,  0,  0,
        0,  0,   5,   5,   5,   5,  0,  0,
        0,  5,   5,  10,  10,   5,  5,  0,
        0,  5,  10,  20,  20,  10,  5,  0,
        0,  5,  10,  20,  20,  10,  5,  0,
        0,  0,   5,  10,  10,   5,  0,  0,
        0,  5,   5,  -5,  -5,   0,  5,  0,
        0,  0,   5,   0, -15,   0,  10, 0 };

    // mirror squares for black pieces
    const int mirror_squares[SQUARES] = {
        a1, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8
    };

    // **************** improve evaluation ****************
    const int get_rank[64] = {
        7, 7, 7, 7, 7, 7, 7, 7,
        6, 6, 6, 6, 6, 6, 6, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0
    };

    const int double_pawn_penalty = -10;
    const int isolated_pawn_penalty = -10;
    const int passed_pawn_bonus[8] = {0,5,10,20,35,60,100,200};
    const int semi_open_file_score = 10;
    const int open_file_score = 15;
    const int king_shield_bonus = 5;
};

#endif