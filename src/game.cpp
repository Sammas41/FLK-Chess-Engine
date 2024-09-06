#include "game.h"

Game::Game()
{
    parse_fen(initial_position_fen);
}

Game::Game(std::string& fen)
{
    if(is_valid(fen)) parse_fen(fen);
    else std::cerr << "Invalid FEN, cannot build Game object\n";
}

Game::Game(const Game & g)
{
    for(int i = 0; i < 12; i++)
    {
        bitboards[i] = g.bitboards[i];
        if(i < 3) occupancies[i] = g.occupancies[i];
    }

    side = g.side;
    castle = g.castle;
    enpassant = g.enpassant;
    halfmoveClock = g.halfmoveClock;
    fullmoveNumber = g.fullmoveNumber;
}

// getters and setters
U64 Game::get_bitboard(int index)
{	
	return bitboards[index];
}

U64& Game::get_bitboard_reference(int index)
{	
	return bitboards[index];
}

U64* Game::get_bitboards(){
    return bitboards;
}

U64 Game::get_occupancy(int index)
{
    return occupancies[index];
}

U64& Game::get_occupancy_reference(int index){
    return occupancies[index];
}

U64* Game::get_occupancies()
{
    return occupancies;
}

int Game::get_side(){
    return side;
}

int Game::get_castle(){
    return castle;
}

int Game::get_enpassant(){
    return enpassant;
}

void Game::set_side(int side_to_move){
    side = side_to_move;
}

void Game::set_castle(int setcastle){
    castle = setcastle;
}

void Game::set_enpassant(int enpassant_square){
    enpassant = enpassant_square;
}

void Game::set_bitboards(U64 new_bitboards[12]){
    memcpy(bitboards, new_bitboards, 96);//96=sizeof(bitboards)
}

void Game::set_occupancies(U64 new_occupancies[3]){
    memcpy(occupancies, new_occupancies, 24);//24=sizeof(occupancies)
}

void Game::update_occupancy(int side,U64 occupancy){
    occupancies[side] |= occupancy;
}

void Game::reset_occupancies(){
    memset(occupancies, 0ULL, 24);//24=sizeof(occupancies)
}

void Game::set_bitboard(int index, U64 value) {
    // Update bitboards, assuming you have 12 bitboards
    if (index >= 0 && index < 12) bitboards[index] = value;
    else std::cerr << "Index out of range in set_bitboard function." << std::endl;
    
    // Update occupancies
    for(int i = 0; i < 3; i++) occupancies[i] = 0ULL;

    // White occupancy
    for(int piece = P; piece <= K; piece++)
    {
        occupancies[white] |= bitboards[piece];
    }

    // Black occupancy
    for(int piece = p; piece <= k; piece++)
    {
        occupancies[black] |= bitboards[piece];
    }

    // Total occupancy
    occupancies[both] = occupancies[white] | occupancies[black];
}



void Game::parse_fen(const std::string& fen) {
    
    if(!is_valid(fen))
    {
        std::cerr << "Invalid FEN. Insert a valid FEN\n";
        return;
    }
    
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

    int square = 0;
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

Game Game::copyBoardState()
{
    Game game_copy;

    memcpy(game_copy.bitboards, bitboards, 96); // 96=sizeof(U64) * 12
    memcpy(game_copy.occupancies, occupancies, 24); // 24 = sizeof(U64)*3
    game_copy.side = side;
    game_copy.enpassant= enpassant;
    game_copy.castle = castle;

    return game_copy;
}

void Game::take_back_to(Game revert_to)
{
    for(int piece = P; piece <= k; piece++)
        bitboards[piece] = revert_to.bitboards[piece];
    
    for(int i = 0; i < 3; i++)
        occupancies[i] = revert_to.occupancies[i];

    side = revert_to.side;
    castle = revert_to.castle;
    enpassant = revert_to.enpassant;
    halfmoveClock = revert_to.halfmoveClock;
    fullmoveNumber = revert_to.fullmoveNumber;
}

void Game::make_move(int move)
{
    // parse move
    int source_square = get_source_square(move);
    int target_square = get_target_square(move);
    int piece = get_piece_moved(move);
    int promoted_piece = get_promoted_piece(move);
    int capture = is_capture(move);
    int double_push = is_double_push(move);
    int enpassant_flag = is_en_passant(move);
    int castling_flag = is_castling(move);

    // do the move
    pop_bit(bitboards[piece], source_square);
    set_bit(bitboards[piece], target_square);

    // handling captures
    if (capture) {
        // pick up bitboard of opposide side to move to capture opponent pieces
        int start_piece, end_piece;

        if (side == white) {
            // black pieces range
            start_piece = p;
            end_piece = k;
        } else {
            // white pieces range
            start_piece = P;
            end_piece = K;               
        }

        for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++) {
            // check if there's piece on target square
            if (get_bit(bitboards[bb_piece], target_square)) {
                // remove from correspondant bitboard
                pop_bit(bitboards[bb_piece], target_square);
                break;
            }
        }
    }

    // handling pawn promotion
    if (promoted_piece) {
        // erase pawn from target square
        pop_bit(bitboards[side==white ? P : p], target_square);

        // create new selected piece on target square
        set_bit(bitboards[promoted_piece], target_square);
    }

    // handling enpassant
    if (enpassant_flag) {
        // erase opposed pawn
        if (side == white) {
            pop_bit(bitboards[p], target_square + 8);
        } else {
            pop_bit(bitboards[P], target_square - 8);
        }
    }

    // reset enpassant square
    enpassant = no_sq;

    // handle double pawn push
    if (double_push) {
        // set enpassant square 
        if (side == white) {
            enpassant = target_square + 8;
        } else {
            enpassant = target_square - 8;
        }
    }  

    // handle castling
    if (castling_flag) {
            
        switch(target_square) {
            // white kingside
            case (g1):
                pop_bit(bitboards[R],h1);
                set_bit(bitboards[R],f1);
                break;
            // white queenside
            case (c1):
                pop_bit(bitboards[R],a1);
                set_bit(bitboards[R],d1);
                break;
            // black kingside
            case (g8):
                pop_bit(bitboards[r],h8);
                set_bit(bitboards[r],f8);
                break;
            // black queenside
            case (c8):
                pop_bit(bitboards[r],a8);
                set_bit(bitboards[r],d8);
                break;
        }
    }

    // update castling rights
    castle = castle & castling_rights[source_square];
    castle = castle & castling_rights[target_square];

    // reset and update occupancy tables
    memset(occupancies, 0ULL, 24);

    // white occupancy
    for(int piece = P; piece <= K; piece++)
    {
        occupancies[white] |= bitboards[piece];
    }

    // black occupancy
    for(int piece = p; piece <= k; piece++)
    {
        occupancies[black] |= bitboards[piece];
    }

    // total occupancy
    occupancies[both] = occupancies[white] | occupancies[black];

    // change side after the move is done
    side == white ? side = black : side = white;
}

int Game::get_source_square(int move) {
    return move & 0x3f;
}

int Game::get_target_square(int move) {
    return (move & 0xfc0) >> 6;
}

int Game::get_piece_moved(int move) {
    return (move & 0xf000) >> 12;
}

bool Game::is_capture(int move) {
    return move & 0x100000;
}

bool Game::is_double_push(int move) {
    return move & 0x200000;
}

bool Game::is_en_passant(int move) {
    return move & 0x400000;
}

bool Game::is_castling(int move) {
    return move & 0x800000;
}

int Game::get_promoted_piece(int move) {
    return (move & 0xf0000) >> 16;
}

bool Game::is_promotion(int move) {
    return (move & 0xf0000) >> 16;
}