#ifndef ENGINE_H
#define ENGINE_H

#include "game.h"
#include "moveGenerator.h"
#include "search.h"
#include "commandLine.h"
#include <iomanip>
#include <cctype>

struct InputResult {
    bool is_move;
    Move move;
};

class Engine {
    public:
        // Constructors 
        Engine();
        Engine(std::string);

        // Start the engine
        void run();

    private:
        Game game;
        CommandLine c_line;

        // Engine settings
        int depth = 8;
        int max_search_time = 5;
        bool all_info = false;

        // State variables
        bool game_over = false;
        bool close_engine = false;
        const std::string VERSION = "1.0";

        // Read user input during game
        std::string take_input();
        InputResult process_input(int colour, std::vector<Game>& game_history);
        Move create_move(std::string input, int colour);

        // Perform actions during game
        void takeback();
        void undo(int steps, std::vector<Game>& game_history);
        bool is_mate(Game&);
        bool is_legal_move(Move& move);

        // Menu functions
        void read();
        void play();
        void make_header();
        void run_set_command();
        void run_print_command();
        void print_settings();
        void print_help();

        // Engine search functions
        flk::BestLine search_position();
        void print_engine_line(flk::BestLine);
};

#endif