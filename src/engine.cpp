#include "engine.h"

// Constructors:
// Default
Engine::Engine() : game() {
    flk::init_all_attacks();
	flk::init_evaluation_masks();
}

// From a chosen position
Engine::Engine(std::string fen) : game(fen) {
    flk::init_all_attacks();
	flk::init_evaluation_masks();
}

// Start the engine
void Engine::run() {
    
    make_header();

    // Main loop
    while (close_engine == false) {
        // Read input from the command line
        read();
    }
    
    std::cout << "Thanks for playing\n";
}

// Game loop
void Engine::play() {   

    // To store game states for undo functionality
    std::vector<Game> game_history;
    game_history.push_back(game);
    game_over = false;

    // Choose the side
    char side;
    std::cout << "Choose a color [w/b]:\n";
    std::cin >> side;

    // This line is necessary due to the fact that std::getline is also used
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (side == 'w' || side == 'W') {

        // Print board
        std::cout << "\n";
        game.print_board();

        // Game loop
        while (!game_over) { 
            
            InputResult result = process_input(white, game_history);

            if (!result.is_move) {
                // If it's not a move (i.e., it was an undo), continue the loop
                continue;
            }

            // Make player's move
            game.make_move(result.move);
            game_history.push_back(game);
            
            // Check for checkmate or stalemate after player's move
            if (is_mate(game)) {
                game.print_board();
                break;
            }

            // Let the engine find the best move
            flk::BestLine engine_line = search_position();
            
            // Make the engine move
            game.make_move(engine_line.best_move);
            game_history.push_back(game);

            // Print board and engine move
            game.print_board();
            print_engine_line(engine_line);

            // Check for checkmate or stalemate after engine's move
            if (is_mate(game))
                break;       
        }
    }
    else if (side == 'b' || side == 'B') {

        // Game loop
        while (!game_over) {

            // Let the engine find the best move
            flk::BestLine engine_line = search_position();

            // Make the engine move
            game.make_move(engine_line.best_move);
            game_history.push_back(game);

            // Print the board and the engine move
            game.print_board();
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
            if (is_mate(game)) {
                game.print_board();
                break;
            }
        }
    }
    else {
        std::cout << "Invalid side. Select 'w' for white or 'b' for black\n";
        return;
    }

    // reset the board to its initial state and clear history
    game = game_history.at(0);
    game_history.clear();
}

// Read the user inputs
std::string Engine::take_input() {
    std::string input;
    std::cout << "Insert the move you want to play (Piece + starting square + landing square):\n";
    std::cin >> input;

    // This line is necessary due to the fact that std::getline is also used
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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

// Returns the user move
Move Engine::create_move(std::string input, int colour) {
    return Move(input, colour);
}

// Checks if the move inserted is among the legal moves
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

// Undo the played moves
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

// Obtain the engine best line in this position
flk::BestLine Engine::search_position() {
    return flk::iterative_search(game, depth, max_search_time);
} 

// Checks if it is mate
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

// Print the engine line
void Engine::print_engine_line(flk::BestLine line) {
    // Two possibilites:
    //   - just the print the engine move
    //   - print all the info of the search 
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
        for(int i = 0; i < line.depth_reached; i++) {
            
            // If we have found a mate print until the mating move
            if(line.pv_line[i] == NULL_MOVE)
                break;
            
            line.pv_line[i].print_move();
            std::cout << " ";
        }

        // Print evaluation
        double eval = static_cast<double>(line.evaluation) / 100;

        // If it white side to move then engine has black (and viceversa) 
        // so we fix the + and - sign according to the usual convention and
        // not form the engine perspective
        if(game.get_side() == white)
            (eval > 0) ? std::cout << "Evaluation: -" : std::cout << "Evaluation: +";
        else
            (eval > 0) ? std::cout << "Evaluation: +" : std::cout << "Evaluation: -";

        std::cout << std::setprecision(2);
        (eval >= 0) ? std::cout << eval << "\n" : std::cout << -eval <<  "\n";

        // Print nodes searched, depth reached, search time
        std::cout << "Nodes searched: " << line.nodes_visited 
                  << "  Depth reached: " << line.depth_reached
                  << std::setprecision(4)
                  << "  Search time: " << line.search_time << "\n";
    }
}

// Reads the user input
void Engine::read() {
    
    // Clear the previous command
    c_line.clear();
    
    std::cout << ">> ";

    // Get the new command
    c_line.read_command();

    // Set command
    if(c_line.get_command() == "set") {
        run_set_command();
        return;
    }

    // Print command
    if(c_line.get_command() == "print") {
        run_print_command();
        return;
    }

    // Play command
    if(c_line.get_command() == "play") {
        play();
        return;
    }

    // Quit command
    if(c_line.get_command() == "quit" || c_line.get_command() == "q") {
        close_engine = true;
        return;
    }

    // Help command
    if(c_line.get_command() == "help") {
        print_help();
        return;
    }

    // Null command
    if(c_line.get_command() == "") {
        return;
    }

    std::cout << "Invalid command\n";
}

// Process the user command
void Engine::run_set_command() {
    // Max depth
    if(c_line.get_specifier(0) == "maxd" && isdigit(c_line.get_specifier(1).at(0))) {
        std::string s = c_line.get_specifier(1);
        depth = std::stoi(s);
        std::cout << "Max depth set to: " << s << "\n";
        return;
    }

    // Max time search
    if(c_line.get_specifier(0) == "maxst" && isdigit(c_line.get_specifier(1).at(0))) {
        std::string s = c_line.get_specifier(1);
        max_search_time = std::stoi(s);
        std::cout << "Max search time set to: " << s << " sec\n";
        return;
    }

    // Search info
    if(c_line.get_specifier(0) == "info" || c_line.get_specifier(0) == "-i") {
        std::cout << "Engine now prints all information on the search\n";
        all_info = true;
        return;
    }

    std::cout << "Invalid command\n";
}

void Engine::run_print_command() {
    // Print engine settings
    if(c_line.get_specifier(0) == "settings" || c_line.get_specifier(0) == "-s") {
        print_settings();
        return;
    }

    // Print current board
    if(c_line.get_specifier(0) == "board" || c_line.get_specifier(0) == "-b") {
        game.print_board();
        return;
    }

    std::cout << "Invalid command\n";
}

void Engine::print_settings() {
    std::cout << "Current settings:\n";
    std::cout << "  Max search depth: " << depth << "\n";
    std::cout << "  Max search time: " << max_search_time << " sec\n"; 
}

void Engine::print_help() {
    std::cout << "Command: set\n";
    std::cout << "   ... maxd [int number]   --> set maximum depth at [int number]\n";
    std::cout << "   ... maxst [int number]  --> set maximum search time at [int number] seconds\n";
    std::cout << "   ... info                --> set the all_info flag\n";

    std::cout << "\nCommand: print\n";
    std::cout << "   ... settings            --> print engine's settings\n";
    std::cout << "   ... board               --> print the board\n";

    std::cout << "\nCommand: play\n";
    std::cout << "   ...                     --> start a new game\n";

    std::cout << "\nCommand: quit (or just q)\n";
    std::cout << "   ...                     --> quit the application\n";
}

void Engine::make_header() {

    std::cout << "------------------------------------------------\n";
    std::cout << "         ***********   **          **    ***    \n";
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

    std::cout << "Type help for more information on the commands\n";
}
