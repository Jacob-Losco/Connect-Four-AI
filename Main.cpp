#include <iostream>
#include <limits>
using namespace std;

#include "Gamespace.h"

// Preconditions: Takes the row values and column values
// Postconditions: Sets the row and column values for the connect 4 board
void setRowsColumns(int &row, int &col);

int main() {
    Gamespace* g; //pointer to the gamespace instance
    int numTurns = 0; //the number of turns there have been
    int numRows, numCols; //the number of rows and cols for the gamespace
    bool isDefensive; //boolean for whether the computer plays first or second

    cout << "Welcome to Connect Four!" << endl;
    setRowsColumns(numRows, numCols);

    cout << "Who is going first? choose '1' for you, or '0' for the computer: ";
    cin >> isDefensive;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "The value is invalid please provide a '1' for player or '0' for computer: ";
        std::cin >> isDefensive;
    }

    g = new Gamespace(numRows, numCols); //create the gamespace
    while (true) {
      g -> printBoard();
      int chosenCol;
      //if computer's turn, generate next move, else let user input next move
      if(numTurns % 2 == 0 ? !isDefensive : isDefensive) {
        chosenCol = 0; //replace with code for ai
        cout << "\nComputer dropped chip in column " << chosenCol << endl;
      }
      else {
        while(true) {
            cout << "Choose a number 0-" << numCols - 1 << " for the column you want to drop a chip into: ";
            cin >> chosenCol;
            while (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "The value is invalid please provide a number ";
                std::cin >> chosenCol;
            }
            if(g -> checkValidMove(chosenCol))
                break;
            cout << "Invalid move... " << endl;
        }
      }
      //check for a win
      if(g -> dropChip(numTurns % 2 == 0 ? !isDefensive : isDefensive, chosenCol))
        break;
      numTurns++;
      //check for a tie
      if(numTurns == numRows * numCols) {
          numTurns = -1;
          break;
      }
    }

    //print final board and determine winner
    g -> printBoard();
    delete g;
    if (numTurns == -1)
        cout << "It was a Tie!" << endl;
    else
        cout << ((numTurns % 2 == 0 ? !isDefensive : isDefensive) ? "Computer Wins!" : "Player Wins!") << endl;
}

void setRowsColumns(int& row, int& col) {
    do {
        cout << "Please enter the number of rows for the board: ";
        cin >> row;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "The value is invalid please provide a number ";
            std::cin >> row;
        }
        if (row < 4)
            cout << "Invalid input. Make sure the number of rows is greater than four..." << endl;
    } while (row < 4);
    do{
        cout << "Please enter the number of columns for the board ";
        cin >> col;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "The value is invalid please provide a number ";
            std::cin >> col;
        }
        if (col < 4)
            cout << "Invalid input. Make sure the number of columns is greater than four..." << endl;
    } while (col < 4);
}
