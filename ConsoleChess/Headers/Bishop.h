#pragma once

class Bishop: public BoardItem
{
    public:
        void setName() {
            this -> name = "BSHP";
        }

        void setIcon() {
            if (this -> isDark) {
                this -> icon = "♗";
                return;
            }

            this -> icon = "♝";
        }

        bool validateVector(std::pair<int, int> vector) {
            if (std::abs(vector.first) == std::abs(vector.second)) {
                return true;
            }

            return false;
        }
};
