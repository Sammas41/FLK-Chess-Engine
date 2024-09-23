#include "engine.h"

Engine::Engine() : game() {
    
}

Engine::Engine(std::string fen) : game(fen) {
    
}

void Engine::play(char side) {

    if(side == 'w' || side == 'W')
    {
        while(is_running())
        {
            // Print board
            std::cout << "\n";
            game.print_board();

            // Read player's move
            Move player_move = get_player_move(white);

            // Make player's move
            game.make_move(player_move);

            Move engine_move;
            flk::negamax(game, 6, -50000, 50000, engine_move);
            game.make_move(engine_move);

            std::cout << "Engine played: ";
            engine_move.print_move();           
        }
    }
    else
    {
        if (side == 'b' || side == 'B')
        {
            Move player_move = get_player_move(black);
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