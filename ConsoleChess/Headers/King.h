#pragma once
#include "BoardItem.h"

class King: public BoardItem
{
    public:
        void setName() {
            this -> name = "KING";
        }    
};
