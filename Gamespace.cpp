#include <iostream>
using namespace std;

#include "Gamespace.h"

#define RED "\033[31m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

Gamespace::Gamespace() {
    for(int i = 0; i < 6; i++) {
        availableSpacesToDrop[i] = 5;
        for(int j = 0; j < 6; j++)
            board[i][j] = 'n';
    }
}

void Gamespace::printBoard() {
    cout << "BOARD: " << endl;
    for(int i = 0; i < 6 * 3; i++) {
        cout << "_";
    }
    cout << endl;
    for(int i = 0; i < 6; i++) {
        cout << "|";
        for(int j = 0; j < 6; j++) {
            if(board[i][j] == 'n')
                cout << " X ";
            else if(board[i][j] == 'r')
                cout << RED << " X " << RESET;
            else
                cout << BLUE << " X " << RESET;
        }
        cout << "|" << endl;
    }
    for(int i = 0; i < 6 * 3; i++) {
        cout << "_";
    }
}

bool Gamespace::checkValidMove(int possibleColChosen) {
    return availableSpacesToDrop[possibleColChosen] != -1;
}

bool Gamespace::dropChip(char player, int colChosen) {
    //get the row position that the chip will drop into, then set the next available row to be the one above it
    int rowChosen = availableSpacesToDrop[colChosen];
    availableSpacesToDrop[colChosen]--;

    //set the player char as the new value for this space
    board[rowChosen][colChosen] = player;
    //check for a win condition
    for(int rowIndex = rowChosen - 1; rowIndex <= rowChosen + 1; rowIndex++) {
        for(int colIndex = colChosen - 1; colIndex <= colChosen + 1; colIndex++) {
            if((rowIndex >= 0 && rowIndex <= 5) && 
            (colIndex >= 0 && colIndex <= 5) &&
            board[rowIndex][colIndex] == player && 
            (rowIndex != rowChosen && colIndex != colChosen)) {
                int numConnected = 2; //the number of chips we have connected so far. Start with two, since we have the origonal dropped chip, and the surrounding one we found
                //get the direction we have to step in to find the rest of the connection
                int directionX = colIndex - colChosen; 
                int directionY = rowIndex - rowChosen;
                //get the next space to check
                int possibleConnectedSpaceRow = rowIndex + directionX;
                int possibleConnectedSpaceCol = colIndex + directionY;
                //while indices are valid and the space matches the rest of the connections
                while((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= 5) &&
                (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= 5) && 
                board[possibleConnectedSpaceRow][possibleConnectedSpaceCol] == player) {
                    numConnected++;
                    //If we have found 4 connected chips, return true
                    if(numConnected == 4)
                        return true;
                    //get the next space to check
                    possibleConnectedSpaceRow += directionX;
                    possibleConnectedSpaceCol += directionY;
                }
                //reverse the direction incase some the connection is in the opposite direction
                directionX = directionX * -1;
                directionY = directionY * -1;
                //get the next space in the opposite direction to check
                possibleConnectedSpaceRow = rowIndex + directionX;
                possibleConnectedSpaceCol = colIndex + directionY;
                while((possibleConnectedSpaceCol >= 0 && possibleConnectedSpaceCol <= 5) &&
                (possibleConnectedSpaceRow >= 0 && possibleConnectedSpaceRow <= 5) && 
                board[possibleConnectedSpaceRow][possibleConnectedSpaceCol] == player) {
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