#pragma once
#include "Piece.h"

class King: public Piece
{
    public:
        void setName() {
            this -> name = "KING";
        }    
};
