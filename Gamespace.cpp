#include <iostream>
using namespace std;

#include "Gamespace.h"

//definitions used to color and reset color for the chips when printing
#define RED "\033[31m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

Gamespace::Gamespace() {
  numRows = 6;
  numCols = 7;
  for(int i = 0; i < 7; i++)
    availableSpacesToDrop.push_back(5);
  for(int i = 0; i < 6; i++) {
    vector<Chip*> rowToInsert;
    for(int j = 0; j < 7; j++)
      rowToInsert.push_back(NULL);
    board.push_back(rowToInsert);
  }
}

Gamespace::Gamespace(int nr, int nc) {
  numRows = nr;
  numCols = nc;
  for(int i = 0; i < nc; i++)
    availableSpacesToDrop.push_back(nr - 1);
  for(int i = 0; i < nr; i++) {
    vector<Chip*> rowToInsert;
    for(int j = 0; j < nc; j++)
      rowToInsert.push_back(NULL);
    board.push_back(rowToInsert);
  }
}

void Gamespace::printBoard() {
  cout << "\nBOARD: " << endl << " ";
  for(int i = 0; i < numCols; i++)
    cout << " " << i << " ";
  cout << endl;
  for(int i = 0; i < numRows; i++) {
    cout << "|";
    for(int j = 0; j < numCols; j++) {
      Chip* cp = board.at(i).at(j);
      if(!cp)
        cout << " X ";
      else if(!(cp -> getIsAI()))
        cout << RED << " X " << RESET;
      else
        cout << BLUE << " X " << RESET;
    }
    cout << "|" << endl;
  }
}

bool Gamespace::checkValidMove(int possibleColChosen) {
  return possibleColChosen >= 0 && possibleColChosen <= numCols - 1 && availableSpacesToDrop.at(possibleColChosen) != -1;
}

bool Gamespace::dropChip(bool isAI, int colChosen) {

  //get the row position that the chip will drop into, then set the next available row to be the one above it
  int rowChosen = availableSpacesToDrop.at(colChosen);
  availableSpacesToDrop.at(colChosen)--;

  //set the chip at the position as the new value for this space
  board.at(rowChosen).at(colChosen) = new Chip(isAI);

  return checkWin(rowChosen, colChosen, isAI);
}

bool Gamespace::checkWin(int rowChosen, int colChosen, bool isAI) {
  //check for a win condition
  for(int rowIndex = rowChosen - 1; rowIndex <= rowChosen + 1; rowIndex++) {
    for(int colIndex = colChosen - 1; colIndex <= colChosen + 1; colIndex++) {
      if((rowIndex >= 0 && rowIndex <= numRows - 1) &&
        (colIndex >= 0 && colIndex <= numCols - 1) &&
        board.at(rowIndex).at(colIndex) &&
        board.at(rowIndex).at(colIndex) -> getIsAI() == isAI &&
        (rowIndex != rowChosen || colIndex != colChosen)) {
          int numConnected = 1; //the number of chips we have connected so far.
          //get the direction we have to step in to find the rest of the connection
          int directionY = colIndex - colChosen;
          int directionX = rowIndex - rowChosen;
          //get the next space to check
          int possibleConnectedSpaceRow = rowIndex + directionX;
          int possibleConnectedSpaceCol = colIndex + directionY;
          //while indices are valid and the space matches the rest of the connection
          while((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= numCols - 1) &&
          (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= numRows - 1) &&
          board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) &&
          board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) -> getIsAI() == isAI) {
            numConnected++;
            //If we have found 4 connected chips, return true
            if(numConnected == 4)
              return true;
            //get the next space to check
            possibleConnectedSpaceRow += directionX;
            possibleConnectedSpaceCol += directionY;
          }
          //reverse the direction in case some of the connection is in the opposite direction
          directionX = directionX * -1;
          directionY = directionY * -1;
          //get the next space in the opposite direction to check
          possibleConnectedSpaceRow = rowIndex + directionX;
          possibleConnectedSpaceCol = colIndex + directionY;
          while((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= numCols - 1) &&
          (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= numRows - 1) &&
          board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) &&
          board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) -> getIsAI() == isAI) {
            numConnected++;
            if(numConnected >= 4)
              return true;
            possibleConnectedSpaceRow += directionX;
            possibleConnectedSpaceCol += directionY;
          }
        }
      }
    }
  //no connection of 4 was found in any direction; return false
  return false;
}
