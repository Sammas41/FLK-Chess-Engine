#ifndef ATTACKS_H
#define ATTACKS_H

#include "general.h"

extern U64 pawn_attacks[COLORS][SQUARES];
extern U64 knight_attacks[SQUARES];
extern U64 king_attacks[SQUARES];
extern U64 bishop_occupancy_bits[SQUARES];
extern U64 rook_occupancy_bits[SQUARES];


// Initialize precalculated attack tables
void init_precalc_attack_tables();

// Computes the attack tables for a specific piece on a specific square
U64 generate_pawn_attacks(int, U64);
U64 generate_knight_attacks(U64);
U64 generate_king_attacks(U64);
U64 generate_bishop_occupancy_bits(int);
U64 generate_rook_occupancy_bits(int);
U64 generate_bishop_attacks_with_blockers(int, U64); 
U64 generate_rook_attacks_with_blockers(int, U64);
#endif
