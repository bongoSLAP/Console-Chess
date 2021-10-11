#include <iostream>
#include <string> 

const int gridAmount = 8;
const int yOverflow = 9; //9 vertical lines add extra space that needs to be accounted for when drawing horizontal line
const int xSpacing = 8;
const int ySpacing = 3;

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
    for (int i=0; i < gridAmount * xSpacing + yOverflow; i++) {
        out("―");
    } 
}

void printYGridline() {
    std::string spacing = generateWhitespace(gridAmount);

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