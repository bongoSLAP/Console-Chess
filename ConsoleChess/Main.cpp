#include <iostream>
#include <string> 

const int gridAmount = 8;
const int xSpacing = 8;
const int ySpacing = 3;
const int yLineOverflow = 9; //the vertical lines displace x axis by 9 spaces
const int pieceOverflow = 1; //each board piece displaces x axis by 1 space

void out(std::string message) {
    std::cout << message;
}

std::string generateWhitespace(int spacing) {
    std::string whitespace = "";
    for (int i = 0; i < spacing; i ++) {
        whitespace += " ";
    }

    return whitespace;
}

void printXGridline() {
    for (int i=0; i < (gridAmount * xSpacing) + (gridAmount * pieceOverflow) + yLineOverflow; i++) {
        out("―");
    } 
}

void printYGridline() {
    std::string spacing = generateWhitespace(gridAmount + pieceOverflow);

    for (int i = 0; i < ySpacing; i++) {
        out("|");

        for (int j = 0; j < gridAmount; j++) {
            out(spacing + "|");
        }

        out("\n");
    }
}

int main()
{
    for (int i = 0; i < gridAmount; i++) {
        printXGridline();
        out("\n");
        printYGridline();
    }

    printXGridline();
}