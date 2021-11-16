#ifndef GAMENODE_H
#define GAMENODE_H

#include "Gamespace.h"

#include <deque>

struct GameNode {
  public:
    GameNode(); //default constructor for GameNode
    GameNode(Gamespace* g, std::deque<int> pd, bool im, int depth); //parameterized constructor for GameNode
    int getValue(); //getter for value
    void setValue(int v); //setter fo value
    bool getIsMax(); //getter for isMax
    int getMax(); //getter for max
    void setMax(int m); //setter for max
    int getMin(); //getter for min
    void setMin(int m); //setter for min
    int getChosenMove(); //getter for chosenMove
    void setChosenMove(int cm); //setter for chosenMove
    GameNode* getParentNode(); //getter for parentNode
  private:
    int value; //the heuristic value for the hypthetical board this node represents
    //MEMBER VARIABLES FOR NON-ROOT NODES
    GameNode* parentNode; //points to the parent of this node. Used for alpha beta
    //MEMBER VARIABLES FOR NON-LEAF NODES
    bool isMax; //whether or not this node is a max node
    int max; //the current maximum heuristic value from a nodes children
    int min; //the current minimum heuristic value from a nodes chidren
    int chosenMove; //the current move that this node would chose given max, min
    GameNode* currentChild; //points to the currentChild that we are calculating heuristic values for.
};

#endif
