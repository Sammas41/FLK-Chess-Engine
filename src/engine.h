#ifndef ENGINE_H
#define ENGINE_H

#include "game.h"
#include "moveGenerator.h"

class Engine {
    public:
        Engine();
        Engine(std::string);

        void play(char);
    private:
        Game game;

        bool is_running();
        int get_player_move(int);
        int string_to_int(std::string, int);

};

#endif