#pragma once

class Knight: public BoardItem
{
    public:
        void setName() {
            this -> name = "KNHT";
        }   

        void setIcon() {
            if (this -> isDark) {
                this -> icon = "♘";
                return;
            }
            
            this -> icon = "♞";
        }
};
