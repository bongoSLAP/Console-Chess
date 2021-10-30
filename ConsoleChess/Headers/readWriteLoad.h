std::string boolToString(bool b) {
  return b ? "true" : "false";
} 

void updateSave(std::vector<std::vector<BoardItem>> board, bool isDarkTurn) {
    std::ofstream save(currentFile, std::ofstream::out | std::ofstream::trunc); //trunc deletes contents
    
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            save << board[i][j].name + "." + boolToString(board[i][j].isDark) + " ";
        }

        save << "\n";
    }

    for (int i = 0; i < takenPieces.size(); i++) {
        for (int j = 0; j < takenPieces[i].size(); j++) {
            save << takenPieces[i][j] + " ";
        }

        save << "\n";
    } 

    save << boolToString(isDarkTurn);
    save.close();
}

void newSave(std::vector<std::vector<BoardItem>> board, bool isDarkTurn) {
    time_t present = time(0);
    char* presentDate = ctime(&present);
    std::string dateString(presentDate);

    currentFile = "ConsoleChess/SaveFiles/" + dateString + ".txt";
    std::ofstream save(currentFile);
    save.close();

    updateSave(board, isDarkTurn);
}

std::vector<std::vector<std::string>> readFile2d(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::vector<std::string>> entries2d;
  
    while(getline(file, line)) {
        std::stringstream stream(line);
        std::string field;
        std::vector<std::string> rowList;

        while(getline(stream, field, ' ')) {
            rowList.push_back(field);
        }

        entries2d.push_back(rowList);        
    }

    file.close();
    return entries2d;
}

std::vector<std::filesystem::path> getPathsInFolder() {
    std::vector<std::filesystem::path> fileVector;

    for (const auto & file : std::filesystem::directory_iterator("ConsoleChess/SaveFiles/")) {
        std::filesystem::path path = file.path();
        fileVector.push_back(path);
    }

    return fileVector;
}

std::vector<std::vector<BoardItem>> loadBoard(std::vector<std::vector<BoardItem>> board, std::vector<std::vector<std::string>> dataGrid) {
    for (int i = 0; i < squaresOnAxes; i++) {
        std::vector<BoardItem> row;

        for (int j = 0; j < squaresOnAxes; j++) {
            BoardItem boardItem;
            boardItem.setName(dataGrid[i][j].substr(0, 4));

            if (dataGrid[i][j].substr(5) == "true")
                boardItem.isDark = true;
            else
                boardItem.isDark = false;

            boardItem.setIcon();
            row.push_back(boardItem);
        }
                                    
        board.push_back(row);
    }

    board = assignPositions(board);
    return board;
}