#pragma once

class Bishop: public BoardItem
{
    public:
        void setName() {
            this -> name = "BSHP";
        }

        void setIcon() {
            if (this -> isDark) {
                this -> icon = "♝";
                return;
            }

            this -> icon = "♗";
        }
};
