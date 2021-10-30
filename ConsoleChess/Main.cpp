/*
TODO (in order):
    + pawn takes
        - if pawn move is at a one-away diagonal infront of an enemy piece, then take this piece

    + pawn promotion to either bishop, knight, rook or queen when it reaches opposing x axis of board
        - after every pawn move check to see whether the piece moved is in the last opposing row of 2d board vector (if dark pawn, board[7], if light pawn, board[0])
        - if so, output a menu to select from above 4 pieces
        - replace pawn with selected piece of same colour and then move to opponents turn (no extra turns after promotion)
        ! required because fundamental part of game

    + peasant revolt game mode
        - https://www.chessvariants.com/large.dir/peasantrevolt.html
        - quite easy to do as i only have to change board layout, no rules need to be changed
        - add menu option
        ~ not required but would be good to add variety

    + add castling condition
        - to castle there have to be no pieces between the King and the Rook on the side youre castling on.
        ~ not as required as it was added to chess much later for the purpose of speeding up games

    + add option to resign
        - after each move give the user the option to input a string that declares they want to resign
        - halt game loop and assert opponent as winner if they resign
        ~ kind of required but player can just physically give up and close window

    + work out when king is in check
        - use vectors + stepthrough to figure out and notify player when king is in check
        - maybe highlight some things in red when king is in check
        ~ would be helpful but not technically required as this does not exist in real life chess, may be too complex for time i have left

    + flip board each turn 
        - flip the board so that each player is making their moves from the perspective of the bottom of the board
        - this is mainly so that dark has 
    
    + work out when the game is stalemate (draw)
        - use vectors + stepthrough to figure out end game in stalemate
        x probably too complex for time i have left
    
    + add timer
        x not sure how to do this as i would need to update console each second which would clear inputs each second
        x probably a way to do this without losing input idk
*/

#include <iostream>
#include <string> 
#include <vector>
#include <variant>
#include <regex>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <ctime>
#include <filesystem>

#include "Headers/BoardItem.h"

const int squaresOnAxes = 8;
const int gridSpacing = 6;
const int xWidth = 8;
const int yLength = 3;
const int yLineOverflow = 9; //the vertical lines displace x axis by 9 spaces
const int pieceOverflow = 1; //each board piece displaces x axis by 1 space

std::vector<std::vector<std::string>> takenPieces = {
    {""},
    {""}
};

int rowCount = 0;
int itemInRowCount = 0;

std::string currentFile;

//+ input and ouput

void out(std::string message) {
    std::cout << message;
}

std::string input(std::string message) {
    std::string put;

    out(message);
    std::getline(std::cin, put);
    return put;
}

//+ draw grid

std::string generateIconSpacing(int spacing) {
    std::string whitespace = "";
    for (int i = 0; i < spacing; i++) {
        whitespace += " ";
    }

    return whitespace;
}

void printXLabels() {
    std::string spacing = generateIconSpacing(gridSpacing / 2);
    std::vector labels = {"a", "b", "c", "d", "e", "f", "g", "h"};
    int labelIndex = 0;

    for (int i=0; i < (gridSpacing * xWidth) + (squaresOnAxes * pieceOverflow) + yLineOverflow; i++) {
        if (i % squaresOnAxes == 0 && i != 0) {
            out(" " + spacing + labels[labelIndex] + spacing);
            labelIndex ++;
        }
    }

    out("\n");
}

void printXGridline() {
    for (int i=0; i < (gridSpacing * xWidth) + (squaresOnAxes * pieceOverflow) + yLineOverflow; i++) {
        out("―");
    }

    out("\n");
}

void printYGridline(std::vector<std::vector<BoardItem>> board) {
    std::string spacing;
    int midPoint = 1;

    for (int i = 0; i < yLength; i++) {
        out("|");

        for (int j = 0; j < squaresOnAxes; j++) {
            spacing = generateIconSpacing(gridSpacing + pieceOverflow);

            if (i == midPoint) {
                spacing = generateIconSpacing(gridSpacing / 2);
                out(spacing + board[rowCount][itemInRowCount].icon);

                itemInRowCount ++;

                if (itemInRowCount % squaresOnAxes == 0) {
                    rowCount ++;
                    itemInRowCount = 0;
                }
            }

            out(spacing + "|");
        }

        if (i == midPoint) {
            out(spacing + std::to_string(9-rowCount)); //9 inverts numbers
        }

        out("\n");
    }
}

void drawBoard(std::vector<std::vector<BoardItem>> board, bool isAnimated = false) {
    for (int i = 0; i < squaresOnAxes; i++) {
        printXGridline();

        if (isAnimated) {
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }

        printYGridline(board);
    }

    printXGridline();
    printXLabels();

    rowCount = 0;
    itemInRowCount = 0;
}

void clearAndDraw(std::vector<std::vector<BoardItem>> board, bool isAnimated = false) {
    system("clear");
    drawBoard(board, isAnimated);
}

//+ generate 2d board vector

std::vector<BoardItem> generateMajorPieceRow(bool isDark) {
    std::vector<BoardItem> majorPieceRow;
    std::vector<std::string> names = {"ROOK", "KNHT", "BSHP", "QUEN", "KING", "BSHP", "KNHT", "ROOK"};

    for (int i = 0; i < names.size(); i++) {
        BoardItem boardItem;
        boardItem.isDark = isDark;
        boardItem.setName(names[i]);
        boardItem.setIcon();
        majorPieceRow.push_back(boardItem);
    }

    return majorPieceRow;
}

std::vector<BoardItem> generatePawnRow(bool isDark) {
    std::vector<BoardItem> pawnRow;

    for (int i = 0; i < squaresOnAxes; i++) {
        BoardItem boardItem;
        boardItem.isDark = isDark;
        boardItem.setName("PAWN");
        boardItem.setIcon();
        pawnRow.push_back(boardItem);
    }

    return pawnRow;
}

std::vector<BoardItem> generateEmptyRow() {
    std::vector<BoardItem> emptyRow;

    for (int i = 0; i < squaresOnAxes; i++) {
        BoardItem boardItem;
        boardItem.setName("EMTY");
        boardItem.setIcon();
        emptyRow.push_back(boardItem);
    }

    return emptyRow;
}

std::vector<std::vector<BoardItem>> initialiseBoardStructure() {
    std::vector<std::vector<BoardItem>> board;
    int emptyRowAmount = 4;

    board.push_back(generateMajorPieceRow(true));
    board.push_back(generatePawnRow(true));
    //board.push_back(generateEmptyRow());

    for (int i = 0; i < emptyRowAmount; i++) {
        board.push_back(generateEmptyRow());
    }

    //board.push_back(generateEmptyRow());
    board.push_back(generatePawnRow(false));
    board.push_back(generateMajorPieceRow(false));

    return board;
}

//+ interactions with board

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

//+ read, write and load save data

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

//+ validating inputs

std::string lower(std::string toBeLowered) {
    std::string lowerString = "";
    std::smatch matchLetters;
    std::regex lettersExpression("[^A-H]");
    
    for (int i = 0; i < toBeLowered.size(); i++) {
        if (std::regex_search(toBeLowered, matchLetters, lettersExpression)) {
            char lowered = tolower(toBeLowered[i]);
            lowerString += lowered;
        }
        else {
            lowerString += toBeLowered[i];
        }
    }

    return lowerString;    
}

bool validateLength(std::string input) {
    int acceptedLength = 4; // 2 chars each position, 1 char for whitespace

    if (input.size() == acceptedLength) {
        return true;
    } 

    return false;
}

bool validateNoMove(std::string input) {
    if (input.substr(0, 2) != input.substr(2, 2)) {
        return true;
    } 

    return false;
}

bool validateNumber(std::string toBeValidated) {
    std:: smatch matchLetters;
    std::regex lettersExpression("[^0-9]"); 

    if (std::regex_search(toBeValidated, matchLetters, lettersExpression))
        return false;
    
    return true;
}

bool validateChars(std::string input) {
    std::smatch matchChars;
    std::regex charsExpression("[^0-8] [^a-h]");

    if (std::regex_search(input, matchChars, charsExpression)) {
        return false;
    }

    std::regex digitsExpression("[^0-8]");

    std::ptrdiff_t digitOccurences(std::distance(
        std::sregex_iterator(input.begin(), input.end(), digitsExpression),
        std::sregex_iterator())
    );

    std::regex lettersExpression("[^a-h]");

    std::ptrdiff_t lettersOccurences(std::distance(
        std::sregex_iterator(input.begin(), input.end(), lettersExpression),
        std::sregex_iterator())
    );
    
    if (digitOccurences > 2 || lettersOccurences > 2) {
        return false;
    }

    std::smatch matchDigits;

    std::string inputSecond = input.substr(1, 1);
    std::string inputFourth = input.substr(3, 1);

    if (std::regex_search(inputSecond, matchDigits, digitsExpression) || std::regex_search(inputFourth, matchDigits, digitsExpression)) {
        return false;
    }

    std::smatch matchLetters;
    
    std::string inputFirst = input.substr(0, 1);
    std::string inputThird = input.substr(2, 1);

    if (std::regex_search(inputFirst, matchLetters, lettersExpression) || std::regex_search(inputThird, matchLetters, lettersExpression)) {
        return false;
    }
    
    return true;
}

//+ miscellaneous methods directly for main

bool coinToss() {
	int random;
    srand((unsigned)time(0)); //generates random seed
    random = (rand() % 2) + 1; //generates random int of 0 or 1

    if (random == 1)
        return true;
    
    return false;
}

void outputSaves(std::vector<std::filesystem::path> fileVector) {
    int counter = 0;

    for (int i = 0; i < fileVector.size(); i++) {
        std::string cleaned = std::string(fileVector[i]).substr(23);
        cleaned = cleaned.substr(0, cleaned.size() - 4);

        out(std::to_string(counter) + ". " + cleaned);
        counter ++;
    }
}

void outputTakes(bool isDark) {
    int index = (isDark) ? 0 : 1;

    if (takenPieces[index].size() != 1) {
        out(" You have taken:");

        for (int i = 1; i < takenPieces[index].size(); i++) {
            out(" " + takenPieces[index][i]);
        }
    }
}

void outputLosses(bool isDark) {
    int index = (isDark) ? 0 : 1;
    
    if (takenPieces[index].size() != 1) {
        out(" You have lost:");

        for (int i = 1; i < takenPieces[index].size(); i++) {
            out(" " + takenPieces[index][i]);
        }
    }
}

bool validateMoveset(BoardItem current, std::pair<int, int> vector) {
    bool isVectorValid = false;

    if (current.name == "KING") {
        isVectorValid = current.validateOneAround(vector);
    }
    else if (current.name == "QUEN") {
        if (current.validateOneAround(vector) || current.validateDiagonal(vector) || current.validateStraight(vector))
            isVectorValid = true;
    }
    else if (current.name == "KNHT") {
        isVectorValid = current.validateJumps(vector);
    }
    else if (current.name == "BSHP") {
        isVectorValid = current.validateDiagonal(vector);
    }
    else if (current.name == "ROOK") {
        isVectorValid = current.validateStraight(vector);
    }
    else if (current.name == "PAWN") {
        isVectorValid = current.validateStep(vector, current.isDark);
    }

    return isVectorValid;
}

void congratulations(std::vector<std::vector<BoardItem>> board, bool winner) {
    std::string flag;
    if (winner) 
        flag = "⚐";
    else
        flag = "⚑";

    for (int i = 0; i < 3; i++) {
        system("clear");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        drawBoard(board);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    out("\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    for (int j = 0; j < 33; j++) {
        out(flag + " ");
    }

    out("\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    out("A big congratulations to ");

    if (winner) 
        out("dark ");
    else
        out("light ");

    out("for winning the match!");
}

int main() {
    bool isGameFinished = false;
    bool isTurnOver = false;

    bool isLoadChoiceValid = false;
    bool isSaveChoiceValid = false;

    bool isDarkWinner;
    bool isDarkTurn;
    std::vector<std::vector<BoardItem>> board;

    system("clear");

    while(!isLoadChoiceValid) {
        std::string loadChoice = lower(input("Would you like to load a save? (y/n): "));

        if (loadChoice == "y") {
            system("clear");

            std::vector<std::filesystem::path> fileVector = getPathsInFolder();
            if (fileVector.size() != 0) {
                isLoadChoiceValid = true;

                while(!isSaveChoiceValid) {
                    outputSaves(fileVector);
                    std::string saveChoice = input("Use the numpad to select a choice: ");

                    if (validateNumber(saveChoice)) {
                        int choiceInt = stoi(saveChoice);

                        if (choiceInt <= fileVector.size() - 1) {
                            isSaveChoiceValid = true;

                            currentFile = fileVector[choiceInt];
                            std::vector<std::vector<std::string>> dataGrid = readFile2d(fileVector[choiceInt]);
                            board = loadBoard(board, dataGrid);

                            takenPieces[0] = dataGrid[8]; //isDark
                            takenPieces[1] = dataGrid[9]; //!isDark

                            if (dataGrid[10][0] == "true") //isDarkTurn
                                isDarkTurn = true;
                            else
                                isDarkTurn = false;
                        }
                        else {
                            system("clear");
                            out("There is no file at the place '" + saveChoice + "'.\n\n"); 
                        }
                    }
                    else {
                        system("clear");
                        out("'" + saveChoice + "' is not a valid choice.\n\n");
                    }
                }
            }
            else {
                system("clear");
                out("There are no saves.\n");
            }
        }
        else if (loadChoice == "n") {
            isLoadChoiceValid = true;
            isDarkTurn = coinToss();

            board = initialiseBoardStructure();
            board = assignPositions(board);

            newSave(board, isDarkTurn);
        }
        else {
            system("clear");
            out("Answer 'y' for yes and 'n' for no.\n");
        }
    }

    while(!isGameFinished) {
        isTurnOver = false;
        //system("clear");

        clearAndDraw(board);

        while(!isTurnOver) {
            if (isDarkTurn) {
                out("\nDark, it is your turn.");
                outputTakes(true);
                outputLosses(false);
            }
            else {
                out("\nLight, it is your turn.");
                outputTakes(false);
                outputLosses(true);
            }

            std::string move = lower(input("\nMake your move: "));

            if (validateLength(move) && validateChars(move) && validateNoMove(move)) {
                std::string currentString = move.substr(0, 2);
                std::pair<int, int> currentIndices = findIndexInVector(board, currentString);
                BoardItem curr = board[currentIndices.first][currentIndices.second];

                std::string desiredString = move.substr(2, 2);
                std::pair<int, int> desiredIndices = findIndexInVector(board, desiredString);
                BoardItem des = board[desiredIndices.first][desiredIndices.second];

                if (curr.isDark == isDarkTurn) {
                    if (curr.name == "EMTY") {
                        clearAndDraw(board);
                        out("\nThere is no piece on position '" + curr.position + "' to move to position '" + desiredString + "'.");
                    }
                    else {
                        std::pair<int, int> vector = curr.createColumnVector(desiredString);

                        if (validateMoveset(curr, vector)) {
                            if (stepThrough(currentIndices, desiredIndices, vector, board) || curr.name == "KNHT") {
                                //need to consider castling and eventually change this
                                if (des.name == "EMTY") {
                                    board = swap(board, currentString, desiredString);
                                    isTurnOver = true;
                                    isDarkTurn = !isDarkTurn;
                                    clearAndDraw(board, true);
                                }
                                else {
                                    if (des.isDark != isDarkTurn) {
                                        if (curr.isDark) 
                                            takenPieces[0].push_back(des.icon);
                                        else
                                            takenPieces[1].push_back(des.icon);

                                        board = take(board, currentString, desiredString);

                                        if (des.name != "KING") {
                                            isTurnOver = true;
                                            isDarkTurn = !isDarkTurn;
                                            clearAndDraw(board, true);
                                        }
                                        else {
                                            isTurnOver = true;
                                            isGameFinished = true;
                                            isDarkWinner = isDarkTurn;
                                        }
                                    }
                                    else {
                                        clearAndDraw(board);
                                        out("you cannot take your own piece '" + des.icon + "'.");
                                    }
                                }

                                updateSave(board, isDarkTurn);
                            }
                            else 
                                out("\nThe path of the piece '" + curr.icon + "' on the way to '" + desiredString + "' is being blocked by another piece.");
                        }
                        else {
                            clearAndDraw(board);
                            out("\nThe move '" + move + "' is not part of the moveset of the board piece '" + curr.icon + " '.");
                        }
                    }
                }
                else {
                    clearAndDraw(board);
                    out("\nThe piece on position '" + curr.position + "' is not your piece to move.");
                }
            }
            else {
                clearAndDraw(board);
                out("\nInput is invalid, it should match the following pattern:\n<coordinate of piece to move><coordinate of where to move to>\nThe 2 coordinates must be different.\ne.g: d2g5\n");
            }
        }
    }

    congratulations(board, isDarkWinner);
}