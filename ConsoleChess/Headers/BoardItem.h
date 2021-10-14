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

        std::pair <int, int> generateCoordinates(std::string position) {
            std::pair <int, int> coordinates;

            coordinates.first = translateXToInt(position.substr(0, 1));
            coordinates.second = stoi(position.substr(1, 1));

            return coordinates;
        }

        std::pair <int, int> createColumnVector(std::string desiredPosition) {
            std::pair <int, int> columnVector;
            std::pair <int, int> currentCoordinates = generateCoordinates(this -> position);
            std::pair <int, int> desiredCoordinates = generateCoordinates(desiredPosition);

            columnVector.first = desiredCoordinates.first - currentCoordinates.first;
            columnVector.second = desiredCoordinates.second - currentCoordinates.second;
            
            return columnVector;
        };      
};
