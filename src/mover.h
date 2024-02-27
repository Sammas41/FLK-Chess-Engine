#ifndef MOVER_H
#define MOVER_H

#include "moveGenerator.h"

class Mover {
public:
    static unsigned int encodeMove(unsigned int source, unsigned int target, unsigned int piece, unsigned int promoted, unsigned int capture, unsigned int doublePush, unsigned int enpassant, unsigned int castling);
    static unsigned int getMoveSource(unsigned int move);
    static unsigned int getMoveTarget(unsigned int move);
    static unsigned int getMovePiece(unsigned int move);
    static unsigned int getMovePromoted(unsigned int move);
    static unsigned int getMoveCapture(unsigned int move);
    static unsigned int getMoveDouble(unsigned int move);
    static unsigned int getMoveEnpassant(unsigned int move);
    static unsigned int getMoveCastling(unsigned int move);
};

#endif