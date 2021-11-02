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

char** Gamespace::getBoard() {
    return board;
}

void Gamespace::printBoard() {
    cout << "BOARD: " << endl;
    for(int i = 0; i < dim * 2; i++) {
        cout << "_";
    }
    cout << endl;
    for(int i = 0; i < dim; i++) {
        cout << "|";
        for(int j = 0; j < dim; j++) {
            if(board[i][j] == 'n')
                cout << "0 ";
            else if(board[i][j] == 'r')
                cout << RED << "X " << RESET;
            else
                cout << BLUE << "X " << RESET;
        }
        cout << "|" << endl;
    }
}

bool checkWin(int row, int col) {

}