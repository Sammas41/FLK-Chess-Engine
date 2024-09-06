#include "search.h"

namespace flk {

    int nodes = 0, ply = 0;
    
    int Negamax(Game& game, int depth, int alpha, int beta, int& best_move)
    {
        if (depth == 0)
            return 0; // Quiescence_search(game, alpha, beta);
        
        // Only used to check if the new features that
        // we will introduce will actually reduce the
        // number of nodes searched
        nodes++;

        // Generate all possible moves in the position
        MoveGenerator m(game);

        // If there are no legal moves then it is either
        // checkmate or stalemate
        int is_check = m.is_square_attacked(game.get_side() == white ? 
                                            get_ls1b_index(game.get_bitboard(K)) :
                                            get_ls1b_index(game.get_bitboard(k)), game.get_side() ^ 1);

        if(m.legal_moves.count == 0)
        {
            if(is_check)
                return -1000 + ply;
            else return 0;
        }

        int score;

        // Copy the game state
        Game g(game);

        // Loop through all the legal moves
        for(int i = 0; i < m.legal_moves.count; i++)
        {
            // Play the move and increment the ply counter
            g.make_move(m.legal_moves.move_list[i]);
            ply++;

            // Call negamax again for the opposite side and depth - 1
            score = -Negamax(g, depth - 1, -beta, -alpha, best_move);
            
            // Take back the move and reduce the ply counter
            ply--;
            g.take_back_to(game);

            // Alpha - Beta pruning
            // f the move is better than I
            if(score >= beta)
                return beta;

            // If the best move 
            if(score > alpha)
            {
                alpha = score;
                // If we are at the root node (thus ply = 0)
                // then save the best move
                if(ply == 0)
                    best_move = m.legal_moves.move_list[i];
            }
        }
        // return
        return alpha;
    }
    /*
    int Quiescence_search(Game& game, int alpha, int beta)
    {
        MoveGenerator m(game);
        moves captures = m.getMover().get_capture_move_list();

        // If there are no legal moves then it is either
        // checkmate or stalemate
        if(captures.count == 0)
        {
            std::cout << "No captures available, checking for mate/stalemate\n";
            game.print_board();
            std::cin.get();

            if(m.generate_moves().empty())
            {
                std::cout << "No legal moves\n";
                std::cin.get();
                int is_check = m.is_square_attacked(game.get_side() == white ? 
                                                get_ls1b_index(game.get_bitboard(K)) :
                                                get_ls1b_index(game.get_bitboard(k)), game.get_side() ^ 1);
                
                if(is_check) return -1000 + ply;
                else return 0;
            }
            else
            {
                std::cout << "Return static evaluation\n";
                std::cin.get();
                return 5;
            }
        }

        Game g(game);
        int score;

        std::cout << "Search until no captures\n";
        std::cin.get();

        for(int i = 0; i < captures.count; i++)
        {
            g.make_move(captures.movesArray[i]);
            ply++;

            score = -Quiescence_search(g, -beta, -alpha);

            g.take_back_to(game);
            ply--;

            if(score >= beta)
                return beta;

            // If the best move 
            if(score > alpha)
                alpha = score;
        }
        // return
        return alpha;
    } 
    */
    void perft_search(Game& game, int depth, std::vector<int>& move_count)
    {   
        if (depth == 0)
            return;
        
        MoveGenerator m(game);

        if(m.legal_moves.move_list.empty())
            return;

        int moves_in_this_position = m.legal_moves.count;
        move_count.at(depth - 1) += moves_in_this_position;

        Game g(game);
    
        for(int i = 0; i < m.legal_moves.count; i++)
        {
            g.make_move(m.legal_moves.move_list[i]);
            perft_search(g, depth - 1, move_count);
            g.take_back_to(game);
        }

        return;
    }

    void Perft(Game& game, int depth)
    {
        std::vector<int> move_count(depth, 0);

        MoveGenerator move_gen(game);

        Game g(game);

        int previous_nodes = 0;

        std::cout << "\n**********************\n";
        std::cout << "     Perft Results\n";
        std::cout << "**********************\n\n";

        std::cout << "Nodes per move:\n";
        for(int i = 0; i < move_gen.legal_moves.count; i++)
        {
            move_count.at(depth - 1)++;
            if(depth == 1)
            {
                move_gen.print_move(move_gen.legal_moves.move_list[i]);
                std::cout << ": 1\n"; 
            }
            else
            {
                g.make_move(move_gen.legal_moves.move_list[i]);
                perft_search(g, depth - 1, move_count);

                move_gen.print_move(move_gen.legal_moves.move_list[i]);
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