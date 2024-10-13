#ifndef ENGINE_H
#define ENGINE_H

#include "game.h"
#include "moveGenerator.h"
#include "search.h"
#include <iomanip>

struct InputResult {
    bool is_move;
    Move move;
};

class Engine {
    public:
        // Constructors 
        Engine();
        Engine(std::string);

        //
        void play();
        void takeback();
        void undo(int steps, std::vector<Game>& game_history);

        bool is_mate(Game&);
        std::string take_input();
        InputResult process_input(int colour, std::vector<Game>& game_history);

        Move create_move(std::string input, int colour);
        bool is_legal_move(Move& move);
    private:
        Game game;

        int depth = 8;
        int max_search_time = 5;
        
        bool game_over = false;
        bool all_info = true;

        const std::string VERSION = "1.0";

        void make_header();
        void print_engine_line(flk::BestLine);

        bool is_running();
        Move get_player_move(int, std::string);
        flk::BestLine search_position();
};

#endif