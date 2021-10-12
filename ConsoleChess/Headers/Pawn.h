#pragma once
#include "Piece.h"

class Pawn: public Piece
{
    public:
        void setName() {
            this -> name = "PAWN";
        }
};
