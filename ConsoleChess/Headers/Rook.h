#pragma once

class Rook: public BoardItem
{
    public:
        void setName() {
            this -> name = "ROOK";
        }

        void setIcon() {
            if (this -> isDark) {
                this -> icon = "♖";
                return;
            }

            this -> icon = "♜";
        }
};
