#include <iostream>
#include <string> 
#include <vector>
#include <variant>

#include "Headers/BoardItem.h"
#include "Headers/King.h"
#include "Headers/Queen.h"
#include "Headers/Bishop.h"
#include "Headers/Knight.h"
#include "Headers/Rook.h"
#include "Headers/Pawn.h"
#include "Headers/Empty.h"

const int squaresOnAxes = 8;
const int gridSpacing = 6;
const int xWidth = 8;
const int yLength = 3;
const int yLineOverflow = 9; //the vertical lines displace x axis by 9 spaces
const int pieceOverflow = 1; //each board piece displaces x axis by 1 space

int rowCount = 0;
int itemInRowCount = 0;

void out(std::string message) {
    std::cout << message;
}

std::string generateIconSpacing(int spacing) {
    std::string whitespace = "";
    for (int i = 0; i < spacing; i++) {
        whitespace += " ";
    }

    return whitespace;
}

void printXGridline() {
    for (int i=0; i < (gridSpacing * xWidth) + (squaresOnAxes * pieceOverflow) + yLineOverflow; i++) {
        out("―");
    } 
}

void printYGridline(std::vector<std::vector<BoardItem>> board) {
    std::string spacing;

    for (int i = 0; i < yLength; i++) {
        out("|");

        for (int j = 0; j < squaresOnAxes; j++) {
            spacing = generateIconSpacing(gridSpacing + pieceOverflow);

            if (i == 1) {
                spacing = generateIconSpacing(gridSpacing / 2);
                out(spacing + board[rowCount][itemInRowCount].icon);

                itemInRowCount += 1;

                if (itemInRowCount % squaresOnAxes == 0 && rowCount < 9) {
                    rowCount += 1;
                    itemInRowCount = 0;
                }
            }

            out(spacing + "|");
        }

        out("\n");
    }
}

void drawBoard(std::vector<std::vector<BoardItem>> board) {
    for (int i = 0; i < squaresOnAxes; i++) {
        printXGridline();
        out("\n");
        printYGridline(board);
    }

    printXGridline();
    out("\n");
}

std::vector<BoardItem> generateMajorPieceRow(bool isDark) {
    std::vector<BoardItem> majorPieceRow;

    Rook leftRook;
    leftRook.isDark = isDark;
    leftRook.setName();
    leftRook.setIcon();
    majorPieceRow.push_back(leftRook);

    Knight leftKnight;
    leftKnight.isDark = isDark;
    leftKnight.setName();
    leftKnight.setIcon();
    majorPieceRow.push_back(leftKnight);

    Bishop leftBishop;
    leftBishop.isDark = isDark;
    leftBishop.setName();
    leftBishop.setIcon();
    majorPieceRow.push_back(leftBishop);

    Queen queen;
    queen.isDark = isDark;
    queen.setName();
    queen.setIcon();
    majorPieceRow.push_back(queen);
    
    King king;
    king.isDark = isDark;
    king.setName();
    king.setIcon();
    majorPieceRow.push_back(king);

    Bishop rightBishop;
    rightBishop.isDark = isDark;
    rightBishop.setName();
    rightBishop.setIcon();
    majorPieceRow.push_back(rightBishop);

    Knight rightKnight;
    rightKnight.isDark = isDark;
    rightKnight.setName();
    rightKnight.setIcon();
    majorPieceRow.push_back(rightKnight);

    Rook rightRook;
    rightRook.isDark = isDark;
    rightRook.setName();
    rightRook.setIcon();
    majorPieceRow.push_back(rightRook);

    /*
    for (int i = 0; i < majorPieceRow.size(); i++) {
        out(majorPieceRow[i].name + " ");
    }
    */

    /*
    for (int i = 0; i < majorPieceRow.size(); i++) {
        out(majorPieceRow[i].isDark + " ");
    }
    */

    /*
    for (int i = 0; i < majorPieceRow.size(); i++) {
        out(majorPieceRow[i].isDark + " ");
    }
    */

    return majorPieceRow;
}

std::vector<BoardItem> generatePawnRow(bool isDark) {
    std::vector<BoardItem> pawnRow;

    for (int i = 0; i < squaresOnAxes; i++) {
        Pawn pawn;
        pawn.isDark = isDark;
        pawn.setName();
        pawn.setIcon();
        pawnRow.push_back(pawn);
    }

    return pawnRow;
}

std::vector<BoardItem> generateEmptyRow() {
    std::vector<BoardItem> emptyRow;

    for (int i = 0; i < squaresOnAxes; i++) {
        Empty empty;
        empty.setName();
        empty.setIcon();
        emptyRow.push_back(empty);
    }

    return emptyRow;
}

std::string boolToString(bool b) {
  return b ? "true" : "false";
}

std::vector<std::vector<BoardItem>> initialiseBoardStructure() {
    std::vector<std::vector<BoardItem>> board;
    int emptyRowAmount = 4;

    board.push_back(generateMajorPieceRow(true));
    board.push_back(generatePawnRow(true));

    for (int i = 0; i < emptyRowAmount; i++) {
        board.push_back(generateEmptyRow());
    }

    board.push_back(generatePawnRow(false));
    board.push_back(generateMajorPieceRow(false));

    /*
    for (int j = 0; j < board.size(); j++) {
        for (int k = 0; k < board[j].size(); k++) { 
            //out(board[j][k].name + " ");
            //out(boolToString(board[j][k].isDark) + " ");
            //out(board[j][k].icon + " ");
        }
        
        out("\n");
    }
    */

    return board;
} 

int main()
{
    std::vector<std::vector<BoardItem>> board = initialiseBoardStructure();
    drawBoard(board);
}

