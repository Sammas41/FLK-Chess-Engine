#ifndef SEARCH_H
#define SEARCH_H

#include "evaluation.h"
#include "moveGenerator.h"
#include <vector>

namespace flk {

    extern int nodes;
    extern Move killer_moves[2][100];

    // History moves matrix for each piece (12 pieces x 64 squares)
    extern int history_moves[12][SQUARES];
    
    void perft_search(Game&, int, std::vector<int>&);
    void Perft(Game&, int);

    int negamax(Game&, int, int, int, Move&);
    int quiescence_search(Game&, int, int);
}

#endif

