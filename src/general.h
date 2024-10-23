#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>
#include <unordered_map>

typedef unsigned long long U64;

// ------ Useful constants ------
// Board constants
constexpr int RANKS = 8;
constexpr int FILES = 8;
constexpr int SQUARES = 64;
constexpr int COLORS = 2;
constexpr int PIECE_TYPES = 12;
constexpr int MAX_MOVES_IN_POSITION = 150; 

// Squares notation
enum {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

// Colors
enum { white, black, both };

// Piece_type
enum { king, queen, rook, bishop, knight, pawn };

// encode pieces
enum {P, N, B, R, Q, K, p, n, b, r, q, k};

// type of moves
enum { all_moves, only_captures};

extern const char *square_to_coordinates[64];
extern char ascii_pieces[13];
extern std::unordered_map<char, int> char_pieces;
extern std::unordered_map<int, char> int_to_char_pieces;
extern std::unordered_map<int, char> promoted_pieces;
extern std::string initial_position_fen;
extern const int castling_rights[64];

// Useful functions for manipulating bits in bitboards
U64 get_bit(U64, int);
void set_bit(U64 &, int);
void pop_bit(U64 &, int);

U64 FENtoBitboard(std::string);

int count_bits(U64);
int get_ls1b_index(U64);

// Print the bitboard
void print_bitboard(U64);

#endif