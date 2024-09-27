#include "search.h"

namespace flk {

    int nodes = 0, ply = 0;
    int follow_pv = 0, score_pv = 0;  

    // Triangular principal variation table
    Move pv_table[MAX_PV_LENGTH][MAX_PV_LENGTH];

    // Stores the length of the pv at each ply
    int pv_length[MAX_PV_LENGTH];

    // Killer moves matrix (2 killer moves x ply)
    // Killer moves are stored up to depth = MAX_KILLER_DEPTH
    Move killer_moves[2][MAX_PV_LENGTH];
    
    // History moves scores (12 pieces x 64 squares)
    int history_moves[PIECE_TYPES][SQUARES];

    bool is_check(Game& game) {
        MoveGenerator m(game);
        return m.is_square_attacked(game.get_side() == white ? 
                                            get_ls1b_index(game.get_bitboard(K)) :
                                            get_ls1b_index(game.get_bitboard(k)), game.get_side() ^ 1);
    }

    // Negamax searching algorithm
    int negamax(Game& game, int depth, int alpha, int beta)
    {
        // Initialize pv
        pv_length[ply] = ply;

        // Base case: start the quiescence search
        if (depth == 0)
            return quiescence_search(game, alpha, beta);
        
        // If we have reached the maximum ply length stop
        // the search and evaluate the current position
        if(ply > MAX_PV_LENGTH - 1)
            return evaluate(game);

        // Only used to check if the new features that
        // we will introduce will actually reduce the
        // number of nodes searched
        nodes++;

        // Generate all possible moves in the position
        MoveGenerator m(game);
        MoveArray legal_moves = m.generate_moves(all_moves);

        // If we are following the pv line, put the pv move
        // on top of the search 
        if(follow_pv) {
            enable_pv_score(legal_moves);
        }

        sort_moves(legal_moves, game);

        // If there are no legal moves then it is either
        // checkmate or stalemate
        int is_check = m.is_square_attacked(game.get_side() == white ? 
                                            get_ls1b_index(game.get_bitboard(K)) :
                                            get_ls1b_index(game.get_bitboard(k)), game.get_side() ^ 1);

        // If the king is in check, search one move deeper
        // to avoid mates
        if(is_check) depth++; 

        if(legal_moves.count == 0)
        {
            if(is_check)
                return -10000 + ply;
            else return 0;
        }

        int score;

        // Copy the game state
        Game g(game);

        // Loop through all the legal moves
        for(int i = 0; i < legal_moves.count; i++)
        {      
            // Play the move and increment the ply counter
            g.make_move(legal_moves.move_list[i]);
            ply++;

            // Call negamax again for the opposite side and depth - 1
            score = -negamax(g, depth - 1, -beta, -alpha);

            // Take back the move and reduce the ply counter
            ply--;
            g.take_back_to(game);

            // Alpha - Beta pruning
            //
            // Beta cut-off: the opponent will not go along this
            // line since it already has better alternatives so 
            // cut the search 
            if(score >= beta) {
            
                if(!legal_moves.move_list[i].is_capture()) {
                    // Found killer move, store it in the dedicated array
                    killer_moves[1][ply] = killer_moves[0][ply];
                    killer_moves[0][ply] = legal_moves.move_list[i];
                }
                return beta;
            }

            // New best move found 
            if(score > alpha)
            {
                // Update history score
                if(!legal_moves.move_list[i].is_capture()) {
                    history_moves[legal_moves.move_list[i].get_piece_moved()]
                                 [legal_moves.move_list[i].get_target_square()] += depth;
                }

                // Update the alpha score
                alpha = score;

                // Save the move as part of the principal variation
                pv_table[ply][ply] = legal_moves.move_list[i];

                // Copy the moves from deeper pvs in the current pv line
                for(int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++)
                    pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];

                // Update the length of the pv line
                pv_length[ply] = pv_length[ply + 1];
            }
        }
        // Return the value of the best move found
        // in the position
        return alpha;
    }

    // Quiescence search function for searching until no captures
    // are available
    int quiescence_search(Game& game, int alpha, int beta)
    {
        nodes++;

        // Evaluate the position
        int evaluation = flk::lazy_evaluation(game);

        // If the score is better than the opponent's
        // best response then prune the branch
        if(evaluation >= beta) {
                return beta;
        }

        // If the score is the better than the current alpha, save it 
        if(evaluation > alpha) {
            alpha = evaluation;
        }

        // Generate only captures
        MoveGenerator m(game);
        MoveArray captures = m.generate_moves(only_captures);
        sort_moves(captures, game);

        Game g(game);
        int score;

        // Search
        for(int i = 0; i < captures.count; i++)
        {      
            g.make_move(captures.move_list[i]);
            ply++;

            score = -quiescence_search(g, -beta, -alpha);

            g.take_back_to(game);
            ply--;

            // If the score is better than the opponent's best
            // response then prune the branch
            if(score >= beta) {
                return beta;
            }

            // If the score is the better than the current alpha, save it 
            if(score > alpha) {
                alpha = score;
            }
        }

        // return the best score in the position
        return alpha;
    }
    
    // Clears moves' tables
    void clear_tables() {

        // Reset flags
        nodes = 0;
        ply = 0;
        follow_pv = 0;
        score_pv = 0;

        // Clear killer moves table
        for(int i = 0; i < MAX_PV_LENGTH; i++) {
            killer_moves[0][i] = Move();
            killer_moves[1][i] = Move();
        }

        // Clear history scores table
        for(int i = 0; i < PIECE_TYPES; i++) {
            for(int j = 0; j < MAX_PV_LENGTH; j++) {
                history_moves[i][j] = 0;
            }
        }

        // Clear pv_table and pv_length
        for(int i = 0; i < MAX_PV_LENGTH; i++) {
            pv_length[i] = 0;
            
            for(int j = 0; j < MAX_PV_LENGTH; j++)
                pv_table[i][j] = Move();
        }
    }
    
    // Checks if we are still inside the pv line and in that
    // case enables the pv moves scoring and ordering
    void enable_pv_score(MoveArray& legal_moves) {

        // Disable the follow pv flag
        follow_pv = 0;

        // Loop through the legal moves
        for(int i = 0; i < legal_moves.count; i++) {

            // If we found a move contained in the principal variation
            // at this ply
            if(legal_moves.move_list[i] == pv_table[0][ply]) {

                // Activate the score pv flag
                score_pv = 1;

                // We are still in the pv line, reactivate the
                // follow pv flag
                follow_pv = 1;
            }
        }
    }

    Move iterative_search(Game& game, int depth) {

        int alpha = -50000, beta = 50000;

        follow_pv = 0;
        score_pv = 0;

        clear_tables();

        for(int current_depth = 1; current_depth <= depth; current_depth++) {
            // Reset the nodes count
            nodes = 0;

            follow_pv = 1;

            // Search the current position at the current depth
            negamax(game, current_depth, alpha, beta);
            /*
            std::cout << "Principal variation at depth " << current_depth << ": ";
	        for(int i = 0; i < pv_length[0]; i++) {
                pv_table[0][i].print_move();
                std:: cout << "  ";
            }
	        std::cout << "\nNumber of nodes searched: " << flk::nodes << "\n";*/
        }

        return pv_table[0][0];
    }

    // Returns the score of a move relevant for 
    // move ordering
    int score_move(Move move, Game& game) {

        // If we are still in the pv line
        if(score_pv) {
            // Search the pv move
            if(pv_table[0][ply] == move) {
                // Unset the score_pv flag
                score_pv = 0;
                
                // Return the highest score to the pv move
                return 20000;
            }
        }

        // If the move is a capture
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

    // Sorts the move based on their scores
    void sort_moves(MoveArray& moves, Game& game) {
        // Array of move scores
        int move_scores[moves.count];

        // Computes the move scores
        for(int i = 0; i < moves.count; i++)
            move_scores[i] = score_move(moves.move_list[i], game);

        // Sort the moves using selection sort
        for(int current = 0; current < moves.count; current++)
        {
            for(int next = current + 1; next < moves.count; next++)
            {
                if(move_scores[current] < move_scores[next])
                {
                    int temp = move_scores[current];
                    move_scores[current] = move_scores[next];
                    move_scores[next] = temp;

                    Move temp_move = moves.move_list[current];
                    moves.move_list[current] = moves.move_list[next];
                    moves.move_list[next] = temp_move;
                }
            }
        }
    }
    
    // Print each move's score
    void print_score(MoveArray& list, Game& game) {
        std::cout << "Move scores:\n\n";
        for(int i = 0; i < list.count; i++) {
            std::cout << i+1 << ": ";
            list.move_list[i].print_move();
            std::cout << "   Score: " << score_move(list.move_list[i], game) << "\n";
        }
    }

    // Performs a perf test search in the current position
    void perft_search(Game& game, int depth, std::vector<int>& move_count)
    {
        if (depth == 0)
            return;
        
        MoveGenerator m(game);
        MoveArray legal_moves = m.generate_moves(all_moves);

        if(legal_moves.count == 0)
            return;

        int moves_in_this_position = legal_moves.count;
        move_count.at(depth - 1) += moves_in_this_position;

        Game g(game);
    
        for(int i = 0; i < legal_moves.count; i++)
        {
            g.make_move(legal_moves.move_list[i]);
            perft_search(g, depth - 1, move_count);
            g.take_back_to(game);
        }

        return;
    }

    // Perf test driver
    void Perft(Game& game, int depth)
    {
        std::vector<int> move_count(depth, 0);

        MoveGenerator move_gen(game);
        MoveArray legal_moves = move_gen.generate_moves(all_moves);

        Game g(game);

        int previous_nodes = 0;

        std::cout << "\n**********************\n";
        std::cout << "     Perft Results\n";
        std::cout << "**********************\n\n";

        std::cout << "Nodes per move:\n";
        for(int i = 0; i < legal_moves.count; i++)
        {
            move_count.at(depth - 1)++;
            if(depth == 1)
            {
                legal_moves.move_list[i].print_move();
                std::cout << ": 1\n"; 
            }
            else
            {
                g.make_move(legal_moves.move_list[i]);
                perft_search(g, depth - 1, move_count);

                legal_moves.move_list[i].print_move();
                std::cout << ": " << move_count.at(0) - previous_nodes << "\n";
                previous_nodes = move_count.at(0);

                g.take_back_to(game);
            }
        }
        
        int total_nodes = 1;

        std::cout << "\nTotal nodes per depth level:\n";
        std::cout << "Nodes at depth 0: 1\n";
        for(int i = 0; i < move_count.size(); i++)
        {
            std::cout << "Nodes at depth " << i + 1 << ": " << move_count.at(move_count.size() - 1 - i) << "\n";
            total_nodes += move_count.at(i);
        }
        std::cout << "\nTotal nodes searched: " << total_nodes << "\n";
    }
}