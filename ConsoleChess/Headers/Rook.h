#pragma once
#include "BoardItem.h"

class Rook: public BoardItem
{
    public:
        void setName() {
            this -> name = "ROOK";
        }
};
