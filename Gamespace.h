#ifndef Gamespace
#define Gamespace

struct Gamespace {
    public:
        Gamespace(int dim); //constructor for Gamespace
        char** getBoard(); //returns the board
        void printBoard(); //prints the board to console
        bool dropChip(char player, int rowChosen); //sets the matrix[rowChosen][availableSpacesToDrop[rowChosen]] to player. Returns a bool for whether the win condition has been met
        bool checkWin(int row, int col); //returns true if the new move at board[row][col] causes a player to win the game, false otherwise
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