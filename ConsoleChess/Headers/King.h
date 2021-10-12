#pragma once

class King: public BoardItem
{
    public:
        void setName() {
            this -> name = "KING";
        }    

        void setIcon() {
            if (this -> isDark) {
                this -> icon = "♚";
                return;
            }

            this -> icon = "♔";
        }
};
