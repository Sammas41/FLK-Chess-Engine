#include "attacks.h"

// Precalculated attack tables for leaper pieces
U64 pawn_attacks[COLORS][SQUARES];
U64 knight_attacks[SQUARES];
U64 king_attacks[SQUARES];

U64 bishop_attacks[SQUARES][512];
U64 rook_attacks[SQUARES][4096];

U64 bishop_occupancy_bits[SQUARES]; 
U64 rook_occupancy_bits[SQUARES];

// Initialize attack tables
void init_precalc_attack_tables()
{
	int square = 0;
	U64 bitboard = 0ULL;
	
	for(int square = 0; square < SQUARES; square++)
	{
		bitboard = set_bit(bitboard, square);
			
		pawn_attacks[white][square] = generate_pawn_attacks(white, bitboard);
		pawn_attacks[black][square] = generate_pawn_attacks(black, bitboard);
			
		knight_attacks[square] = generate_knight_attacks(bitboard);

		king_attacks[square] = generate_king_attacks(bitboard);
		
		bishop_occupancy_bits[square] = generate_bishop_occupancy_bits(square);

		rook_occupancy_bits[square] = generate_rook_occupancy_bits(square);

		bitboard = pop_bit(bitboard, square);
	}

	for(int square = 0; square < SQUARES; square++)
	{
		int total_occupancies = 1 << bits_in_bishop_attacks[square];

		for(int index = 0; index < total_occupancies; index++)
		{
			U64 occupancy = set_occupancies(index, 
			                                bits_in_bishop_attacks[square], 
											bishop_occupancy_bits[square]);

			int magic_index = static_cast<int>(
							 (occupancy * bishop_magics[square]) >> (64 - bits_in_bishop_attacks[square]));

			bishop_attacks[square][magic_index] = generate_bishop_attacks_with_blockers(square, occupancy);
		}		
	}

	for(int square = 0; square < SQUARES; square++)
	{
		int total_occupancies = 1 << bits_in_rook_attacks[square];

		for(int index = 0; index < total_occupancies; index++)
		{
			U64 occupancy = set_occupancies(index, 
			                                bits_in_rook_attacks[square], 
											rook_occupancy_bits[square]);

			int magic_index = static_cast<int>(
							 (occupancy * rook_magics[square]) >> (64 - bits_in_rook_attacks[square]));

			rook_attacks[square][magic_index] = generate_rook_attacks_with_blockers(square, occupancy);
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

U64 get_bishop_attacks(int square, U64 occupancy)
{	
	occupancy &= bishop_occupancy_bits[square];

	int magic_index = static_cast<int>(
					  (occupancy * bishop_magics[square]) >> (64 - bits_in_bishop_attacks[square]));
	
	return bishop_attacks[square][magic_index];
}

U64 get_rook_attacks(int square, U64 occupancy)
{	
	occupancy &= rook_occupancy_bits[square];

	int magic_index = static_cast<int>(
					  (occupancy * rook_magics[square]) >> (64 - bits_in_rook_attacks[square]));
	
	return rook_attacks[square][magic_index];
}

// generate bishop relevant occupancy bits
U64 generate_bishop_occupancy_bits(int square){
	
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
U64 generate_rook_occupancy_bits(int square){
	
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

// Generates the attack range for a rook on a bitboard with blockers
U64 generate_rook_attacks_with_blockers(int square, U64 blockers) {
    U64 attacks = 0ULL;
    int r, f;
    int tr = square / 8; // target rank
    int tf = square % 8; // target file

    // up
    for (r = tr + 1; r <= 7; r++) {
		attacks |= (1ULL << (r * 8 + tf)); 
		if (blockers & (1ULL << (r * 8 + tf))) break;  // stop if blocker encountered
        // attacks |= (1ULL << (r * 8 + tf));   
    }

    // down
    for (r = tr - 1; r >= 0; r--) {
		attacks |= (1ULL << (r * 8 + tf)); 
        if (blockers & (1ULL << (r * 8 + tf))) break; 
		// attacks |= (1ULL << (r * 8 + tf));
    }

    // right
    for (f = tf + 1; f <= 7; f++) {
		attacks |= (1ULL << (tr * 8 + f)); 
		if (blockers & (1ULL << (tr * 8 + f))) break; 
        // attacks |= (1ULL << (tr * 8 + f));   
    }

    // left
    for (f = tf - 1; f >= 0; f--) {
		attacks |= (1ULL << (tr * 8 + f)); 
		if (blockers & (1ULL << (tr * 8 + f))) break; 
        // attacks |= (1ULL << (tr * 8 + f));      
    }

    return attacks;
}

// Generates the attack range for a bishop on a bitboard with blockers
U64 generate_bishop_attacks_with_blockers(int square, U64 blockers) {
	U64 attacks = 0ULL;
	int r, f;
	int tr = square / 8; // target rank
	int tf = square % 8; // target file

	// up-right
	for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
		attacks |= (1ULL << (r * 8 + f));
		if (blockers & (1ULL << (r * 8 + f))) break; // stop if blocker encountered
		// attacks |= (1ULL << (r * 8 + f));
	}

	// up-left
	for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
		attacks |= (1ULL << (r * 8 + f));
		if (blockers & (1ULL << (r * 8 + f))) break; // stop if blocker encountered
		// attacks |= (1ULL << (r * 8 + f));
	}

	// down-right
	for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
		attacks |= (1ULL << (r * 8 + f));
		if (blockers & (1ULL << (r * 8 + f))) break; // stop if blocker encountered
		// attacks |= (1ULL << (r * 8 + f));
	}

	// down-left
	for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
		attacks |= (1ULL << (r * 8 + f));
		if (blockers & (1ULL << (r * 8 + f))) break; // stop if blocker encountered
		// attacks |= (1ULL << (r * 8 + f));
	}

	return attacks;
}

// Returns a possible occupancy that a slider piece may find based on
// what index is passed to the function
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