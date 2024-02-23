#include "game.h"
#include "general.h"

#include <sstream>


Game::Game(){
    initialize_pieces_bitboards(initial_position_fen,bitboards);
    std::cout << "Game constructed\n";
}

U64 Game::get_bitboard(int index)
{	
	return bitboards[index];
}

void Game::set_bitboard(int index, U64 value) {
    if (index >= 0 && index < 12) { // Assuming you have 12 bitboards
        bitboards[index] = value;
    } else {
        // Handle error: index out of range
        std::cerr << "Index out of range in set_bitboard function." << std::endl;
    }
}

void Game::initialize_pieces_bitboards(const std::string& fen, U64 bitboards[12]) {
    int square = 0;

    // Initialize all bitboards to 0
    for (int i = 0; i < 12; ++i) {
        bitboards[i] = 0;
    }

    // reset game state variables
    side = 0;

    enpassant = no_sq;

    castle = 0;

    std::istringstream fenStream(fen);
    std::string board, activeColor, castlingRights, enPassant, halfmove, fullmove;

    // Parse FEN string
    fenStream >> board >> activeColor >> castlingRights >> enPassant >> halfmove >> fullmove;

	for (char c : fen) {
	if (c == '/') {
		continue; // Skip to the next row
	}

	if (isdigit(c)) {
		square += c - '0'; // Skip empty squares
	} else {
		// Find the index corresponding to the piece type
		int pieceIndex;
		switch (c) {
			case 'P': pieceIndex = P; break;
			case 'N': pieceIndex = N; break;
			case 'B': pieceIndex = B; break;
			case 'R': pieceIndex = R; break;
			case 'Q': pieceIndex = Q; break;
			case 'K': pieceIndex = K; break;
			case 'p': pieceIndex = p; break;
			case 'n': pieceIndex = n; break;
			case 'b': pieceIndex = b; break;
			case 'r': pieceIndex = r; break;
			case 'q': pieceIndex = q; break;
			case 'k': pieceIndex = k; break;
			default: pieceIndex = -1; break;
		}

		if (pieceIndex != -1) {
			// Set the bit corresponding to the square
			bitboards[pieceIndex] |= 1ULL << square;
		}
		square++;
	}
	}

    // Parse active color
    side = (activeColor == "w") ? white : black;

    // Parse castling rights
    for (char c : castlingRights) {
        switch (c) {
            case 'K': castle |= wk; break;
            case 'Q': castle |= wq; break;
            case 'k': castle |= bk; break;
            case 'q': castle |= bq; break;
        }
    }

    // Parse en passant target square
    if (enPassant != "-") {
        int file = enPassant[0] - 'a'; // File index (0-7)
        int rank = 8 - (enPassant[1] - '0'); // Rank index (0-7), inverted
        enpassant = rank * 8 + file;
    } else {
        enpassant = no_sq;
    }

    // Parse halfmove clock and fullmove number
    // Assuming these are integers and stored in your class
    halfmoveClock = std::stoi(halfmove);
    fullmoveNumber = std::stoi(fullmove);
}



// print board
void Game::print_board()
{
    // print offset
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop ober board files
        for (int file = 0; file < 8; file++)
        {
            // init square
            int square = rank * 8 + file;
            
            // print ranks
            if (!file)
                printf("  %d ", 8 - rank);
            
            // define piece variable
            int piece = -1;
            
            // loop over all piece bitboards
            for (int bb_piece = P; bb_piece <= k; bb_piece++)
            {
                if (get_bit(bitboards[bb_piece], square))
                    piece = bb_piece;
            }
            
            // print piece set 
            printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
            
        }
        
        // print new line every rank
        printf("\n");
    }
    
    // print board files
    printf("\n     a b c d e f g h\n\n");
    
    // print side to move (white = 0, black = 1)
    printf("     Side:     %s\n", !side ? "white" : "black");
    
    // print enpassant square
    printf("     Enpassant:   %s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "no");
    
    // print castling rights
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) ? 'K' : '-',
                                           (castle & wq) ? 'Q' : '-',
                                           (castle & bk) ? 'k' : '-',
                                           (castle & bq) ? 'q' : '-');

    std::cout << "     Halfmove: " << halfmoveClock << std::endl;
    std::cout << "     Fullmove: " << fullmoveNumber << std::endl;                                    
}
