# Advanced Programming 2 - Console Chess

Play it here - https://replit.com/@BongoSLAP/Console-Chess-working

## Outline
The main problems I needed to overcome when trying to plan and implement a solution were:
* Having a set of classes to store the properties and methods for each piece.
* Storing the pieces in a data structure to iterate over.
* Using suitable characters to represent the board and print it out to the terminal with adequate spacing.
* Validating the string input so that it is congruent with a coordinate system for moving the pieces. (a-h 1-8).
* Further validating that the moves input are congruent with the legal moves for each pieces. 
    * This essentially meant that I needed to figure out a way of representing the rules of chess in code.
* Figuring out a way of translating string inputs into usable and iterable values to make this validation possible.
* Saving and Loading the game. 

## UML diagram
[See UML diagram](https://replit.com/@BongoSLAP/Console-Chess-working#Docs/initial_uml.PNG)

## Initial working plan
My initial approach to implementing the game after I had got the grid printing out correctly was to create a way for the program to understand how to traverse from any one place on the 2d vector to any other place. While playing around with an online chess board editor I realised that the board could be thought of as the top right quadrant (Quadrant I) of a four-quadrant grid:

![quadrants](https://blog.prepscholar.com/hubfs/image-4.png)

Armed with this medium it made it easier to think of a way to traverse the board. I remembered the trigonometric idea of a column vector from GCSE maths and I realised that this is exactly what I needed to overcome my first challenge. 

A column vector is a pair of x y values that can be either negative or positive which are used to represent the direction and distance between 2 points on a grid:

![vector](https://mammothmemory.net/images/user/base/Maths/Pythagoras%20and%20Trigonometry/Vectors/how-to-read-a-column-vector.311f36e.jpg)

![vector](https://mammothmemory.net/images/user/base/Maths/Pythagoras%20and%20Trigonometry/Vectors/vector-showing-the-verticle-and-horizontal-components.f47ba83.jpg)

Once I had the way to traverse the board I chose to use these column vectors as a fundamental role in my overall approach to implementing the game. The vectors would go on to be used throughout the program, being passed into and returned as type of `std::pair<int, int>` for the purposes of validating legal moves. This essentialy became my development strategy, using primarily `std::pair` and `bool` to store and control the flow of data in the game loop.

## Epics
* Print the board out onto the terminal using a 2d vector to organise the icons on the grid, use spacing to make the board clear to understand and easy to look at.
* Exhaustively validate the string input using multiple regex patterns, also checking length to completely avoid difficult-to-debug runtime errors. Do not let the game progress until the input is appropriate.
* Use column vectors to validate moves against the rules of the game. 
    * For example, if bishops can only move at perfect diagonals, a column vector where the absolutely casted x value is equal to the absolutely casted y value can be the only valid vector for the rules of that specific piece. Do not let the game progress until move input is legal.
    * Absolutely casting is a way of making negative values positive which is required because something like -3 would not be equal to 3, however in the case of diagonals the negative does not matter.
* Use the column vector to traverse the 2d grid in a for loop to see whether any pieces on the board block the path of the chosen board piece on the way to its destination. If something is being blocked, this move is illegal. Do not progress the game until the player enters a move that is not blocked by other pieces.
* Save the state of the game to a text file using some loops to write the name of the piece and its colour. Also write any other relevant data to the file such as whos turn it is. Save the name of the file as the current date and time.
    * Create a method to update this file after every turn.
* Create a method to read save files and apply the data to the board and any other variables relevant.

## Initial OOP strategy
Initially I wanted to structure my classes in the way I had designed under the UML section. This is so that I could have a base class of `BoardItem` that contained the most basic properties and methods that every piece would need. 

Each piece would then extend this base class and the idea I had in my head was that it could use the base properties and methods such as `.name`, `.position`, `.isDark` and `.createColumnVector()` in each child class method `.validateVector()`. For each child class, this method would have the same name however depending on the class the logic would be different in order to correspond with the moveset of the specific piece. 

For example, the logic for validating legal moves for a `Rook` class (only straight horizontal or vertical lines), would be different for a `Bishop`-typed object (only perfect diagonals). It would also be different for the `King` class and so on.

However, I ran into issues with this strategy which I regrettably could not overcome in the time I had allowed myself to focus on this blocker (2 days), choosing to opt for a more complete solution in trade off for a less granular class structure. I will speak more on this issue and what I did to try and overcome it in the video and the evaluation section.

## Good programming standards
* Minimised side effects and scope
    * My code is quite functional and quite strictly scoped. I was careful to ensure that most methods are clear in their purpose and there is little to no variability in what is returned for identical arguments. 

    * That being said, there are 4 non-constant globally scoped variables in my program. However I believe that they are neccesary and they have not proven to be an issue for me when debugging.
* No Multi-purpose functions
    * I think that it was important for me to make sure that I was not over-engineering the functions to be all singing all dancing as it made it much easier to debug my code.
* Indentation, Naming conventions and spacing
    * I think that my use of whitespace is appropriate in separating the logic and conditions into clean blocks, my variable names sacrifice succinctness for the sake of being more descriptive and readable.
* Repetition
    * By splitting up my code into many pure and reusable functions I was able to reduce unneccesary repeats of code.
* Testing and Errors 
    * I am very happy with the overall robustness of my program and I believe that the layered validation approach I took enabled me to adopt a good process for finding bugs and fixing them.

## Main functions implemented
My program is split into 2 main phases, the initial phase I stopped working on as I had quite a big blocker that was costing me time. This phase is under the 'experimental' branch on the repo and the replit of this phase can be found at this link: https://replit.com/@JacobBright/Console-Chess#ConsoleChess/Main.cpp. 

### Phase 1
Rendering out the board to the terminal:
```cpp
void drawBoard(std::vector<std::vector<BoardItem>> board) {
    for (int i = 0; i < squaresOnAxes; i++) {
        printXGridline();
        printYGridline(board);
    }

    printXGridline();
    printXLabels();

    rowCount = 0;
    itemInRowCount = 0;
}
```
* `printXGridline()` prints a line of '_'. 
* `printYGridline(board)` prints a line of '|' and uses the 2d board vector passed into it to output the icons of that row along with some spacing on either side. 
* `printXlabels()` prints the letters a-f under each square.
---
Creating and pushing the rows of the pieces into the 2d board vector:
```cpp
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

    return board;
} 
```
* `generatePawnRow()` returns a vector of pieces of type `Pawn`
* `generateEmptyRow()` returns a vector of pieces of type `Empty`
* `generateMajorPieceRow()` returns a vector of multiple different types of the major pieces on the top and bottom of the board. it takes a bool to decide whether the icon is dark or light coloured.
```cpp
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
    ...
```
* I thought that because each of these child classes extend the base class `BoardItem` that it wouldnt be a problem if I specified that the return type of the vector would be `std::vector<BoardItem>`. This was the start of the issue for this phase for me which would ultimately lead to me changing the class structure.
* After changing the structure this method became more simple to implement as I did not need to create lots of different classes and I could refactor just using a loop:
```cpp
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
```
---
Validation:
```cpp 
if (validateLength(move) && validateChars(move) && validateNoMove(move)) {
```
* The 3 validation functions take the input string `move`
* `validateLength()` ensures that the string is exactly 4 chars long. 
* `validateChars()` uses regex to ensure that the string contains no chars other than a-f or 1-8, it also ensures that the letter comes before the number.
* `validateNoMove()` ensures that the 2 coordinates are not the same.
---
Swapping pieces around:
```cpp
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
```
* `swap()` swaps any 2 pieces on the 2d vector and swaps around their position properties
* A similar function also exists for takes, replacing with an empty board item instead of simply swapping them around.
---
Creating a column vector:
```cpp
std::pair <int, int> createColumnVector(std::string desiredPosition) {
    std::pair <int, int> columnVector;
    std::pair <int, int> currentCoordinates = generateCoordinates(this -> position);
    std::pair <int, int> desiredCoordinates = generateCoordinates(desiredPosition);

    columnVector.first = desiredCoordinates.first - currentCoordinates.first;
    columnVector.second = desiredCoordinates.second - currentCoordinates.second;
            
    return columnVector;
}
```
* This method lives in the `BoardItem` base class.
* `generateCoordinates()` takes a string and translates it into coordinates that are used to calculate the column vector.
---
Validating diagonals for bishop:
```cpp
bool validateVector(std::pair<int, int> vector) {
    if (std::abs(vector.first) == std::abs(vector.second)) {
        return true;
    }

    return false;
}
```
* This method lived in the `Bishop` class and checks that the absolute x and y values of the column vector are equal. This means that it is a perfect diagonal.
* This method of the same name would be in all piece classes but with different logic.

This is where the problem with this approach manifested itself. as when I tried to write:
```cpp
board[indices.first][indices.second].validateVector(vector);
```
I got the error `error: no member named 'validateVector' in 'BoardItem'`
This is because I initialised the vector as `BoardItem`, which does not contain the method `.validateVector()`.

I will speak about my approaches in trying to get this to work in the evaluation.

### Phase 2
Validating vectors:
```cpp
bool validateStraight(std::pair<int, int> vector) {
    if ((vector.first != 0 && vector.second == 0) || (vector.first == 0 && vector.second != 0))
        return true;
    return false;
};
```
* This function validates that the vector only travels in straight horizontal or diagonal lines
---
```cpp
bool validateStep(std::pair<int, int> vector, bool isDark) {
    int direction;

    if (isDark)
        direction = -1;
    else
        direction = 1;

    if ((vector.first == 0 && vector.second == direction) || (vector.first == 0 && vector.second == direction * 2))
        return true;
    
    return false;
}
```
* This function validates that the pawn can only move in 1 or 2 steps, it also ensures that it cant move backwards.

I later changed this method to accomodate takes, as pawns can only take at diagonals:
```cpp
if (!isTake) {
    if (vector.first == 0 && vector.second == direction)
        return true;
    else if ((vector.first == 0 && vector.second == direction * 2) && isFirstMove)
        return true;
}
else {
    if (std::abs(vector.first) == std::abs(direction) && vector.second == direction)
        return true;
}
```
---
```cpp
bool validateOneAround(std::pair<int, int> vector) {
    if (((vector.first == 0 && std::abs(vector.second) == 1) || (std::abs(vector.first) == 1 && vector.second == 0)) || (std::abs(vector.first) == 1 && std::abs(vector.second) == 1)) 
        return true;
    
    return false;
};
```
* This function validates that the vector only travels one space in any direction.
---
```cpp
bool validateJumps(std::pair<int, int> vector) {
    if ((vector.first == 1) && (vector.second == 2 || vector.second == -2)) 
        return true;
    else if ((vector.first == 2) && (vector.second == 1 || vector.second == -1)) 
        return true;
    else if ((vector.first == -1) && (vector.second == 2 || vector.second == -2)) 
        return true;
    else if ((vector.first == -2) && (vector.second == 1 || vector.second == -1)) 
        return true;
    
    return false;
};
```
* This function validates that the knight jumps only onto its legal spaces.
---
Validating moveset:
```cpp
bool validateMoveset(std::pair<int, int> currentIndices, BoardItem current, BoardItem desired, std::pair<int, int> vector) {
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
        bool isTake = false;
        bool isFirstMove = false;
        out(std::to_string(vector.first) + std::to_string(vector.second));

        if (desired.name != "EMTY")
            isTake = true;
        
        if ((current.isDark && currentIndices.first == 1) || (!current.isDark && currentIndices.first == 6)) 
            isFirstMove = true;

        isVectorValid = current.validateStep(vector, current.isDark, isTake, isFirstMove);
    }

    return isVectorValid;
}
```
* This function uses all of those previous functions to ensure that the vectors are part of the moveset of the current piece.
* The `name == "PAWN"` condition needed further logic to determine whether it was the pawns first move or if the pawn is taking another piece.
---
```cpp
bool stepThrough(std::pair<int, int> currentPosition, std::pair<int, int> desiredPosition, std::pair<int, int> vector, std::vector<std::vector<BoardItem>> board) {
    int largestAbsolute = findLargest(vector);
    int stepX = 0;
    int stepY = 0;
    
    for (int i = 0; i < largestAbsolute; i++) {
        if (vector.first != 0 && vector.first != stepX) 
            (isPositive(vector.first)) ? stepX ++ : stepX --;

        if (vector.second != 0 && vector.second != stepY) 
            (isPositive(vector.second)) ? stepY ++ : stepY --;

        if (currentPosition.second + stepX != desiredPosition.second || currentPosition.first - stepY != desiredPosition.first) {
            if (board[currentPosition.first - stepY][currentPosition.second + stepX].name != "EMTY") 
                return false;
        }
        else if (currentPosition.second + stepX == desiredPosition.second && currentPosition.first - stepY == desiredPosition.first) {
            return true;
        }
    }
}
```
* This function serves as the final level of validation of the input to check whether anything blocks the piece on its way to its destination.
* It uses the column vector to step through it and checks at each step to ensure that all board items on its path are empty.

## Testing and resolving bugs
My approach to testing was mainly focussed on using the analysis program on chess.com: https://www.chess.com/analysis

I would manually test my program and compare its behaviour against the analysis board on chess.com to see if the behaviour matched. As well as having knowledge of the rules of chess, this allowed me to have a good way of expecting the appropriate behaviour of my game. 

After implementing the save game functionality, testing became easier as I was able to save different scenarios and replay them to check the correct functionality was working.

## Key design challenges
The key design challenge I faced was structuring my classes and validating the moves. I believe that I was able to overcome the validation without too much issue as evidenced in the code examples above but I think that if I had a more fundamental understanding of how to structure c++ programs that I would have been able to have a more granular class structure.

## Evaluation
The biggest refactor of my program was by far the class structure issue I mentioned previously, I went from having 8 classes with inheritance to just having a single class.

This was because I needed to specify all of the different classes that would be in my 2d vector and I believed that just specifying the base class `BoardItem` would suffice. However this was not the case as the compiler was not recognising any functions that were in the extended child classes. So when I tried to do something like:

```cpp
board[indices.first][indices.second].validateVector(vector);
```
The function `.validateVector()` would not be recognised because this method was in the child class, not the parent.

After doing a lot of research on Stackoverflow and Youtube I tried to use `std::variant` to have a vector of variant types that in theory would be accessed using `std::get` in order to access all of the different types:

```cpp
std::vector<std::vector<std::variant<BoardItem, King, Queen, Knight, Bishop, Rook, Pawn, Empty>>>
```
However, this caused further issues for me as it appeared that the type of the `std::get` call would need to be specified at compile time. This was an issue because throughout my code I needed to loop through the board vector and access various properties and call various methods:
```cpp
Rook rook;
board[0][0] = rook;
rook.setName();
std::get<Rook>(board[0][0]); //no bad variant access exception as [0][0] contains rook
std::get<Queen>(board[0][0]); //bad variant access exception as [0][0] does not contain queen
```
This above example shows the problem I faced. If for example I wanted to access the first item in the vector which was of type `Rook` I could simply use `std::get<Rook>` with little issue, but if I was iterating over the board and the next item in the vector was of type `Queen`, I would get a `bad variant access` exception as I am still looking to access a `Rook`. 

After spending a few hours trying to overcome this it appeared obvious to me that there was something fundamentally built into the architecture of the `std::variant` that forced me at every turn to specify the type of the `std::get` at compile time which made it seemingly impossible to have some kind of programmatic change in the type to allow me to loop over the board. 

After exhausting everything I had tried, I had a look at using `std::any`:
```cpp
std::vector<std::vector<std::any>> board
```
I could then use an if else if block to programmatically cast to each type when needed:
```cpp
if (strcmp(board[rowCount][itemInRowCount].type().name(), "4King") == 0) 
    out(spacing + std::any_cast<King>(board[rowCount][itemInRowCount]).icon);
else if (strcmp(board[rowCount][itemInRowCount].type().name(), "5Queen") == 0) 
    out(spacing + std::any_cast<Queen>(board[rowCount][itemInRowCount]).icon);
else if (strcmp(board[rowCount][itemInRowCount].type().name(), "6Knight") == 0)
    out(spacing + std::any_cast<Knight>(board[rowCount][itemInRowCount]).icon);
else if (strcmp(board[rowCount][itemInRowCount].type().name(), "6Bishop") == 0) 
    out(spacing + std::any_cast<Bishop>(board[rowCount][itemInRowCount]).icon);
else if (strcmp(board[rowCount][itemInRowCount].type().name(), "4Rook") == 0) 
    out(spacing + std::any_cast<Rook>(board[rowCount][itemInRowCount]).icon);
else if (strcmp(board[rowCount][itemInRowCount].type().name(), "4Pawn") == 0) 
    out(spacing + std::any_cast<Pawn>(board[rowCount][itemInRowCount]).icon);
else if (strcmp(board[rowCount][itemInRowCount].type().name(), "5Empty") == 0) 
    out(spacing + std::any_cast<Empty>(board[rowCount][itemInRowCount]).icon);
```
This block is an example of using the `std::any_cast` that I was playing around with and it seemed to work without producing cast exceptions. This probably would have solved my problem but I chose to not use this as for every loop of the board I would have to include this fat, largely unreadable block of conditions to get it to work.

The next logical step that one would think would be 'why not put this into a function and use a callback for acessing the member' but when I tried this I had to specify a return type for this function at compile time and I was running into the same issue as the `std::variant` approach.

In the end, I made an active design decision to not waste any more time in trying to get this function to work and I opted to just use a single class as the most simple approach that worked. I technically could have repeated this above block of code for every loop and it would have worked but I chose not to as it seemed ridiculous to do so.

I am sure there is a solution to this using pointers or structs which would have been my next approach but I had to give up on this in order to build out more functionality of my game.

## Key features: 
* I am proud of the "Filtering down" approach to validation that allowed me to take something as basic as a 4 char string and determine whether this would be a valid move on a chess board.
    * This made it really simple for the player to make moves while still giving the program full control over the rules of the game.
    * It also made it clear to the user where they have gone wrong as I had multiple levels of validation that could output to the user at which stage they had made a mistake:
```cpp
                                else 
                                out("\nThe path of the piece '" + curr.icon + "' on the way to '" + desiredString + "' is being blocked by another piece.");
                        }
                        else {
                            clearAndDraw(board);
                            out("\nThe move '" + move + "' is not part of the moveset of the board piece '" + curr.icon + " '.");
                        }
                    }
                    else {
                        clearAndDraw(board);
                        out("\nThere is no piece on position '" + curr.position + "' to move to position '" + desiredString + "'.");
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
```
    
[validation diagram](https://replit.com/@JacobBright/Console-Chess-working#Docs/validation_diagram.png)

* Being able to save and load games.
    * I think that choosing to have a 2d vector data structure that stores the objects on the board and then just rendering out this vector made it very easy for me to implement things like saving and loading the game.
    * I believe that a different approach would have created more obstacles for doing something like this.
```cpp
void updateSave(std::vector<std::vector<BoardItem>> board, bool isDarkTurn) {
    std::ofstream save(currentFile, std::ofstream::out | std::ofstream::trunc); //trunc deletes contents
    
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++)
            save << board[i][j].name + "." + boolToString(board[i][j].isDark) + " ";

        save << "\n";
    }

    for (int i = 0; i < takenPieces.size(); i++) {
        for (int j = 0; j < takenPieces[i].size(); j++)
            save << takenPieces[i][j] + " ";

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

std::vector<std::vector<BoardItem>> loadBoard(std::vector<std::vector<BoardItem>> board, std::vector<std::vector<std::string>> dataGrid) {
    for (int i = 0; i < squaresOnAxes; i++) {
        std::vector<BoardItem> row;

        for (int j = 0; j < squaresOnAxes; j++) {
            BoardItem boardItem;
            boardItem.setName(dataGrid[i][j].substr(0, 4));

            (dataGrid[i][j].substr(5) == "true") ? boardItem.isDark = true : boardItem.isDark = false;

            boardItem.setIcon();
            row.push_back(boardItem);
        }
                                    
        board.push_back(row);
    }

    board = assignPositions(board);
    return board;
}
```
* Promoting pawns.
    * I think that being able to promote pawns when they reach the top/bottom of the board adds richness to my game, contributing to a fuller solution and a truer representation of real life chess:
```cpp
std::vector<std::vector<BoardItem>> promote(std::vector<std::vector<BoardItem>> board, std::string positionTo, std::pair<int, int> desiredIndices, bool isDark, std::string choice) {
    BoardItem promotion;
    promotion.setName(choice);
    promotion.isDark = isDark;
    promotion.setIcon();
    promotion.position = positionTo;
    
    board[desiredIndices.first][desiredIndices.second] = promotion;

    return board;
}

while (!isPromoteSelectionValid) {
    std::string promoteChoice = input("\n0. Queen\n1. Knight\n2. Rook\n3. Bishop\nEnter a number to select the piece you would like to promote the pawn to: ");
    if (promoteChoice == "0") {
        isPromoteSelectionValid = true;
        board = swap(board, currentString, desiredString);
        board = promote(board, desiredString, desiredIndices, isDarkTurn, "QUEN");
    }
    else if (promoteChoice == "1") {
        isPromoteSelectionValid = true;
        board = swap(board, currentString, desiredString);
        board = promote(board, desiredString, desiredIndices, isDarkTurn, "KNHT");
    }
    else if (promoteChoice == "2") {
        isPromoteSelectionValid = true;
        board = swap(board, currentString, desiredString);
        board = promote(board, desiredString, desiredIndices, isDarkTurn, "ROOK");
    }
    else if (promoteChoice == "3") {
        isPromoteSelectionValid = true;
        board = swap(board, currentString, desiredString);
        board = promote(board, desiredString, desiredIndices, isDarkTurn, "BSHP");
    }
    else {
        clearAndDraw(board);
        out("\n'" + promoteChoice + "' is not a valid choice.");
    }
}
```
In the interest of not making this document extremely long with code examples of all of my bug fixes and changes to functionality of both phases, my full commit history of both branches where I outline bugs and issues and how I tried to fix them can be seen:

https://github.com/jbr1gh7/Console-Chess/commits/experimental - phase 1 (not working)

https://github.com/jbr1gh7/Console-Chess/commits/main - final phase (working)
## Features that would be good to implement in future
* Peasant revolt gamemode.
    * Peasant revolt is a gamemode where the black side has 4 knights, a king and 1 pawn and the white side has only one king and 8 pawns.
    * It would be a fun little gamemode to add more variation to the game.

![peasants revolt](https://greenchess.net/variant/1upvsn.png)
* Castling
    * Allowing the rook to jump over the king while also moving the king if there are no pieces between them and if the king hasnt already moved.
    * It is the only move in chess that sees 2 pieces move in a single turn which suggests more complex changes to my program.

![castling](https://skillsology.com/wc/wp-content/uploads/2016/07/chess1.jpg)
* Adding an option for a player to resign if they know they cant win.
* Creating an "AI" type algorithm to determine whether the king is in check and limiting the players moves to only those which get them out of check
* Creating an "AI" type algorithm to determine whether the game is in stalemate (draw).
* Flipping the board each turn so that both players are making their attacks from the bottom of the board, this just makes it easier as the dark pieces have to think a little bit more when making their moves as they do it from the top down.
* Adding a timer
    * I initially wanted to do this but couldnt figure out how to keep user inputs before they are entered while also clearing the terminal to update the time.

## Review
Overall I am happy with what I was able to produce for this assignment.

I previously did an evaluation of the good practices I employed in the development section so this section will focus on the standards I need to improve on:
* Simplicity in logic and flow.
    * When implementing all of the different rules of chess into my program I began to be less satisfied with the simplicity of my code. As I implemented more rules and discovered all of the different caveats for the different pieces (the pawn in particular), the logic became a bit convoluted.
    * In future I must consider further splitting my main code up into more clearly signposted parts so anyone reading it is less likely to become lost.
* Deep nested conditionals.
    * Following on from the previous point, my main function code did start to become increasingly nested as I filtered down the validation and blocked the user from entering invalid data using while loops. 
    * In the worst case some parts of my code was indented 11 times.
    * To avoid this I need to split up these conditions in my main function into their own function and have some way of chaining them together, stopping the chain whenever something is invalid.
* Long lines of code.
    * There arent many long lines of instruction or variable assignment in my program, however there are quite a few if statements that have multiple conditions when trying to validate the vectors.
    * I tried my best to make it clear by separating the conditions with extra braces but this still appears very long and confusing.
    * I could have split them into multiple statements but then I run into the issue of having more deep nested conditionals so I need to find a balance.

```cpp
if (((vector.first == 0 && std::abs(vector.second) == 1) || (std::abs(vector.first) == 1 && vector.second == 0)) || (std::abs(vector.first) == 1 && std::abs(vector.second) == 1)) 
    return true;
```
* Load times and general structure.
    * When discussing my program with a friend of mine who makes c++ games at university, he told me that the way I had structured my header files is not the best practice.
    * In c++ the header files are actually supposed "set the table" for your class and there shouldnt really be any inline functions in them.
    * What I should have done is had a `BoardItem.cpp` file which contained the fucntion code for the functions in that class.
    * I think an impact of the way I have done it is that the program takes a few noticeable seconds to compile. However I am not 100% sure if this is the reason why as it could be something to do with the replit virtual machine or there is perhaps simply a lot of code to compile regardless.
* **Class structure.**
    * I mentioned previously in this section about the blocker I faced and this is probably the biggest part of my program that I am dissatisfied with.
    * In future I need to get an increased fundamental understanding of c++ and have a look at pointers and structs to solve this issue.
    * Once I have figured out how to structure things properly I can refine my code and re-add the inheritance back into my program.
    * Right now it seems that the structure of my code is much more of a functional or procedural approach as opposed to an object-oriented one.













