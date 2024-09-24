#ifndef SEARCH_H
#define SEARCH_H

#include "evaluation.h"
#include "moveGenerator.h"
#include <vector>

namespace flk {

    extern int nodes;

    // Killer moves matrix (2 killer moves x ply)
    // Killer moves are stored up to depth = MAX_KILLER_DEPTH
    extern Move killer_moves[2][MAX_KILLER_DEPTH];

    // History moves scores (12 pieces x 64 squares)
    extern int history_moves[PIECE_TYPES][SQUARES];
    
    // Triangular principal variation table
    extern Move pv_table[MAX_PV_LENGTH][MAX_PV_LENGTH];

    // Stores the length of the pv at each ply
    extern int pv_length[MAX_PV_LENGTH];

    // Perf test functions, useful for debugging the move
    // generation procedure
    void perft_search(Game&, int, std::vector<int>&);
    void Perft(Game&, int);

    // Search functions
    int negamax(Game&, int, int, int);
    int quiescence_search(Game&, int, int);
}

#endif

