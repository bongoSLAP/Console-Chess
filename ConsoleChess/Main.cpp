#include <iostream>
#include <string> 

const int gridAmount = 8;
const int xSpacing = 10;
const int ySpacing = 3;

void out(std::string message) {
    std::cout << message;
}

void printXGridline() {
    for (int i=0; i<gridAmount*xSpacing+1; i++) {
        out("―");
    } 
}

void printYGridline() {
    for (int i=0; i<ySpacing; i++) {
        out("|");
        for (int j=0; j<gridAmount; j++) {
            out("         |");
        }
        out("\n");
    }

}

int main()
{


    for (int i=0; i<gridAmount; i++) {
        printXGridline();

        out("\n");

        printYGridline();
    }

    printXGridline();
}