#ifndef CHIP_H
#define CHIP_H

struct Chip {
    public:
      Chip(); //default constructor for Chip
      Chip(bool ia); //parameterized constructor for Chip
      bool getIsAI(); //getter for isAI
      bool getIsSet(); //getter of isSet
      void toggleIsSet(bool is); //setter for isSet

    private:
      bool isAI; //true if the chip is the AIs chip, false otherwise
      bool isSet; //true if this chip and all combinations involving it have been calculated for the search heuristic of a board, false otherwise
};

#endif
