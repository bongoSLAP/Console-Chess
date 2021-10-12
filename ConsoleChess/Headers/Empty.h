#pragma once
#include "BoardItem.h"

class Empty: public BoardItem
{
    public:
        void setName() {
            this -> name = "EMTY";
        }
};
