#pragma once

class Queen: public BoardItem
{
    public:
        void setName() {
            this -> name = "QUEN";
        }
        
        void setIcon() {
            if (this -> isDark) {
                this -> icon = "♛";
                return;
            }

            this -> icon = "♕";
        }
};
