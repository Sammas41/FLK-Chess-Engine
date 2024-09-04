#ifndef SEARCH_H
#define SEARCH_H

#include "game.h"
#include "evaluation.h"
#include "moveGenerator.h"

namespace flk {

    extern int nodes;
    
    void perft_search(Game&, int, std::vector<int>&);
    void Perft(Game&, int);

    int Negamax(Game&, int, int, int, int&);
    int Quiescence_search(Game&, int, int);
}

#endif