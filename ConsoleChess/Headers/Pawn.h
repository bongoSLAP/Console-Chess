#pragma once
#include "BoardItem.h"

class Pawn: public BoardItem
{
    public:
        void setName() {
            this -> name = "PAWN";
        }
};
