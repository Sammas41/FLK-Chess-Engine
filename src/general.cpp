#include "general.h"

// Returns the bit of a chosen square
U64 get_bit(U64 bitboard, int square)
{
	return bitboard & (1ULL << square);
}

// Sets the bit on the chosen square
U64 set_bit(U64 bitboard, int square)
{
	return bitboard | (1ULL << square);
}

// Pops the bit on the chosen square
U64 pop_bit(U64 bitboard, int square)
{
	return (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0);
}

// Counts the number of active bits in a bitboard
int count_bits(U64 bitboard)
{
	int count = 0;

	while(bitboard)
	{
		count++;
		bitboard &= (bitboard - 1);
	}

	return count;
}

// Returns the index of the 1st bit set in the bitboard
int get_ls1b_index(U64 bitboard)
{
	// If the bitboard is empty returns illegal index
	if(!bitboard) return -1;
	else
	{
		return count_bits((bitboard & -bitboard) - 1);
	}
}

// Prints the bitboard on screen
void print_bitboard(U64 bitboard)
{
	std::cout << "\n";
	for(int i = 0; i < RANKS; i++)
	{
		std::cout << " " << RANKS - i << "   ";
		for(int j = 0; j < FILES; j++)
		{
			std::cout << (get_bit(bitboard, i * RANKS + j) ? 1 : 0) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n     a b c d e f g h\n\n";
	std::cout << "     Decimal = " << bitboard << "\n\n";
}
