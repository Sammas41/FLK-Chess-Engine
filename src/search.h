#ifndef SEARCH_H
#define SEARCH_H

#include "evaluation.h"
#include "moveGenerator.h"
#include <vector>
#include <chrono>

namespace flk {
    
    // Search constants
    constexpr int MAX_PV_LENGTH = 64;
    constexpr int FLK_INFINITY = 50000;
    constexpr int WINDOW_VAL = 50;
    constexpr int FULL_DEPTH_MOVES = 4;
    constexpr int REDUCTION_LIMIT = 3;
    
    // Contains all the information regarding the best line
    struct BestLine {
        Move pv_line[MAX_PV_LENGTH][MAX_PV_LENGTH];
        Move best_move;

        int pv_line_length;
        int depth_reached;
        int nodes_visited;
        int evaluation;
        double search_time;
    };

    extern int nodes;

    // Perf test functions, useful for debugging the move
    // generation process
    void perft_search(Game&, int, std::vector<int>&);
    void Perft(Game&, int);

    // Search functions
    int negamax(Game&, int, int, int);
    int quiescence_search(Game&, int, int);
    BestLine iterative_search(Game&, int, int);

    // Score move functions
    int score_move(Move, Game&);
    void sort_moves(MoveArray&, Game&);
    void enable_pv_score(MoveArray&);
    bool ok_to_reduce(Move, int);
    bool is_check(Game&);

    // Clear tables from previous iterations
    void clear_tables();

    // Print functions
    void print_score(MoveArray&, Game&);

    // This table is used to sort the list of captures such
    // that it searches first the most favourable captures
    // (ie a pawn capturing a piece). In this way it is more
    // likely that alpha-beta pruning cuts off much of the
    // searching tree. The first index represents the attacking
    // piece while the second index represents the piece targeted.
    //
    // Example:
    //  - pawn takes queen: MVV_LVA[P][q] = 505 --> good capture,
    //    high probability to prune other branches, should be searched
    //    first
    //  - queen takes pawn: MVV_LVA[Q][p] = 101 --> bad capture,
    //    low probability to prune other branches, should be searched last                                       
    const int MVV_LVA[12][12] = {
        {105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605},
        {104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604},
        {103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603},
        {102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602},
        {101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601},
        {100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600},

        {105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605},
        {104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604},
        {103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603},
        {102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602},
        {101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601},
        {100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600}
    };
}

#endif

