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
            flk::negamax(game, 6, -50000, 50000);
            game.make_move(flk::pv_table[0][0]);

            // Output the engine move
            std::cout << "Engine played: ";
            flk::pv_table[0][0].print_move();  
            std::cout << "\n";

            //Print board
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
                flk::negamax(game, 6, -50000, 50000);
                game.make_move(flk::pv_table[0][0]);

                game.print_board();
                std::cout << "Engine played: ";
                flk::pv_table[0][0].print_move();
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
// crea movegenerator da game, -> feneratemoveall() ritorna move array.
// prendi mosse legali 

Move Engine::get_player_move(int colour) {
    std::string string_move;
    MoveGenerator move_generator(game);
    MoveArray legal_moves = move_generator.generate_moves(all_moves);

    Move player_move(string_move, colour);

    while (true) {
        std::cout << "Insert the move you want to play (Piece + starting square + landing square):\n";
        std::cin >> string_move;

        Move player_move(string_move, colour);

        // Check if the move is legal
        bool is_legal = false;
        for (int i = 0; i < legal_moves.count; i++) {
            if (player_move == legal_moves.move_list[i]) {
                is_legal = true;
                break;
            }
        }

        if (is_legal) {
            return player_move;
        } else {
            std::cout << "Invalid move. Please try again.\n";
        }
    }

}