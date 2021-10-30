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
#include "Headers/validateInput.h"
#include "Headers/inputOutput.h"
#include "Headers/drawGrid.h"
#include "Headers/generateBoard.h"
#include "Headers/interactBoard.h"
#include "Headers/readWriteLoad.h"

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

void outputSaves(std::vector<std::filesystem::path> fileVector) {
    int counter = 0;

    for (int i = 0; i < fileVector.size(); i++) {
        std::string cleaned = std::string(fileVector[i]).substr(23);
        cleaned = cleaned.substr(0, cleaned.size() - 4);

        out(std::to_string(counter) + ". " + cleaned);
        counter ++;
    }
}

bool coinToss() {
	int random;
    srand((unsigned)time(0)); //generates random seed
    random = (rand() % 2) + 1; //generates random int of 0 or 1

    if (random == 1)
        return true;
    
    return false;
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