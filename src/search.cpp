#include "search.h"

namespace flk {

    int nodes = 0, ply = 0;

    int negamax(Game* game, int depth, int alpha, int beta, int& best_move)
    {
        // if the node "fails low"
        return alpha;
    }

    void perft_search(Game& game, int depth, std::vector<int>& move_count)
    {   
        if (depth == 0)
            return;
        
        MoveGenerator m(game);
        std::vector<int> legal_moves = m.generate_moves();

        if(legal_moves.empty()) return;

        move_count.at(depth - 1) += legal_moves.size();

        Game g(game);

        for(int count = 0; count < legal_moves.size(); count++)
        {
            g.make_move(legal_moves.at(count));
            perft_search(g, depth - 1, move_count);
            
            g.take_back_to(game);
        }

        return;
    }

    void Perft(Game& game, int depth)
    {
        std::vector<int> move_count(depth, 0);

        MoveGenerator move_gen(game);
        std::vector<int> initial_moves = move_gen.generate_moves();

        Game g(game);

        int previous_nodes = 0;

        std::cout << "\n**********************\n";
        std::cout << "     Perft Results\n";
        std::cout << "**********************\n\n";

        std::cout << "Nodes per move:\n";
        for(int m : initial_moves)
        {
            move_count.at(depth - 1)++;
            if(depth == 1)
            {
                move_gen.getMover().print_move(m);
                std::cout << ": 1\n"; 
            }
            else
            {
                g.make_move(m);
                perft_search(g, depth - 1, move_count);

                move_gen.getMover().print_move(m);
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