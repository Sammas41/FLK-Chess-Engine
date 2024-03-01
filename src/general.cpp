#include "general.h"

// initial position FEN
std::string initial_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// convert squares to coordinates
const char *square_to_coordinates[64] = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

// ASCII pieces
char ascii_pieces[13] = "PNBRQKpnbrqk";

// convert ASCII character pieces to encoded constants
std::unordered_map<char, int> char_pieces = {
	{'P', P},
	{'N', N},
	{'B', B},
	{'R', R},
	{'Q', Q},
	{'K', K},
	{'p', p},
	{'n', n},
	{'b', b},
	{'r', r},
	{'q', q},
	{'k', k}
};

std::unordered_map<int, char> promoted_pieces = {
    {Q, 'q'},
    {R, 'r'},
    {B, 'b'},
    {N, 'n'},
    {q, 'q'},
    {r, 'r'},
    {b, 'b'},
    {n, 'n'}
};

/*
							  castling  move     in      in
							   right  update     binary  decimal

 king & rooks didn't move:     1111 & 1111  =  1111    15

        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13
     
         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7

*/

// castling rights update constants
const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

// Returns the bit of a chosen square
U64 get_bit(U64 bitboard, int square)
{
	return bitboard & (1ULL << square);
}

// Sets the bit on the chosen square
void set_bit(U64 & bitboard, int square)
{
	bitboard |= (1ULL << square);
}

// Pops the bit on the chosen square
void pop_bit(U64 &bb, int square) {
    bb &= ~(1ULL << square);
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


// function to convert a FEN string (only the board representation part) to a bitboard
// with ones on the pieces location
U64 FENtoBitboard(std::string fen) {
    U64 bitboard = 0;
    int square = 0; 

    for (char c : fen) {
        
        if (c == '/') {
            continue; // Skip to the next row
        }

        if (isdigit(c)) {
            square += c - '0'; // Skip empty squares
        } else {
            bitboard |= 1ULL << (square);
            square++;
        }
    }

    return bitboard;
}
