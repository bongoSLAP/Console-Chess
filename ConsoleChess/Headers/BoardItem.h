#pragma once
#include <string>

class BoardItem
{
	public:
		std::string name;
		std::string icon;
        std::string position;
        bool isDark;

        int translateXToInt(std::string xAxisLetter) {
            std::vector<std::string> xAxisLabels = {"a", "b", "c", "d", "e", "f", "g", "h"};

            for (int i = 0; i < xAxisLabels.size(); i++) {
                if (xAxisLabels[i] == xAxisLetter) {
                    return i;
                }
            }

            return -1;
        }

        std::pair <int, int> createColumnVector(std::string desiredPosition) {
            std::pair <int, int> columnVector;
            std::pair <int, int> currentCoordinates;
            std::pair <int, int> desiredCoordinates;

            currentCoordinates.first = translateXToInt(this -> position.substr(0, 1));
            currentCoordinates.second = stoi(this -> position.substr(1, 1));

            desiredCoordinates.first = translateXToInt(desiredPosition.substr(0, 1));
            desiredCoordinates.second = stoi(desiredPosition.substr(1, 1));

            columnVector.first = desiredCoordinates.first - currentCoordinates.first;
            columnVector.second = desiredCoordinates.second - currentCoordinates.second;
            
            return columnVector;
        };
        
        
};
