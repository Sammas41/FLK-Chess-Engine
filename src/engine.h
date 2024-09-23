#ifndef ENGINE_H
#define ENGINE_H

#include "game.h"
#include "moveGenerator.h"
#include "search.h"

class Engine {
    public:
        Engine();
        Engine(std::string);

        void play(char);
    private:
        Game game;

        bool is_running();
        Move get_player_move(int);
};

#endif