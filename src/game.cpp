#include "game.h"
#include "general.h"

#include <sstream>


Game::Game(){
    initialize_pieces_bitboards(initial_position_fen);
}

// getters and setters
U64 Game::get_bitboard(int index)
{	
	return bitboards[index];
}

U64 Game::get_occupancy(int index)
{
    return occupancies[index];
}

int Game::get_side(){
    return side;
}

void Game::set_side(int side_to_move){
    side = side_to_move;
}

void Game::set_bitboard(int index, U64 value) {
    if (index >= 0 && index < 12) { // Assuming you have 12 bitboards
        bitboards[index] = value;
    } else {
        // Handle error: index out of range
        std::cerr << "Index out of range in set_bitboard function." << std::endl;
    }
}

void Game::initialize_pieces_bitboards(const std::string& fen) {
    
    if(!is_valid(fen))
    {
        std::cerr << "Invalid FEN. Insert a valid FEN\n";
        return;
    }
    
    int square = 0;

    // Initialize all bitboards to 0
    for (int i = 0; i < 12; ++i) {
        bitboards[i] = 0ULL;
        if(i < 3) occupancies[i] = 0ULL;
    }

    // reset game state variables
    side = 0;

    enpassant = no_sq;

    castle = 0;

    std::istringstream fenStream(fen);
    std::string board, activeColor, castlingRights, enPassant, halfmove, fullmove;

    // Parse FEN string
    fenStream >> board >> activeColor >> castlingRights >> enPassant >> halfmove >> fullmove;

	for (char c : board) {
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
        // obtain index subtracting ascii values of strings a and 0
        int file = enPassant[0] - 'a'; // File index (0-7)
        int rank = 8 - (enPassant[1] - '0'); // Rank index (0-7), inverted
        enpassant = rank * 8 + file;
    } else {
        enpassant = no_sq;
    }

    // Parse halfmove clock and fullmove number
    halfmoveClock = std::stoi(halfmove);
    fullmoveNumber = std::stoi(fullmove);
    
    // Initialize white pieces bitboard
    for(int piece = P; piece <= K; piece++)
    {
        occupancies[white] |= bitboards[piece];
    }

    // Initialize black pieces bitboard
    for(int piece = p; piece <= k; piece++)
    {
        occupancies[black] |= bitboards[piece];
    }

    // Initialize all pieces bitboard
    occupancies[both] = occupancies[white] | occupancies[black];

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


bool Game::is_valid(const std::string & fen)
{
    std::istringstream iss(fen);
    std::string fen_board, fen_side, fen_enpassant, fen_castling;

    // Checks that the board is not empty
    if(!iss) return false;
    iss >> fen_board;

    // Checks that there is one side to move
    if(!iss) return false;
    iss >> fen_side;

    // Checks that there are castling rights
    if (!iss) return false;
    iss >> fen_castling;

    // Checks that there is en passant square
    if(iss) iss >> fen_enpassant;
    else return false;

    // Checks that the side is valid
    if (fen_side != "w" && fen_side != "b") return false;

    // Checks that there are eight ranks
    if(std::count(fen_board.begin(), fen_board.end(), '/') != 7) return false;

    // Checks that there are two kings
    if(std::count(fen_board.begin(), fen_board.end(), 'k') != 1) return false;
    if(std::count(fen_board.begin(), fen_board.end(), 'K') != 1) return false;

    // Checks if every square is in a definite state
    int cont = 0;
    for(char c : fen_board)
    {
        // If every square is in a definite state then when we
        // reach the end of the rank, cont must be equal to 8
        if(c == '/')
        {
            // Invalid FEN
            if (cont != 8) return false;
            else
            {
                // This rank is ok, start again
                cont = 0;
                continue;
            }
        }

        if(isdigit(c)) cont += (c - '0');  // If c is a number, add the empty squares
        else cont++;  // If c is a piece, just add 1
    }

    // Check on castling rights
    if (fen_castling != "-" && fen_castling != "K" && fen_castling != "Kk"
       && fen_castling != "Kkq" && fen_castling != "Kq" && fen_castling !="KQ"
       && fen_castling != "KQk" && fen_castling != "KQq" && fen_castling != "KQkq"
       && fen_castling != "k" && fen_castling != "q" && fen_castling != "kq"
       && fen_castling != "Q" && fen_castling != "Qk" && fen_castling != "Qq"
       && fen_castling != "Qkq")
      return false;

    // Check on en passant square
    if(fen_enpassant != "-")
    {
        // Checks length
        if(fen_enpassant.length() != 2) return false;
            
        // Checks valid file format
        if (!(fen_enpassant[0] >= 'a' && fen_enpassant[0] <= 'h')) return false;
            
        // Checks valid rank format
        if (!((fen_side == "w" && fen_enpassant[1] == '6') || 
              (fen_side == "b" && fen_enpassant[1] == '3'))) return false;
    }

    // Hopefully the FEN is ok
    return true;
}
