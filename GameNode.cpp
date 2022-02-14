#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include "GameNode.h"

GameNode::GameNode() {
  value = 0;
  isMax = 0;
  max = 0;
  min = 0;
  chosenMove = -1;
}

GameNode::GameNode(Gamespace* g, std::deque<int> pd, GameNode* pn, bool im, int depth) {
  //set member variables parentNode, isMax and alphaBetaCutoff
  parentNode = pn;
  isMax = !im;
  alphaBetaCutoff = false;
  //if the max depth of the search has not yet been reached, determine value from all valid children
  if(depth != 0) {
    max = 0;
    min = 0;
    bool cutoff = false;
    //for every column choice to be made, or until there is an alpha beta cutoff
    for(int i = 0; i < g -> getNumCols() && !cutoff; i++) {
      //check to see if the column is not full
      if(g -> checkValidMove(i)) {
        //push this move onto the playDeque
        pd.push_front(i);
        g -> modifyHypotheticalColumnPlays(i, true);
        //recursively create the child node associated with the playDeque
        GameNode currentChild = GameNode(g, pd, this, !im, depth - 1);
        //calculate new maximum and minimum heuristics for this node given this child's value
        if(max == 0 && min == 0) {
          max = min = currentChild.getValue();
          chosenMove = i;
        }
        else {
          if(currentChild.getValue() > max) {
            max = currentChild.getValue();
            if(isMax) chosenMove = i;
          }
          else if(currentChild.getValue() == max && isMax) {
            srand(time(NULL));
            if(rand() % 1)
              chosenMove = i;
          }
          if(currentChild.getValue() < min) {
            min = currentChild.getValue();
            if(!isMax) chosenMove = i;
          }
          else if(currentChild.getValue() == min && !isMax) {
            srand(time(NULL));
            if(rand() % 1)
              chosenMove = i;
          }
        }
        //check to see if there was an alpha beta cutoff
        cutoff = currentChild.getAlphaBetaCutoff();
        //pop this move off of the deque
        pd.pop_front();
        g -> modifyHypotheticalColumnPlays(i, false);
      }
    } //CHILD NODE IS FREED FROM MEMORY HERE
    value = isMax ? max : min;
  }
  //else, calculate heuristic for this node
  else
    value = g -> calculateFeatureHeuristic(pd, !im);

  //determine if this node causes an alpha beta cutoff
  if((!(!parentNode) && !(!parentNode -> getParentNode())))
    if(isMax ? parentNode -> getParentNode() -> getMin() && value < parentNode -> getParentNode() -> getMin() :
      parentNode -> getParentNode() -> getMax() && (value > parentNode -> getParentNode() -> getMax())) {
        alphaBetaCutoff = true;
      }
}

double GameNode::getValue() { return value; }

bool GameNode::getAlphaBetaCutoff() { return alphaBetaCutoff; }

int GameNode::getMax() { return max; }

int GameNode::getMin() { return min; }

int GameNode::getChosenMove() { return chosenMove; }

GameNode* GameNode::getParentNode() { return parentNode; }
