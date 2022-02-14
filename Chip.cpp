#include "Chip.h"

Chip::Chip() {
  isAI = false;
  isSet = false;
}

Chip::Chip(bool ia) {
  isAI = ia;
  isSet = false;
}

bool Chip::getIsAI() { return isAI; }

bool Chip::getIsSet() { return isSet; }

void Chip::toggleIsSet(bool is) { isSet = is; }
