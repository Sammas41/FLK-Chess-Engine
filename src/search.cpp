#include "search.h"

namespace flk {

    int nodes = 0, ply = 0;

    bool check_depth_1 = false, check_depth_2 = false, check_depth_3 = false;
    MoveArray test_moves;

    // Killer moves matrix (two killer moves are stored)
    Move killer_moves[2][100];

    // History moves matrix for each piece (12 pieces x 64 squares)
    int history_moves[12][SQUARES];

    // Principal variation table
    Move pv_table[MAX_PV_LENGTH][MAX_PV_LENGTH];

    // This arrays contains the pv length at each ply
    int pv_length[MAX_PV_LENGTH];

    int negamax(Game& game, int depth, int alpha, int beta, Move& best_move)
    {
        // Initialize pv
        pv_length[ply] = ply;

        if (depth == 0)
            return quiescence_search(game, alpha, beta);
        
        // Only used to check if the new features that
        // we will introduce will actually reduce the
        // number of nodes searched
        nodes++;

        // Generate all possible moves in the position
        MoveGenerator m(game);
        MoveArray legal_moves = m.generate_moves(all_moves);
        m.sort_moves(legal_moves, ply, killer_moves, history_moves);

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
            score = -negamax(g, depth - 1, -beta, -alpha, best_move);

            check_depth_3 = false;

            // Take back the move and reduce the ply counter
            ply--;
            g.take_back_to(game);

            // Alpha - Beta pruning
            // f the move is better than I
            if(score >= beta) {
            
                if(!legal_moves.move_list[i].is_capture() && legal_moves.move_list[i] != killer_moves[0][ply]) {
                    // Found killer move, store it in the dedicated array
                    killer_moves[1][ply] = killer_moves[0][ply];
                    killer_moves[0][ply] = legal_moves.move_list[i];
                }
                return beta;
            }

            // If the best move 
            if(score > alpha)
            {
                // Update history score
                if(!legal_moves.move_list[i].is_capture()) {
                    history_moves[legal_moves.move_list[i].get_piece_moved()]
                                 [legal_moves.move_list[i].get_target_square()] += depth;
                }

                alpha = score;

                // Save the move as part of the principal variation
                pv_table[ply][ply] = legal_moves.move_list[i];

                for(int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++)
                    pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];

                pv_length[ply] = pv_length[ply + 1];
                
                // If we are at the root node (thus ply = 0)
                // then save the best move
                if(ply == 0)
                    best_move = legal_moves.move_list[i];
            }
        }
        // return
        return alpha;
    }

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
        m.sort_moves(captures, ply, killer_moves, history_moves);

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