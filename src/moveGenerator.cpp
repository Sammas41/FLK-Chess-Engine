#include "moveGenerator.h"

int MoveGenerator::is_square_attacked(int square, int side){
    
    // attacked by white pawns
    if((side == white) && 
       (get_pawn_attack(black,square) & game.get_bitboard(P))) return 1;
    
    // attacked by black pawns
    if((side == black) && 
       (get_pawn_attack(white,square) & game.get_bitboard(p))) return 1;
    
    // attacked by knights
    if(get_knight_attack(square) & 
      ((side == white) ? game.get_bitboard(N) : game.get_bitboard(n))) return 1;
    
    // attacked by king
    if(get_king_attack(square) & 
      ((side == white) ? game.get_bitboard(K) : game.get_bitboard(k))) return 1;
    
    // attacked by bishops
    if(get_bishop_attack(square, game.get_occupancy(both)) & 
      ((side == white) ? game.get_bitboard(B) : game.get_bitboard(b))) return 1;
    
    // attacked by rooks
    if(get_rook_attack(square, game.get_occupancy(both)) & 
      ((side == white) ? game.get_bitboard(R) : game.get_bitboard(r))) return 1;

    // attacked by queens
    if(get_queen_attack(square, game.get_occupancy(both)) & 
      ((side == white) ? game.get_bitboard(Q) : game.get_bitboard(q))) return 1;
    
    // by default return false (square not attacked)
    return 0;
}

// print attacked squares
void MoveGenerator::print_attacked_squares(int side){

    printf("\n");

    for (int rank = 0; rank < 8; rank ++){

        for (int file = 0; file < 8; file++){

            int square = rank * 8 + file;

            // print ranks
            if(!file) {
                printf("  %d   ", 8 - rank);
            }   

            // check wether current square is attacked  
            printf("%d ",is_square_attacked(square, side) ? 1 : 0);

        }

        // print new line every rank
        printf("\n");
    }

    // print files
    printf("\n      a b c d e f g h\n\n");
}

// Generate all possible moves in the position
MoveArray MoveGenerator::generate_moves(int capture_flag){

    // loop through all bitboards
    for(int piece = P; piece <= k; piece++){
        // init piece bitboard copy
        U64 bitboard = game.get_bitboard(piece);

        // generate white pawns & white king castling
        // not relying on precalculated attack tables
        if(game.get_side() == white){
            
            generate_white_pawns_moves(piece, bitboard);
            generate_white_king_castling_moves(piece, bitboard);
            
        }
        // generate black pawns & black king castling
        else{
            generate_black_pawns_moves(piece, bitboard);
            generate_black_king_castling_moves(piece, bitboard);
        }

        // generate knight moves
        generate_knights_moves(piece,bitboard,game.get_side());

        // generate bishop moves
        generate_bishops_moves(piece,bitboard,game.get_side());
        
        // generate rook moves
        generate_rooks_moves(piece,bitboard,game.get_side());

        // generate queen moves
        generate_queens_moves(piece,bitboard,game.get_side());

        // generate king moves
        generate_kings_moves(piece,bitboard,game.get_side());
    }

    MoveArray legal_moves;

    if(capture_flag == only_captures)
    {
        for(int i = 0; i < possible_moves.count; i++)
        {
            if(is_legal(possible_moves.move_list[i]) && possible_moves.move_list[i].is_capture())
            {
                legal_moves.move_list[legal_moves.count] = possible_moves.move_list[i];
                legal_moves.count++;
            }
        }
    }
    else
    {
        for(int i = 0; i < possible_moves.count; i++)
        {
            if(is_legal(possible_moves.move_list[i]))
            {
                legal_moves.move_list[legal_moves.count] = possible_moves.move_list[i];
                legal_moves.count++;
            }
        }
    }

    return legal_moves;
}

// Generate white pawn moves (push, double push, captures and en passant)
void MoveGenerator::generate_white_pawns_moves(int piece, U64 bitboard) {
    if (piece == P){
        // loop over white pawns (while theres bits(pieces) available)
        while (bitboard){
            int source_square = get_ls1b_index(bitboard);
            // move white pawn forward
            int target_square = source_square - 8;

            // generate quite pawn moves
            if(!(target_square < 0) && !get_bit(game.get_occupancy(both), target_square)){
                // three type of moves
                // PAWN PROMOTION
                if (source_square >= a7 && source_square <= h7){
                    possible_moves.add_move(Move(source_square,target_square,piece, Q, 0,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, R, 0,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, B, 0,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, N, 0,0,0,0));    
                } 
                else{
                    // ONE SQUARE AHEAD MOVE
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 0,0,0,0));

                    // TWO SQUARES AHEAD MOVE
                    if((source_square >= a2 && source_square <= h2) && !get_bit(game.get_occupancy(both), target_square - 8)){
                        possible_moves.add_move(Move(source_square,target_square - 8,piece, 0, 0,1,0,0));
                    }

                }
            }

            U64 attacks = get_pawn_attack(white,source_square) & game.get_occupancy(black);

            // generate en passant captures
            int en_passant_square = game.get_enpassant();
            if(en_passant_square != no_sq)
            {
                if(source_square == a5 && en_passant_square == b6)
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == b5 && (en_passant_square == a6 || en_passant_square == c6))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == c5 && (en_passant_square == b6 || en_passant_square == d6))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == d5 && (en_passant_square == c6 || en_passant_square == e6))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == e5 && (en_passant_square == d6 || en_passant_square == f6))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == f5 && (en_passant_square == e6 || en_passant_square == g6))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == g5 && (en_passant_square == f6 || en_passant_square == h6))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == h5 && en_passant_square == g6)
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));
            }

            // generate pawn captures
            while(attacks){

                target_square = get_ls1b_index(attacks);

                // CAPTURE AND PROMOTION
                if (source_square >= a7 && source_square <= h7){
                    possible_moves.add_move(Move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                    possible_moves.add_move(Move(source_square, target_square, piece, R, 1, 0, 0, 0));
                    possible_moves.add_move(Move(source_square, target_square, piece, B, 1, 0, 0, 0));
                    possible_moves.add_move(Move(source_square, target_square, piece, N, 1, 0, 0, 0));

                } 
                else{
                    // CAPTURE
                    possible_moves.add_move(Move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, target_square);
            }
            
            // pop ls1b from piece bitboard copy
            pop_bit(bitboard, source_square);

        }
    }
}

// Generate white castling moves
void MoveGenerator::generate_white_king_castling_moves(int piece, U64 bitboard) {
    if(piece == K){
        // king side castling available
        if (game.get_castle() & game.wk){
            // check no pieces between K and R
            if(!get_bit(game.get_occupancy(both), f1) && !get_bit(game.get_occupancy(both), g1)){
                // king and f1 squares not attacked
                if(!is_square_attacked(e1,black) && !is_square_attacked(f1,black)){
                    possible_moves.add_move(Move(e1, g1, piece, 0, 0, 0, 0, 1));
                }
            }
        }

        // queen side available
        if (game.get_castle() & game.wq){
            // make sure square between king and queen's rook are empty
            if (!get_bit(game.get_occupancy(both), d1) && !get_bit(game.get_occupancy(both), c1) && !get_bit(game.get_occupancy(both), b1))
            {
                // make sure king and the d1 squares are not under attacks
                if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
                    possible_moves.add_move(Move(e1, c1, piece, 0, 0, 0, 0, 1));
            }
            
        }
    }
}

// Generate black pawn moves (push, double push, captures and en passant)
void MoveGenerator::generate_black_pawns_moves(int piece, U64 bitboard) {
    if (piece == p){
        // loop over black pawns (while theres bits(pieces) available)
        while (bitboard){
            int source_square = get_ls1b_index(bitboard);
            // move black pawn forward
            int target_square = source_square + 8;

            // generate quite pawn moves
            if(!(target_square > h1) && !get_bit(game.get_occupancy(both), target_square)){
                // three type of moves
                // PAWN PROMOTION
                if (source_square >= a2 && source_square <= h2){
                    possible_moves.add_move(Move(source_square,target_square,piece, q, 0,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, r, 0,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, b, 0,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, n, 0,0,0,0));
        
                } 
                else{
                    // ONE SQUARE AHEAD MOVE
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 0,0,0,0));
                    // TWO SQUARES AHEAD MOVE
                    if((source_square >= a7 && source_square <= h7) && !get_bit(game.get_occupancy(both), target_square + 8)){
                        possible_moves.add_move(Move(source_square,target_square + 8,piece,0, 0,1,0,0));
                    }

                }
            }

            U64 attacks =  get_pawn_attack(black,source_square) & game.get_occupancy(white);
            
            // generate en passant captures
            int en_passant_square = game.get_enpassant();
            if(en_passant_square != no_sq)
            {
                if(source_square == a4 && en_passant_square == b3)
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == b4 && (en_passant_square == a3 || en_passant_square == c3))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == c4 && (en_passant_square == b3 || en_passant_square == d3))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == d4 && (en_passant_square == c3 || en_passant_square == e3))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == e4 && (en_passant_square == d3 || en_passant_square == f3))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == f4 && (en_passant_square == e3 || en_passant_square == g3))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == g4 && (en_passant_square == f3 || en_passant_square == h3))
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));

                if(source_square == h4 && en_passant_square == g3)
                    possible_moves.add_move(Move(source_square, en_passant_square, piece, 0, 1, 0, 1, 0));
            }

            // generate pawn captures
            while(attacks){

                target_square = get_ls1b_index(attacks);

                // CAPTURE AND PROMOTION
                if (source_square >= a2 && source_square <= h2){
                    possible_moves.add_move(Move(source_square,target_square,piece, q, 1,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, r, 1,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, b, 1,0,0,0));
                    possible_moves.add_move(Move(source_square,target_square,piece, n, 1,0,0,0));
        
                } 
                else{
                    // CAPTURE
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 1,0,0,0));
                }
                pop_bit(attacks, target_square);
            }

            // pop ls1b from piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}

// Generate black castling moves
void MoveGenerator::generate_black_king_castling_moves(int piece, U64 bitboard) {
    // castling moves
    if (piece == k){
        // king side castling is available
        if (game.get_castle() & game.bk)
        {
            // make sure square between king and king's rook are empty
            if (!get_bit(game.get_occupancy(both), f8) && !get_bit(game.get_occupancy(both), g8))
            {
                // make sure king and the f8 squares are not under attacks
                if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
                    possible_moves.add_move(Move(e8,g8,piece, 0, 0,0,0,1));
            }
        }
        
        // queen side castling is available
        if (game.get_castle() & game.bq)
        {
            // make sure square between king and queen's rook are empty
            if (!get_bit(game.get_occupancy(both), d8) && !get_bit(game.get_occupancy(both), c8) && !get_bit(game.get_occupancy(both), b8))
            {
                // make sure king and the d8 squares are not under attacks
                if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
                    possible_moves.add_move(Move(e8,c8,piece, 0, 0,0,0,1));
            }
        }
    }
}

// Generate all knight moves (white and black)
void MoveGenerator::generate_knights_moves(int piece, U64 bitboard, int side) {
     // genarate knight moves
    if ((side == white) ? piece == N : piece == n)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_knight_attack(source_square) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}

// Generate all bishop moves (white and black)
void MoveGenerator::generate_bishops_moves(int piece, U64 bitboard, int side){
    // generate bishop moves
    if ((side == white) ? piece == B : piece == b)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_bishop_attack(source_square, game.get_occupancy(both)) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}

// Generate all rook moves (white and black)
void MoveGenerator::generate_rooks_moves(int piece, U64 bitboard, int side){

    // generate rook moves
    if ((side == white) ? piece == R : piece == r)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_rook_attack(source_square, game.get_occupancy(both)) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
        
}

// Generate all queen moves (white and black)
void MoveGenerator::generate_queens_moves(int piece, U64 bitboard, int side){

    // generate queen moves
    if ((side == white) ? piece == Q : piece == q)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_queen_attack(source_square, game.get_occupancy(both)) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }
            
            
            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}

// Generate all king moves (white and black)
void MoveGenerator::generate_kings_moves(int piece, U64 bitboard, int side){

    // generate king moves
    if ((side == white) ? piece == K : piece == k)
    {
        // loop over source squares of piece bitboard copy
        while (bitboard)
        {
            // init source square
            int source_square = get_ls1b_index(bitboard);
            
            // init piece attacks in order to get set of target squares
            U64 attacks =  get_king_attack(source_square) & ((side == white) ? ~game.get_occupancy(white) : ~game.get_occupancy(black));
            
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = get_ls1b_index(attacks);    
                
                // quite move
                if (!get_bit(((side == white) ? game.get_occupancy(black) : game.get_occupancy(white)), target_square))
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 0,0,0,0));
                
                else
                    // capture move
                    possible_moves.add_move(Move(source_square,target_square,piece, 0, 1,0,0,0));
                
                // pop ls1b in current attacks set
                pop_bit(attacks, target_square);
            }

            // pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }
}

// Checks if a move is legal or not
bool MoveGenerator::is_legal(Move move)
{
    Game copy(game);
    game.make_move(move);

    // check if the king is in check
    if (is_square_attacked((game.get_side() == white) ? 
                            get_ls1b_index(game.get_bitboard(k)) : get_ls1b_index(game.get_bitboard(K)),
                            game.get_side()))
    {
        game.take_back_to(copy);
        return false;
    }
    else
    {
        game.take_back_to(copy);
        return true;
    }
}

// Adds a move to the possible moves array
void MoveArray::add_move(Move move)
{
    move_list[count] = move;
    count++;
}

void MoveGenerator::print_move_pretty(Move move) {
    std::cout << "|   " << square_to_coordinates[move.get_source_square()] << "   |   "
              << square_to_coordinates[move.get_target_square()] << "   |   "
              << ascii_pieces[move.get_piece_moved()] << "   |   "
              << (move.is_capture() ? " 1 " : " 0 ") << "   |     "
              << (move.is_double_push() ? " 1 " : " 0 ") << "     |    "
              << (move.get_promoted_piece() ? " 1 " : " 0 ") << "    |    "
              << (move.is_castling() ? " 1 " : " 0 ") << "   |     "
              << (move.is_en_passant() ? " 1 " : " 0 ") << "    |\n"; 
              //<< score_move(move, 0, NULL, NULL) << "  |\n";
}

void MoveGenerator::print_move_list(MoveArray legal_moves) {

    std::cout << "---------------------------------------------------------------------------------------\n";
    std::cout << "| Source | Target | Piece | Capture | Double push | Promotion | Castling | En Passant |\n";
    std::cout << "---------------------------------------------------------------------------------------\n";
    for(int i = 0; i < legal_moves.count; i++)
        print_move_pretty(legal_moves.move_list[i]);
    std::cout << "---------------------------------------------------------------------------------------\n";
}

int MoveGenerator::score_move(Move move, int ply, 
                              Move killer_moves[][MAX_KILLER_DEPTH], int history_moves[][SQUARES]) {
    if(move.is_capture())
    {
        int attacking_piece = move.get_piece_moved();
        int attacked_square = move.get_target_square();

        int start_piece, end_piece;
        if(game.get_side() == white) { start_piece = p; end_piece = k;}
        else { start_piece = P; end_piece = K; }

        int attacked_piece = P;
        // To get the captured piece, loop through all the pieces
        for(int piece = start_piece; start_piece < end_piece; piece++)
        {
            // If there is a bit set on the attacked square then we
            // have found the attacked piece.
            // NOTE: in case of en passant capture then the attacked
            //       square is empty but since attacked_piece is initialized
            //       to P we return the correct value anyway. In case
            //       of switched color it works fine also in this case
            //       due to the symmetry of MVV_LVA matrix:
            //       MVV_LVA[P][p] = MVV_LVA[p][P]  
            if(get_bit(game.get_bitboard(piece), attacked_square))
            {
                attacked_piece = piece;
                break;
            }
        }

        return MVV_LVA[attacking_piece][attacked_piece] + 10000;
    }
    // quiet moves scores
    else {
        if(move == killer_moves[0][ply])
            return 9000;
        else if(move == killer_moves[1][ply])
            return 8000;
        else return history_moves[move.get_piece_moved()][move.get_target_square()];
    }

    return 0;
}

void MoveGenerator::sort_moves(MoveArray& moves, int ply,
                               Move killer_moves[][MAX_KILLER_DEPTH], int history_moves[][SQUARES]) {
    int max, score, index_to_swap = 0;
    bool swap;
    Move temp;

    for(int i = 0; i < moves.count; i++)
    {
        swap = false;
        max = -50000;
        for(int j = i; j < moves.count; j++)
        {
            score = score_move(moves.move_list[j], ply, killer_moves, history_moves);
            if(max < score)
            {
                swap = true;
                max = score;
                index_to_swap = j;
            }
        }
    
        if(swap) {
            temp = moves.move_list[index_to_swap];
            moves.move_list[index_to_swap] = moves.move_list[i];
            moves.move_list[i] = temp;
        }
    }
}

void MoveGenerator::print_score(MoveArray list, Move killer[][MAX_KILLER_DEPTH], int hist[][SQUARES])
{
    for(int i = 0; i < list.count; i++) {
        std::cout << i+1 << ": ";
        list.move_list[i].print_move();
        std::cout << "   Score: " << score_move(list.move_list[i], 0, killer, hist) << "\n";
    }
}