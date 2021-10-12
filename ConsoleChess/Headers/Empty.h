#pragma once

class Empty: public BoardItem
{
    public:
        void setName() {
            this -> name = "EMTY";
        }

        void setIcon() {
            this -> icon = " ";
        }
};
