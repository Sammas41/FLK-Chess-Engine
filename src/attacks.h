#ifndef ATTACKS_H
#define ATTACKS_H

#include "general.h"
#include <fstream>
#include <string>

extern U64 pawn_attacks[COLORS][SQUARES];
extern U64 knight_attacks[SQUARES];
extern U64 king_attacks[SQUARES];
extern U64 bishop_attacks[SQUARES];


// Initialize precalculated attack tables
void init_precalc_attack_tables();

// Computes the attack tables for a specific piece on a specific square
U64 generate_pawn_attacks(int, U64);
U64 generate_knight_attacks(U64);
U64 generate_king_attacks(U64);
U64 generate_bishop_attacks(int);

#endif
