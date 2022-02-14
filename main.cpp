#include <iostream> //cout
#include <limits> //numeric limits for clearing bad user input
using namespace std;

#include "Gamespace.h" //Gamespace object
#include "GameNode.h" //GameNode object

//precondition: row and col are references to variables that will determine row and column size of a board
//postcondition: Sets the row and column values for the connect 4 board
void setRowsColumns(int &row, int &col);

//precondition: g is a pointer to the gamespace
  //id is whether or not the AI is playing defensively this game or not
//postcondition: returns the column that the AI will drop a chip into
int calculateAIMove(Gamespace* g, bool id);

int main() {
    Gamespace* g; //pointer to the gamespace instance
    int numTurns = 0; //the number of turns there have been
    int numRows, numCols; //the number of rows and cols for the gamespace
    bool isDefensive; //boolean for whether the computer is playing second

    cout << "Welcome to Connect Four!" << endl;
    setRowsColumns(numRows, numCols);

    //determine first move / set AI as defensive or offensive
    cout << "Who is going first? choose '1' for you, or '0' for the computer: ";
    cin >> isDefensive;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "The value is invalid please provide a '1' for player or '0' for computer: ";
        std::cin >> isDefensive;
    }

    //create Gamespace
    g = new Gamespace(numRows, numCols); //create the gamespace
    while (true) {
      g -> printBoard();
      int chosenCol;
      //if computer's turn, generate next move, else let user input next move
      if(numTurns % 2 == 0 ? !isDefensive : isDefensive) {
        chosenCol = numTurns != 0 ? calculateAIMove(g, !isDefensive) : numCols / 2;
        cout << "Computer dropped chip in column " << chosenCol << endl;
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
            if(chosenCol >= 0 && chosenCol < numCols && g -> checkValidMove(chosenCol))
                break;
            cout << "Invalid move... " << endl;
        }
      }
      //drop chip and check for a win
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

int calculateAIMove(Gamespace* g, bool id) {
  deque<int> playDeque;
  //create the root now, when this statement is done, the search is complete
  GameNode rootNode = GameNode(g, playDeque, NULL, !id, 5);
  return rootNode.getChosenMove();
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
