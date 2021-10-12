#pragma once

class Pawn: public BoardItem
{
    public:
        void setName() {
            this -> name = "PAWN";
        }

        void setIcon() {
            if (this -> isDark) {
                this -> icon = "♟";
                return;
            }

            this -> icon = "♙";
        }
};
