#include <iostream>
#include <string> 

void OutString(std::string message) {
    std::cout << message;
}

int main()
{
    OutString("|\n");
    OutString("|\n");
    OutString("|\n");
    OutString("|\n");
    OutString("|\n");
    OutString("|\n");
    std::cout << "\u2654";
}