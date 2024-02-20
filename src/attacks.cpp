#include "attacks.h"

// Precalculated attack tables for leaper pieces
U64 pawn_attacks[COLORS][SQUARES];
U64 knight_attacks[SQUARES];
U64 king_attacks[SQUARES];
U64 bishop_attacks[SQUARES]; 
U64 rook_attacks[SQUARES];

// Initialize attack tables
void init_precalc_attack_tables()
{
	int square = 0;
	U64 bitboard = 0ULL;
	
	for(int i = 0; i < RANKS; i++)
	{
		for(int j = 0; j < FILES; j++)
		{
			square = i * RANKS + j;
			bitboard = set_bit(bitboard, square);
			
			pawn_attacks[white][square] = generate_pawn_attacks(white, bitboard);
			pawn_attacks[black][square] = generate_pawn_attacks(black, bitboard);
			
			knight_attacks[square] = generate_knight_attacks(bitboard);

			king_attacks[square] = generate_king_attacks(bitboard);

			bishop_attacks[square] = generate_bishop_attacks(square);

			rook_attacks[square] = generate_rook_attacks(square);

			bitboard = pop_bit(bitboard, square);
		}
	}
}

// Generates the attack table for a pawn on a given square, for both colors
U64 generate_pawn_attacks(int color, U64 bitboard)
{	
	U64 attack = 0ULL;
	
	if(color == white)
	{
		// Checks if the pawn is not on the edge of the board
		attack |= ( (bitboard & NOT_H_FILE) >> 7);
		attack |= ( (bitboard & NOT_A_FILE) >> 9);
	}
	else
	{
		// Checks if the pawn is not on the edge of the board
		attack |= ( (bitboard & NOT_A_FILE) << 7);
		attack |= ( (bitboard & NOT_H_FILE) << 9);
	}
	
	return attack;
}

// Generates the attack table for a knight on a given square
U64 generate_knight_attacks(U64 bitboard)
{
	U64 attack = 0ULL;

	// Checks if the knight is not on the edge of the board or on the G and B files
	attack |= ( (bitboard & NOT_H_FILE) >> 15);
	attack |= ( (bitboard & NOT_A_FILE) >> 17);
	attack |= ( (bitboard & NOT_GH_FILE) >> 6);
	attack |= ( (bitboard & NOT_AB_FILE) >> 10);

	attack |= ( (bitboard & NOT_A_FILE) << 15);
	attack |= ( (bitboard & NOT_H_FILE) << 17);
	attack |= ( (bitboard & NOT_AB_FILE) << 6);
	attack |= ( (bitboard & NOT_GH_FILE) << 10);

	return attack;
}

// Generates the attack table for a king on a given square
U64 generate_king_attacks(U64 bitboard)
{
	U64 attack = 0ULL;

	attack |= (bitboard  >> 8);
	attack |= (bitboard << 8);

	// Checks if the king is not on the edge of the board
	attack |= ( (bitboard & NOT_H_FILE) >> 7);
	attack |= ( (bitboard & NOT_A_FILE) >> 1);
	attack |= ( (bitboard & NOT_A_FILE) >> 9);
	
	attack |= ( (bitboard & NOT_A_FILE) << 7);
	attack |= ( (bitboard & NOT_H_FILE) << 1);
	attack |= ( (bitboard & NOT_H_FILE) << 9);

	return attack;
}

// generate bishop relevant occupancy bits
U64 generate_bishop_attacks(int square)
{
	U64 attack = 0ULL;

	// init ranks and files
	int r, f;

	// init target rank and files
	int tr = square / 8;
	int tf = square % 8;

	for (r = tr + 1, f= tf + 1; r <= 6 && f <= 6; r++, f++) attack |= (1ULL << (r * 8 + f));
	for (r = tr - 1, f= tf + 1; r >= 1 && f <= 6; r--, f++) attack |= (1ULL << (r * 8 + f));
	for (r = tr + 1, f= tf - 1; r <= 6  && f >= 1; r++, f--) attack |= (1ULL << (r * 8 + f));
	for (r = tr - 1, f= tf - 1; r >= 1 && f >= 1; r--, f--) attack |= (1ULL << (r * 8 + f));


	return attack;
}

// generate rook relevant occupancy bits
U64 generate_rook_attacks(int square)
{
	U64 attack = 0ULL;

	// init ranks and files
	int r, f;

	// init target rank and files
	int tr = square / 8;
	int tf = square % 8;

	for( r = tr + 1; r <= 6; r++) attack |= (1ULL << (r * 8 + tf));
	for( r = tr - 1; r >= 1; r--) attack |= (1ULL << (r * 8 + tf));
	for( f = tf + 1; f <= 6; f++) attack |= (1ULL << (tr * 8 + f));
	for( f = tf - 1; f >= 1; f--) attack |= (1ULL << (tr * 8 + f));

	return attack;
}

U64 set_occupancies(int index, int bit_count, U64 mask)
{
	U64 occupancy = 0ULL;

	for(int count = 0; count < bit_count; count++)
	{
		int square = get_ls1b_index(mask);

		mask = pop_bit(mask, square);

		if(index & (1 << count))
			occupancy |= (1ULL << square);
	}

	return occupancy;
}