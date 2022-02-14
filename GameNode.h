#ifndef GAMENODE_H
#define GAMENODE_H

#include <deque>

#include "Gamespace.h"

struct GameNode {
  public:
    GameNode(); //default constructor for GameNode
    GameNode(Gamespace* g, std::deque<int> pd, GameNode* pn, bool im, int depth); //parameterized constructor for GameNode
    double getValue(); //getter for value
    bool getAlphaBetaCutoff(); //getter for alphaBetaCutoff
    int getMax(); //getter for max
    int getMin(); //getter for min
    int getChosenMove(); //getter for chosenMove
    GameNode* getParentNode(); //getter for parentNode
  private:
    double value; //the heuristic value for the hypthetical board this node represents
    //MEMBER VARIABLES FOR NON-ROOT NODES
    GameNode* parentNode; //points to the parent of this node. Used for alpha beta
    bool alphaBetaCutoff; //true if the node is an alpha beta cutoff, false otherwise
    //MEMBER VARIABLES FOR NON-LEAF NODES
    bool isMax; //whether or not this node is a max node
    int max; //the current maximum heuristic value from a nodes children
    int min; //the current minimum heuristic value from a nodes chidren
    int chosenMove; //the current move that this node would chose given max, min
};

#endif
