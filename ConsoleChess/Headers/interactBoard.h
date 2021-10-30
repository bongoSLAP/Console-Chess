std::pair<int, int> findIndexInVector(std::vector<std::vector<BoardItem>> board, std::string position) {
    std::pair<int, int> indices;
    indices.first = -1;
    indices.second = -1;

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j].position == position) {
                indices.first = i; //y
                indices.second = j; //x

                return indices;
            }
        }
    }

    return indices;
}

std::vector<std::vector<BoardItem>> swap(std::vector<std::vector<BoardItem>> board, std::string positionFrom, std::string positionTo) {
    std::pair<int, int> indexFrom = findIndexInVector(board, positionFrom);
    std::pair<int, int> indexTo = findIndexInVector(board, positionTo);
    BoardItem temp = board[indexTo.first][indexTo.second];
    
    board[indexTo.first][indexTo.second] = board[indexFrom.first][indexFrom.second];
    board[indexTo.first][indexTo.second].position = positionTo;

    board[indexFrom.first][indexFrom.second] = temp;
    board[indexFrom.first][indexFrom.second].position = positionFrom;

    return board;
}

std::vector<std::vector<BoardItem>> take(std::vector<std::vector<BoardItem>> board, std::string positionFrom, std::string positionTo) {
    std::pair<int, int> indexFrom = findIndexInVector(board, positionFrom);
    std::pair<int, int> indexTo = findIndexInVector(board, positionTo);
    
    BoardItem empty;
    empty.setName("EMTY");
    empty.setIcon();
    
    board[indexTo.first][indexTo.second] = board[indexFrom.first][indexFrom.second];
    board[indexTo.first][indexTo.second].position = positionTo;

    board[indexFrom.first][indexFrom.second] = empty;
    board[indexFrom.first][indexFrom.second].position = positionFrom;

    return board;
}

std::vector<std::vector<BoardItem>> assignPositions(std::vector<std::vector<BoardItem>> board) {
    std::vector<std::string> xAxisLabels = {"a", "b", "c", "d", "e", "f", "g", "h"};
    std::vector<std::string> yAxisLabels = {"8", "7", "6", "5", "4", "3", "2", "1"}; //backwards to assign smallest from bottom to top


    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            board[i][j].position = xAxisLabels[j] + yAxisLabels[i];
        }
    }

    return board;
}

int findLargest(std::pair<int, int> vector) {
    if ((std::abs(vector.first) == std::abs(vector.second)) || (std::abs(vector.first) > std::abs(vector.second))) {
        return std::abs(vector.first);
    }

    return std::abs(vector.second);
}

bool isPositive(int toFind) {
    if (toFind > 0) 
        return true;
    else
        return false;
}

bool stepThrough(std::pair<int, int> currentPosition, std::pair<int, int> desiredPosition, std::pair<int, int> vector, std::vector<std::vector<BoardItem>> board) {
    int largestAbsolute = findLargest(vector);
    int stepX = 0;
    int stepY = 0;
    
    for (int i = 0; i < largestAbsolute; i++) {
        if (vector.first != 0 && vector.first != stepX) {
            if (isPositive(vector.first)) 
                stepX ++;
            else 
                stepX --;
        }

        if (vector.second != 0 && vector.second != stepY) {
            if (isPositive(vector.second)) 
                stepY ++;
            else 
                stepY --;
        }

        if (currentPosition.second + stepX != desiredPosition.second || currentPosition.first - stepY != desiredPosition.first) {
            if (board[currentPosition.first - stepY][currentPosition.second + stepX].name != "EMTY") {
                return false;
            }
        }
        else if (currentPosition.second + stepX == desiredPosition.second && currentPosition.first - stepY == desiredPosition.first) {
            return true;
        }
    }
}