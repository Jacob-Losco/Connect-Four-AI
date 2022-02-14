#include <iostream> //cout
#include <limits> //numeric limit for infinite heuristic
using namespace std;

#include "Gamespace.h"

//definitions used to color and reset color for the chips when printing
#define RED "\033[31m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

Gamespace::Gamespace() {
  //set default gamespace to 6 x 7
  numRows = 6;
  numCols = 7;
  for(int i = 0; i < 7; i++) {
    //set the two vectors that determine the number of plays that can still be made in a column
    availableSpacesToDrop.push_back(5);
    numHypotheticalPlaysInColumn.push_back(0);
  }
  for(int i = 0; i < 6; i++) {
    //set vector with null values and push it into vector of vectors
    vector<Chip*> rowToInsert;
    for(int j = 0; j < 7; j++)
      rowToInsert.push_back(NULL);
    board.push_back(rowToInsert);
  }
}

Gamespace::Gamespace(int nr, int nc) {
  //All same as ^, but with user declare row and column size
  numRows = nr;
  numCols = nc;
  for(int i = 0; i < nc; i++) {
    availableSpacesToDrop.push_back(nr - 1);
    numHypotheticalPlaysInColumn.push_back(0);
  }
  for(int i = 0; i < nr; i++) {
    vector<Chip*> rowToInsert;
    for(int j = 0; j < nc; j++)
      rowToInsert.push_back(NULL);
    board.push_back(rowToInsert);
  }
}

int Gamespace::getNumCols() { return numCols; }

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

void Gamespace::modifyHypotheticalColumnPlays(int colChosen, bool isAdd) {
  isAdd ? numHypotheticalPlaysInColumn[colChosen]++ : numHypotheticalPlaysInColumn[colChosen]--;
}

bool Gamespace::checkValidMove(int possibleColChosen) {
  return availableSpacesToDrop.at(possibleColChosen) - numHypotheticalPlaysInColumn.at(possibleColChosen) >= 0;
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

double Gamespace::calculateFeatureHeuristic(deque<int> playDeque, bool isDefensive) {
  double finalHeuristic = 0;

  //make this Gamespace the hypothetical board
  setHypotheticalBoard(playDeque);

  //iterate through every chip on the board
  for(int i = 0; i < numCols; i++) {
    for(int j = availableSpacesToDrop[i] + 1; j < numRows; j++) {
      //set this chip for all future iterations (so that future chips know everything involving this chip is calculated)
      board.at(j).at(i) -> toggleIsSet(true);
      bool examiningAIChip = board.at(j).at(i) -> getIsAI();
      //calculate feature 4
      int centerOfBoard = numCols % 2 == 0 ? numCols / 2 - .5 : numCols / 2;
      finalHeuristic += board.at(j).at(i) -> getIsAI() != isDefensive ?
                          double(i) != centerOfBoard ? (1 / (abs(double(i) - centerOfBoard))) * numCols : 3 * numCols :
                          -1 * (double(i) != centerOfBoard ? (1 / (abs(double(i) - centerOfBoard))) * numCols : 3 * numCols);

      //examine all valid surrounding spaces
      for(int rowIndex = j - 1; rowIndex <= j + 1; rowIndex++) {
        for(int colIndex = i - 1; colIndex <= i + 1; colIndex++) {
          if((rowIndex >= 0 && rowIndex <= numRows - 1) &&
            (colIndex >= 0 && colIndex <= numCols - 1) &&
            (rowIndex != j || colIndex != i)) {
            //if the adjacent space is empty, check for feature 2 and 3
            if(!board.at(rowIndex).at(colIndex)) {
              bool oneAdjacentSpace = true;
              //get direction of adjacent space
              int directionY = colIndex - i;
              int directionX = rowIndex - j;
              //check for feature 2 heuristic
              //if adjacent move is valid & also empty, increment heuristic by 50,000
              if((rowIndex - directionX >= 0 && rowIndex - directionX <= numRows - 1) &&
                (colIndex - directionY >= 0 && colIndex - directionY <= numCols - 1) &&
                !board.at(rowIndex - directionX).at(colIndex - directionY)) {
                  finalHeuristic += board.at(j).at(i) -> getIsAI() != isDefensive ?
                                    50000 :
                                    -50000;
                  oneAdjacentSpace = false;
              }
              //else, add 10,000 to heuristic for every empty space out, up to 5 spaces
              if(oneAdjacentSpace) {
                int totalSpacesOut = 1;
                int nextSpaceRow = rowIndex;
                int nextSpaceCol = colIndex;
                while(totalSpacesOut <= 5 &&
                  (nextSpaceRow + directionX >= 0 && nextSpaceRow + directionX <= numRows - 1) &&
                  (nextSpaceCol + directionY >= 0 && nextSpaceCol + directionY <= numCols - 1) &&
                  !board.at(rowIndex + directionX).at(colIndex + directionY)) {
                  totalSpacesOut++;
                  finalHeuristic += board.at(j).at(i) -> getIsAI() != isDefensive ?
                                    10000 :
                                    -10000;
                  nextSpaceRow += directionX;
                  nextSpaceCol += directionY;
                }
              }
            }
            //else if adjacent space is another AI chip and that chip has not been calculated already, check for feature 1 and 2
            else if(board.at(rowIndex).at(colIndex) -> getIsAI() == examiningAIChip &&
              !board.at(rowIndex).at(colIndex) -> getIsSet()) {
              int numConnected = 1;
              //get direction of adjacent space
              int directionY = colIndex - i;
              int directionX = rowIndex - j;
              int possibleConnectedSpaceRow = rowIndex + directionX;
              int possibleConnectedSpaceCol = colIndex + directionY;
              int numTimesChipIsSeperatedByPlay = 0;
              int numAdjacentPlaysToConnection = 0;
              while((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= numCols - 1) &&
                (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= numRows - 1) &&
                board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) &&
                board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) -> getIsAI() == examiningAIChip &&
                !board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) -> getIsSet()) {
                numConnected++;
                //if there are four connected, return -1 (inf) as heuristic
                if(numConnected == 4) {
                  bool isAIChip = board.at(j).at(i) -> getIsAI() != isDefensive;

                  finalHeuristic += isAIChip ?
                    numeric_limits<int>::max() :
                    numeric_limits<int>::min();
                }
                possibleConnectedSpaceRow += directionX;
                possibleConnectedSpaceCol += directionY;
              }
              if((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= numCols - 1) &&
                (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= numRows - 1) &&
                !board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol)) {
                if((possibleConnectedSpaceCol + directionY >= 0 && possibleConnectedSpaceCol + directionY <= numCols - 1) &&
                (possibleConnectedSpaceRow + directionX >= 0 && possibleConnectedSpaceRow + directionX <= numRows - 1) &&
                board.at(possibleConnectedSpaceRow + directionX).at(possibleConnectedSpaceCol + directionY) &&
                board.at(possibleConnectedSpaceRow + directionX).at(possibleConnectedSpaceCol + directionY) -> getIsAI() == examiningAIChip) {
                  numTimesChipIsSeperatedByPlay++;
                }
                numAdjacentPlaysToConnection++;
              }
              //reverse the direction in case some of the connection is in the opposite direction
              directionX = directionX * -1;
              directionY = directionY * -1;
              possibleConnectedSpaceRow = rowIndex + (directionX * 2);
              possibleConnectedSpaceCol = colIndex + (directionY * 2);
              while((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= numCols - 1) &&
                (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= numRows - 1) &&
                board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) &&
                board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) -> getIsAI() == examiningAIChip &&
                !board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol) -> getIsSet()) {
                numConnected++;
                if(numConnected == 4) {
                  bool isAIChip = board.at(j).at(i) -> getIsAI() != isDefensive;

                  finalHeuristic += isAIChip ?
                    numeric_limits<int>::max() :
                    numeric_limits<int>::min();
                }
                possibleConnectedSpaceRow += directionX;
                possibleConnectedSpaceCol += directionY;
              }
              if((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= numCols - 1) &&
                (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= numRows - 1) &&
                !board.at(possibleConnectedSpaceRow).at(possibleConnectedSpaceCol)) {
                if((possibleConnectedSpaceCol + directionY >= 0 && possibleConnectedSpaceCol + directionY <= numCols - 1) &&
                (possibleConnectedSpaceRow + directionX >= 0 && possibleConnectedSpaceRow + directionX <= numRows - 1) &&
                board.at(possibleConnectedSpaceRow + directionX).at(possibleConnectedSpaceCol + directionY) &&
                board.at(possibleConnectedSpaceRow + directionX).at(possibleConnectedSpaceCol + directionY) -> getIsAI() == examiningAIChip) {
                  numTimesChipIsSeperatedByPlay++;
                }
                numAdjacentPlaysToConnection++;
              }
              if(numConnected == 2) {
                finalHeuristic += board.at(j).at(i) -> getIsAI() != isDefensive ?
                                  900000 * numTimesChipIsSeperatedByPlay :
                                  -1 * (900000 * numTimesChipIsSeperatedByPlay);
                if(numAdjacentPlaysToConnection == 2)
                  finalHeuristic += board.at(j).at(i) -> getIsAI() != isDefensive ?
                                    450000 :
                                    -450000;
              }
              else if(numConnected == 3) {
                if(numAdjacentPlaysToConnection == 2) {
                  bool isAIChip = board.at(j).at(i) -> getIsAI() != isDefensive;

                  finalHeuristic += isAIChip ?
                    numeric_limits<int>::max() :
                    numeric_limits<int>::min();
                }
                else if(numAdjacentPlaysToConnection == 1)
                  finalHeuristic += board.at(j).at(i) -> getIsAI() != isDefensive ?
                                    900000 :
                                    -900000;
              }
            }
          }
        }
      }
    }
  }

  //reverts the board back to the original gamespace
  revertHypotheticalBoard(playDeque);
  unsetBoard();

  return finalHeuristic;
}

void Gamespace::unsetBoard() {
  for(int i = 0; i < numCols; i++)
    for(int j = availableSpacesToDrop[i] + 1; j < numRows; j++) {
      board.at(j).at(i) -> toggleIsSet(false);
    }
}

void Gamespace::setHypotheticalBoard(deque<int> playDeque) {
  int numHypotheticalPlays = playDeque.size();
  bool isHypotheticalAIPlay = true;
  for(int i = 0; i < numHypotheticalPlays; i++) {
    int play = playDeque.back();
    //get the row position that the chip will drop into, then set the next available row to be the one above it
    int rowChosen = availableSpacesToDrop.at(play);
    availableSpacesToDrop.at(play)--;

    //set the chip at the position as the new value for this space
    board.at(rowChosen).at(play) = new Chip(isHypotheticalAIPlay);
    isHypotheticalAIPlay = isHypotheticalAIPlay ? false : true;

    playDeque.pop_back();
    playDeque.push_front(play);
  }
}

void Gamespace::revertHypotheticalBoard(deque<int> playDeque) {
  int numHypotheticalPlays = playDeque.size();
  for(int i = 0; i < numHypotheticalPlays; i++) {
    int play = playDeque.front();

    availableSpacesToDrop.at(play)++;
    delete board.at(availableSpacesToDrop.at(play)).at(play);
    board.at(availableSpacesToDrop.at(play)).at(play) = NULL;

    playDeque.pop_front();
    playDeque.push_back(play);
  }
}
