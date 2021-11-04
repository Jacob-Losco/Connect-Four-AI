#include <iostream>
#include <limits>
using namespace std;

#include "Gamespace.h"

int main() {
    Gamespace* g = new Gamespace();
    int numTurns = 0;
    bool finished;
    string player;
    cout << "Welcome to Connect Four!" << endl;
    g -> printBoard();
    while(!finished) {
        if(numTurns == 36) {
            player = "Tie";
            finished = true;
            break;
        }
        player = numTurns % 2 == 0 ? "Player 1" : "Player 2";
        int chosenCol;
        bool chosenColValid = false;
        while(!chosenColValid) {
            cout << player << ", choose a number 1-6 for the column you want to drop a chip into: ";
            if(!(cin >> chosenCol)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Bad Input. Please enter a number... " << endl;
            }
            else if(chosenCol < 1 && chosenCol > 6)
                cout << "Invalid column number. Please enter a number between 1 and 6... " << endl;
            else if(g -> checkValidMove(chosenCol - 1)) {
                chosenColValid = true;
                break;
            }
            cout << "Invalid move. This column is already full... " << endl;
        }
        finished = g -> dropChip(numTurns % 2 == 0 ? 'r' : 'b', chosenCol - 1);
        g -> printBoard();
        numTurns++;
    }
    if(player == "Tie")
        cout << "It was a Tie!" << endl;
    else
        cout << player << " Wins!" << endl;
    
}