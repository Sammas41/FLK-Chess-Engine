#include "engine.h"

Engine::Engine() : game() {
    
}

Engine::Engine(std::string fen) : game(fen) {
    
}

void Engine::play(char side) {

    if(side == 'w' || side == 'W')
    {
        // Print board
        std::cout << "\n";
        game.print_board();

        while(is_running()) { 

            // Read player's move
            Move player_move = get_player_move(white);

            // Make player's move
            game.make_move(player_move);
            
            // Let the engine find the best move
            Move engine_move = search_position(6);

            // Output the engine move
            std::cout << "Engine played: ";
            engine_move.print_move();  
            std::cout << "\n";
            
            // Make the engine move
            game.make_move(engine_move);

            // Print board
            game.print_board();         
        }
    }
    else
    {
        if (side == 'b' || side == 'B')
        {
            while(is_running()) {
                // Same as before but reversed since now the engine
                // plays with white
                Move engine_move = search_position(6);

                // Make the engine move
                game.make_move(engine_move);

                game.print_board();
                std::cout << "Engine played: ";
                engine_move.print_move();
                std::cout << "\n";

                Move player_move = get_player_move(black);

                game.make_move(player_move);
            }
        }
        else
            std::cout << "[ERROR] Chose a valid side\n";
    }   
}

bool Engine::is_running() {
    return true;
}

Move Engine::get_player_move(int colour) {
    std::string string_move;

    std::cout << "Insert the move you want to play (Piece + starting square + landing square):\n";
    std::cin >> string_move;

    Move player_move(string_move, colour);

    return player_move;
}

Move Engine::search_position(int depth) {

    int alpha = -50000, beta = 50000;

    flk::clear_tables();

    for(int current_depth = 1; current_depth <= depth; current_depth++)
        flk::negamax(game, current_depth, alpha, beta);    

    return flk::pv_table[0][0];
}