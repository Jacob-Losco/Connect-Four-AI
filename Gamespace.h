#ifndef GAMESPACE_H
#define GAMESPACE_H

#include "Chip.h"

#include <vector>
#include <deque>

struct Gamespace {
    public:
        Gamespace(); //default constructor for Gamespace
        Gamespace(int numRows, int numCols); //parameterized constructor for Gamespace
        //postcondition: prints the board to console
        int getNumCols(); //getter for numCols
        void printBoard();
        //precondition: colChosen is a column where a hypothetical play is/was made
          //isAdd is whether or not the col needs to be incremented or decremented
        //postcondition: update hypotheticalColumnPlays vector
        void modifyHypotheticalColumnPlays(int colChosen, bool isAdd);
        //precondition: possibleColChosen is the column that a player wants to drop a chip in
        //postcondition: returns true if this is a valid move, false otherwise
        bool checkValidMove(int possibleColChosen);
        //precondition: isAI represents who played the chip, colChosen is a valid index in availableSpacesToDrop
        //postcondition: sets the matrix[rowChosen][availableSpacesToDrop[rowChosen]] to player. Returns a bool for whether or not a win condition has been met
        bool dropChip(bool isAI, int colChosen);
        //precondition: rowChosen and colChosen are positions where a chip was dropped
        //postcondition: returns whether or not there is a win
        bool checkWin(int rowChosen, int colChosen, bool isAI);
        //precondition: playDeque is a deque representing, front to back, the plays that need to be made in order to get from the current gameboard to a hypothetical board that needs to be calculated.
          //isDefensive is true if the computer is playing defensive, false otherwise
        //postcondition: returns an int representing the total heuristic score of the hypothetical board represented by this gamespace and playDeque
        double calculateFeatureHeuristic(std::deque<int> playDeque, bool isDefensive); //TODO --------------------------------------------------------------------------------------------------------------------------------------------

    private:
        //precondition: playDeque is a deque representing, front to back, the plays that need to be made in order to get from the current gameboard to a hypothetical board that needs to be calculated.
        //postcondition: alters the gamespace to represent the current board with additional plays
        void setHypotheticalBoard(std::deque<int> playDeque);
        //precondition: playDeque is a deque representing, front to back, the plays that need to be made in order to get from the current gameboard to a hypothetical board that needs to be calculated.
        //postcondition: reverts a hypothetical board back to the origonal
        void revertHypotheticalBoard(std::deque<int> playDeque);
        //postcondition: unsets all chips on the board
        void unsetBoard();
        //number of rows in the board
        int numRows;
        //number of columns in the board
        int numCols;
        //2d vector used for representing the board.
        std::vector<std::vector<Chip*>> board;
        //availableSpacesToDrop[i] returns the row number for the position where the chip would land if the user chose column 'i'.
            //-1 = this column is completely filled up
        std::vector<int> availableSpacesToDrop;
        //numHypotheticalPlaysInColumn[i] returns the number of hypothetical moves in a column
        std::vector<int> numHypotheticalPlaysInColumn;
};

#endif
