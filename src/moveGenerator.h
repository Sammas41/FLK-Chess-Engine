#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "attacks.h"
#include "game.h"
#include <memory>

class MoveGenerator
{
    Game game;
    std::unique_ptr<Attacks> ptr_attacks;

    public:
        MoveGenerator() = delete;   // Cannot initialize MoveGenerator without a game
        MoveGenerator(Game &);      // Default constructor

        int is_square_attacked(int, int);
        void print_attacked_squares(int);

        void generate_moves();
};

#endif