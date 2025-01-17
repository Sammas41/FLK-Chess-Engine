#include "attacks.h"

namespace flk {

	// Attack tables
    U64 pawn_attacks[COLORS][SQUARES];
    U64 knight_attacks[SQUARES];
    U64 king_attacks[SQUARES];
    U64 bishop_attacks[SQUARES][512];
    U64 rook_attacks[SQUARES][4096];
    
    // Masks
    U64 bishop_mask[SQUARES];
    U64 rook_mask[SQUARES];

	void init_all_attacks()
	{
		init_leaper_pieces_attacks();
		init_slider_pieces_attacks();
	}

	// Initialize leaper pieces attack tables (pawns, knights and kings)
	void init_leaper_pieces_attacks()
	{
		U64 bitboard = 0ULL;
		
		for(int square = 0; square < SQUARES; square++)
		{		
			set_bit(bitboard, square);

			pawn_attacks[white][square] = generate_pawn_attacks(white, bitboard);
			pawn_attacks[black][square] = generate_pawn_attacks(black, bitboard);
				
			knight_attacks[square] = generate_knight_attacks(bitboard);

			king_attacks[square] = generate_king_attacks(bitboard);

			pop_bit(bitboard, square);
		}
	}

	// Initialize slider pieces attack tables (bishops, rooks, queens)
	void init_slider_pieces_attacks()
	{
		// Initialize bishop attacks 
		for(int square = 0; square < SQUARES; square++)
		{
			bishop_mask[square] = generate_bishop_mask(square);

			int total_occupancies = (1 << bits_in_bishop_mask[square]);

			for(int index = 0; index < total_occupancies; index++)
			{
				U64 occupancy = generate_occupancy(index, bits_in_bishop_mask[square], bishop_mask[square]);

				int magic_index = compute_magic_index(square, occupancy, bishop);

				bishop_attacks[square][magic_index] = generate_bishop_attacks_with_blockers(square, occupancy);
			}
		}

		// Initialize rook attacks 
		for(int square = 0; square < SQUARES; square++)
		{
			rook_mask[square] = generate_rook_mask(square);

			int total_occupancies = (1 << bits_in_rook_mask[square]);

			for(int index = 0; index < total_occupancies; index++)
			{
				U64 occupancy = generate_occupancy(index, bits_in_rook_mask[square], rook_mask[square]);

				int magic_index = compute_magic_index(square, occupancy, rook);

				rook_attacks[square][magic_index] = generate_rook_attacks_with_blockers(square, occupancy);
			}
		}
	}

	// Retrieve pawns attacks
	U64 get_pawn_attack(int color, int square)
	{
		if(color != white && color != black)
		{
			std::cout << "[ERROR] In: get_pawn_attack, Error: color must be white or black\n";
			return 0ULL;
		}

		if(square < 0 || square > 63)
		{
			std::cout << "[ERROR] In: Attacks::get_pawn_attack, Error: square must be between 0 and 63\n";
			return 0ULL;
		}

		if(color == white) return flk::pawn_attacks[white][square];
		else return flk::pawn_attacks[black][square];
		}

		// Retrieve knights attacks
	U64 get_knight_attack(int square)
	{
		if(square < 0 || square > 63)
		{
			std::cout << "[ERROR] In: get_knight_attack, Error: square must be between 0 and 63\n";
			return 0ULL;
		}

		return flk::knight_attacks[square];
	}

	// Retrieve kings attacks
	U64 get_king_attack(int square)
	{
		if(square < 0 || square > 63)
		{
			std::cout << "[ERROR] In: get_king_attack, Error: square must be between 0 and 63\n";
			return 0ULL;
		}

		return flk::king_attacks[square];
	}

	// Retrieve bishops attacks
	U64 get_bishop_attack(int square, U64 occupancy)
	{
		if(square < 0 || square > 63)
		{
			std::cout << "[ERROR] In: get_bishop_attack, Error: square must be between 0 and 63\n";
			return 0ULL;
		}

		U64 bishop_occupancy = occupancy & flk::bishop_mask[square];
		int magic_index = flk::compute_magic_index(square, bishop_occupancy, bishop);

		return flk::bishop_attacks[square][magic_index];
	}

	// Retrieve rooks attacks
	U64 get_rook_attack(int square, U64 occupancy)
	{
		if(square < 0 || square > 63)
		{
			std::cout << "[ERROR] In: get_rook_attack, Error: square must be between 0 and 63\n";
			return 0ULL;
		}

		U64 rook_occupancy = occupancy & flk::rook_mask[square];
		int magic_index = flk::compute_magic_index(square, rook_occupancy, rook);

		return flk::rook_attacks[square][magic_index];
	}

	// Retrieve queens attacks
	U64 get_queen_attack(int square, U64 occupancy){

		if(square < 0 || square > 63)
		{
			std::cout << "[ERROR] In: get_queen_attack, Error: square must be between 0 and 63\n";
			return 0ULL;
		}

		U64 result = get_bishop_attack(square, occupancy);
		result |= get_rook_attack(square, occupancy);

		return result;
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
	U64 generate_bishop_mask(int square){
		
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
	U64 generate_rook_mask(int square){
		
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
		}

		// down
		for (r = tr - 1; r >= 0; r--) {
			attacks |= (1ULL << (r * 8 + tf)); 
			if (blockers & (1ULL << (r * 8 + tf))) break; 
		}

		// right
		for (f = tf + 1; f <= 7; f++) {
			attacks |= (1ULL << (tr * 8 + f)); 
			if (blockers & (1ULL << (tr * 8 + f))) break;   
		}

		// left
		for (f = tf - 1; f >= 0; f--) {
			attacks |= (1ULL << (tr * 8 + f)); 
			if (blockers & (1ULL << (tr * 8 + f))) break;      
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
		}

		// up-left
		for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
			attacks |= (1ULL << (r * 8 + f));
			if (blockers & (1ULL << (r * 8 + f))) break; // stop if blocker encountered
		}

		// down-right
		for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
			attacks |= (1ULL << (r * 8 + f));
			if (blockers & (1ULL << (r * 8 + f))) break; // stop if blocker encountered
		}

		// down-left
		for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
			attacks |= (1ULL << (r * 8 + f));
			if (blockers & (1ULL << (r * 8 + f))) break; // stop if blocker encountered
		}

		return attacks;
	}

	// Returns a possible occupancy that a slider piece may find based on
	// what index is passed to the function
	U64 generate_occupancy(int index, int bit_count, U64 mask)
	{
		U64 occupancy = 0ULL;

		for(int count = 0; count < bit_count; count++)
		{
			int square = get_ls1b_index(mask);

			pop_bit(mask, square);

			if(index & (1 << count))
				occupancy |= (1ULL << square);
		}

		return occupancy;
	}

	// Returns the magic index for a given square and occupancy for rooks and bishops
	int compute_magic_index(int square, U64 occupancy, int piece_type)
	{
		int magic_index = 0;

		if(piece_type == bishop)
		{
			magic_index = static_cast<int>((occupancy * bishop_magics[square]) 
										>> (64 - bits_in_bishop_mask[square]));

			return magic_index;	
		}
		else
		{
			magic_index = static_cast<int>((occupancy * rook_magics[square]) 
										>> (64 - bits_in_rook_mask[square]));

			return magic_index;
		}
	}
}	// End namespace flk
