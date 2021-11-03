#ifndef GAMESPACE_H
#define GAMESPACE_H

struct Gamespace {
    public:
        Gamespace(); //constructor for Gamespace
        //postcondition: prints the board to console
        void printBoard();
        //precondition: int possibleColChosen is the column that a player wants to drop a chip in
        //postcondition: returns true if this is a valid move, false otherwise
        bool checkValidMove(int possibleColChosen);
        //precondition: player is either 'r' or 'b', colChosen is a valid index in availableSpacesToDrop
        //postcondition: sets the matrix[rowChosen][availableSpacesToDrop[rowChosen]] to player. Returns a bool for whether the win condition has been met
        bool dropChip(char player, int colChosen); 
    private:
        //board used for maintaining the gamespace.
            //n = no chip.
            //r = AI (player 1) chip here.
            //b = other player (player 2) chip here
        char board[6][6];
        //availableSpacesToDrop[i] returns the column number for the position where the chip would land if the user chose row 'i'.
            //-1 = this column is completely filled up
        int availableSpacesToDrop[6];
};

#endif