#ifndef ENGINE_H
#define ENGINE_H

#include "game.h"
#include "moveGenerator.h"
#include "search.h"

struct InputResult {
    bool is_move;
    Move move;
};

class Engine {
    public:
        Engine();
        Engine(std::string);

        void play(char);
        void takeback();
        bool is_mate(Game&);
        std::string take_input();
        InputResult process_input(int colour, std::vector<Game>& game_history);
        Move create_move(std::string input, int colour);
        bool is_legal_move(Move& move);
        void undo(int steps, std::vector<Game>& game_history);
    private:
        Game game;

        bool is_running();
        Move get_player_move(int, std::string);
        Move search_position(int);
};

#endif