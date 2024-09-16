#ifndef SEARCH_H
#define SEARCH_H

#include "evaluation.h"
#include "moveGenerator.h"
#include <vector>

namespace flk {

    extern int nodes;
    
    void perft_search(Game&, int, std::vector<int>&);
    void Perft(Game&, int);

    int negamax(Game&, int, int, int, Move&);
    int quiescence_search(Game&, int, int);
}

#endif

