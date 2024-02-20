#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>

typedef unsigned long long U64;

// Useful constants
constexpr int RANKS = 8;
constexpr int FILES = 8;
constexpr int SQUARES = 64;
constexpr int COLORS = 2;

constexpr U64 NOT_A_FILE = 18374403900871474942ULL;
constexpr U64 NOT_H_FILE = 9187201950435737471ULL;
constexpr U64 NOT_GH_FILE = 4557430888798830399ULL;
constexpr U64 NOT_AB_FILE = 18229723555195321596ULL;

constexpr int bits_in_bishop_attacks[SQUARES] = {	
	6, 5, 5, 5, 5, 5, 5, 6, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 7, 7, 7, 7, 5, 5, 
	5, 5, 7, 9, 9, 7, 5, 5, 
	5, 5, 7, 9, 9, 7, 5, 5, 
	5, 5, 7, 7, 7, 7, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	6, 5, 5, 5, 5, 5, 5, 6  
};

constexpr int bits_in_rook_attacks[SQUARES] = {
	12, 11, 11, 11, 11, 11, 11, 12, 
	11, 10, 10, 10, 10, 10, 10, 11, 
	11, 10, 10, 10, 10, 10, 10, 11, 
	11, 10, 10, 10, 10, 10, 10, 11, 
	11, 10, 10, 10, 10, 10, 10, 11, 
	11, 10, 10, 10, 10, 10, 10, 11, 
	11, 10, 10, 10, 10, 10, 10, 11, 
	12, 11, 11, 11, 11, 11, 11, 12 
};

// Squares notation
enum {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1
};

// Colors
enum { white, black };

// Useful functions for manipulating bits in bitboards
U64 get_bit(U64, int);
U64 set_bit(U64, int);
U64 pop_bit(U64, int);

U64 FENtoBitboard(std::string);

int count_bits(U64);
int get_ls1b_index(U64);

// Print the bitboard
void print_bitboard(U64);

#endif