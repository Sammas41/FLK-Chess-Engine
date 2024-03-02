#ifndef SEARCH_H
#define SEARCH_H

#include "game.h"
#include "evaluation.h"
#include "moveGenerator.h"

namespace flk {

    extern int nodes;

    int negamax(Game*, int, int, int, int&);
}

#endif