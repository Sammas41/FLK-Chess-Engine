#include "engine.h"

Engine::Engine() : game() {
    
}

Engine::Engine(std::string fen) : game(fen) {
    
}

void Engine::play() {
    bool game_over = false;
    std::vector<Game> game_history;  // To store game states for undo functionality
    game_history.push_back(game); // Store initial position

    make_header();

    char side;
    std::cout << "Choose a color [w/b]:\n";
    std::cin >> side;

    while (!game_over) {
        if (side == 'w' || side == 'W') {
            // Print board
            std::cout << "\n";
            game.print_board();

            while (is_running()) { 
                
                InputResult result = process_input(white, game_history);

                if (!result.is_move) {
                    // If it's not a move (i.e., it was an undo), continue the loop
                    continue;
                }

                // Make player's move
                game.make_move(result.move);
                game_history.push_back(game);
                
                // Check for checkmate or stalemate after player's move
                if (is_mate(game))
                    break;

                // Let the engine find the best move
                flk::BestLine engine_line = search_position();
                
                // Make the engine move
                game.make_move(engine_line.best_move);
                game_history.push_back(game);

                // Check for checkmate or stalemate after engine's move
                if (is_mate(game))
                    break;

                // Print board
                game.print_board();
                
                std::cout << "Eval before the func: " << engine_line.evaluation << "\n";

                // Output the engine move
                print_engine_line(engine_line);       
            }
        }
        else if (side == 'b' || side == 'B') {
            while (is_running()) {
                // Let the engine find the best move
                flk::BestLine engine_line = search_position();

                // Make the engine move
                game.make_move(engine_line.best_move);
                game_history.push_back(game);

                // Print the board
                game.print_board();
                
                // Print the engine move
                print_engine_line(engine_line);

                // Check for checkmate or stalemate after engine's move
                if (is_mate(game))
                    break;

                InputResult result = process_input(black, game_history);

                if (!result.is_move) {
                    // If it's not a move (i.e., it was an undo), continue the loop
                    continue;
                }

                game.make_move(result.move);
                game_history.push_back(game);

                // Check for checkmate or stalemate after player's move
                if (is_mate(game))
                    break;
            }
        }
        else {
            std::cout << "[ERROR] Chose a valid side\n";
            return;
        }

        if (game_over) {
            char choice;
            std::cout << "Do you want to play another game? (y/n): ";
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                game = Game(); // Reset the game to initial position
                game_over = false;
                std::cout << "Starting a new game...\n";
            } else {
                std::cout << "Thanks for playing!\n";
                return;
            }
        }
    }   
}

bool Engine::is_running() {
    return game_over;
}

std::string Engine::take_input() {
    std::string input;
    std::cout << "Insert the move you want to play (Piece + starting square + landing square):\n";
    std::cin >> input;
    return input;
}

InputResult Engine::process_input(int colour, std::vector<Game>& game_history) {
    while (true) {
        std::string input = take_input();

        if (input == "undo") {
            undo(1, game_history);
            return {false, Move()};
        } else if (input.substr(0, 5) == "undo-") {
            int index = std::stoi(input.substr(5));
            undo(index, game_history);
            return {false, Move()};
        }

        Move move = create_move(input, colour);
        if (is_legal_move(move)) {
            return {true, move};
        } else {
            std::cout << "Invalid move. Please try again.\n";
        }
    }
}

Move Engine::create_move(std::string input, int colour) {
    return Move(input, colour);
}

bool Engine::is_legal_move(Move& move) {
    MoveGenerator move_generator(game);
    MoveArray legal_moves = move_generator.generate_moves(all_moves);

    for (int i = 0; i < legal_moves.count; i++) {
        if (move == legal_moves.move_list[i]) {
            return true;
        }
    }
    return false;
}

void Engine::undo(int steps, std::vector<Game>& game_history) {
    steps = steps * 2;
    for (int i = 0; i < steps && game_history.size() > 1; i++) {
        game_history.pop_back();
    }
    if (!game_history.empty()) {
        game = game_history.back();
        std::cout << "Moved back " << steps << " step(s). Current position:\n";
        game.print_board();
    } else {
        std::cout << "Cannot undo any further.\n";
    }
}

flk::BestLine Engine::search_position() {
    return flk::iterative_search(game, depth, max_search_time);
} 

bool Engine::is_mate(Game& game) {
    MoveGenerator move_generator(game);
    MoveArray legal_moves = move_generator.generate_moves(all_moves);

    if (legal_moves.count == 0) {
        if (flk::is_check(game)) {
            std::cout << "Checkmate! " << (game.get_side() == white ? "Black" : "White") << " wins.\n";
        } else {
            std::cout << "Stalemate! The game is a draw.\n";
        }
        game_over = true;
        return true;
    }
    return false;
}

void Engine::print_engine_line(flk::BestLine& line) {

    if(all_info == false) {
        std::cout << "Engine played: ";
        line.best_move.print_move();  
        std::cout << "\n";
    }
    else {
        // Print the best move
        std::cout << "Engine played: ";
        line.best_move.print_move();  
        std::cout << "\n";

        // Print the pv line and evaluation
        for(int i = 0; i < line.pv_line_length; i++) {
            line.pv_line[0][i].print_move();
            std::cout << " ";
        }

        std::cout << "Evaluation: " << std::setprecision(1)
                  << static_cast<double>(line.evaluation) / 100 <<  "\n";
        std::cout << "Nodes searched: " << line.nodes_visited 
                  << "  Depth reached: " << line.depth_reached
                  << std::setprecision(4)
                  << "  Search time: " << line.search_time << "\n";
    }
}

void Engine::make_header() {
    std::cout << "------------------------------------------------\n";
    std::cout << "         ***********   **          **    ***   \n";
    std::cout << "        ***********   **          **   ***      \n";
    std::cout << "       **            **          ** ***         \n";
    std::cout << "      *******       **          ****            \n";
    std::cout << "     *******       **          ****             \n";
    std::cout << "    **            **          ** ***            \n";
    std::cout << "   **            ********    **   ***           \n";
    std::cout << "  **            ********    **     ***          \n";
    std::cout << "------------------------------------------------\n";
    std::cout << "         Fried Liver King Chess Engine          \n";
    std::cout << "     By Nicola Schiavo and Samuele Berdusco     \n";
    std::cout << "               Version: " + VERSION + "\n\n";
}