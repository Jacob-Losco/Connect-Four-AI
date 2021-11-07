#include <iostream>
using namespace std;

#include "Gamespace.h"

int main() {
    Gamespace* g;
    int numTurns = 0;
    int numRows, numCols;
    bool finished;
    string player;

    cout << "Welcome to Connect Four!" << endl;
    do {
      cout << "Please enter the number of rows for the board: ";
      cin >> numRows;
      cout << "Please enter the number of columns for the board ";
      cin >> numCols;
      if(numRows < 4 && numCols < 4)
        cout << "Invalid input. Make sure the number of rows and colums are greater than four..." << endl;
    } while(numRows < 4 && numCols < 4);

    g = new Gamespace(numRows, numCols);
    while(!finished) {
      g -> printBoard();
      if(numTurns == numRows * numCols) {
          player = "Tie";
          finished = true;
          break;
      }
      player = numTurns % 2 == 0 ? "Player 1" : "Player 2";
      int chosenCol;
      bool chosenColValid = false;
      while(!chosenColValid) {
          cout << player << ", choose a number 1-6 for the column you want to drop a chip into: ";
          cin >> chosenCol;
          if(g -> checkValidMove(chosenCol - 1)) {
              chosenColValid = true;
              break;
          }
          cout << "Invalid move... " << endl;
      }
      finished = g -> dropChip(numTurns % 2 == 0 ? 'r' : 'b', chosenCol - 1);
      numTurns++;
    }
    if(player == "Tie")
        cout << "It was a Tie!" << endl;
    else
        cout << player << " Wins!" << endl;
}
