#include "search.h"

namespace flk {

    int nodes = 0, ply = 0;

    int negamax(Game* game, int depth, int alpha, int beta, int& best_move)
    {
        std::cout << "Game bitboards:\n";
        for(int piece = P; piece <= k; piece++)
        {
            std::cout << "  [" << piece << "]: " << game->get_bitboard(piece) << "\n";
        }
        system("PAUSE");

        // if we reached the maximum depth, return
        // the evaluation of the position
        if(depth == 0)
        {
            std::cout << "Evaluation: " << evaluate(*game) << "\n";
            return evaluate(*game);
        }

        nodes++;
        
        // generate all possible moves in the current position
        MoveGenerator move_gen(*game);
        move_gen.generate_moves();

        // loop over all the moves
        for(int count = 0; count < move_gen.getMover().moveList.count; count++)
        {  
            // save the current state of the board 
            Game copy_game(*game);
            
            // increase the ply counter
            ply++;

            int legal_move = move_gen.make_move(move_gen.getMover().moveList.movesArray[count], all_moves);

            // checks if we are making an illegal move
            if(!legal_move)
            {   
                // skips the search in this case
                ply--;
                continue;
            }

            std::cout << "Game bitboards after a3 move:\n";
            for(int piece = P; piece <= k; piece++)
            {
                std::cout << "  [" << piece << "]: " << game->get_bitboard(piece) << "\n";
            }
            system("PAUSE");
            // if the move is legal then keep searching along this
            // branch of the moves-tree
            int score = -negamax(game, depth - 1, -beta, -alpha, best_move);

            // restore the board to its original state when we have
            // evaluate the move
            move_gen.takeBack();
            ply--;
            /*
            std::cout << "After takeback\n";
            game.print_board();
            std::cout << "depth: " << depth << "\n";
            system("PAUSE");
            */
            // if nodes "fails high" we can prune the search on
            // this branch
            if(score >= beta) return beta;
            
            if(score > alpha)
            {
                alpha = score;

                // if we are at the root node, save this move as best
                if(ply == 0) best_move = move_gen.getMover().moveList.movesArray[count];

            }
        }

        // if the node "fails low"
        return alpha;
    }
}