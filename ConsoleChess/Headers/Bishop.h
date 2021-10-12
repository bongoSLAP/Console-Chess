#pragma once
#include "Piece.h"

class Bishop: public Piece
{
    public:
        void setName() {
            this -> name = "BSHP";
        }
};
