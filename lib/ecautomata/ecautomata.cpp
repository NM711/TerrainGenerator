#include <iostream>
#include "./ecautomata.hpp"

void ECAutomata::setState(unsigned int state) {
  auto it = this->neighborStateCountMap.find(state);
  if (it != this->neighborStateCountMap.end()) {
    std::cout << "State "
              << "\"" << state << "\" already exists in set!" << std::endl;
    exit(1);
  };

  this->neighborStateCountMap[state] = 0;
};

void ECAutomata::resetNeighborStateMap() {
  for (auto &it : this->neighborStateCountMap) {
    it.second = 0;
  };
};

void ECAutomata::setNeighborhoodPosition(NeighborhoodPosition np) {
  this->neighborhoodPosition = np;
};

void ECAutomata::initGrid(int rowSize, int colSize, unsigned int popValue) {
  this->builder.setSize(rowSize, colSize);
  this->builder.setPopValue(popValue);
  this->grid = this->builder.build();
};

int ECAutomata::computePositionalOffset(NeighborSpecification &spec) {
  return spec.end - spec.start;
};

NeighborhoodPositionOffset ECAutomata::computeNeighborhoodOffsets() {
  return {
    this->computePositionalOffset(this->neighborhoodPosition.top),
    this->computePositionalOffset(this->neighborhoodPosition.topLeft),
    this->computePositionalOffset(this->neighborhoodPosition.topRight),
    this->computePositionalOffset(this->neighborhoodPosition.left),
    this->computePositionalOffset(this->neighborhoodPosition.right),
    this->computePositionalOffset(this->neighborhoodPosition.bottom),
    this->computePositionalOffset(this->neighborhoodPosition.bottomLeft),
    this->computePositionalOffset(this->neighborhoodPosition.bottomRight)
  };
};

void ECAutomata::computeNeighbors(Position pos, NeighboringCellStateCount &neighborStateCount) {
  NeighborhoodPositionOffset offsets = this->computeNeighborhoodOffsets();

  bool topValid = false;
  bool bottomValid = false;

  // Make sure that the position has enough space to perform left hand checks in the grid, make sure that the row position
  // Does not go out of bounds.
  
  if (offsets.top > 0 && pos.row > offsets.top && pos.row < this->builder.getRows()) {
    for (int i = 1; i <= offsets.top; ++i) {
      int top = this->grid.at(pos.row - i).at(pos.col);
      neighborStateCount[top] = ++neighborStateCount[top];
    };

    topValid = true;
  };

  // See if we have enough space to expand downwards.
    
  if (offsets.bottom > 0 && pos.row < this->builder.getRows() - offsets.bottom) {
    for (int i = 1; i <= offsets.top; ++i) {
      int top = this->grid.at(pos.row + i).at(pos.col);
      neighborStateCount[top] = ++neighborStateCount[top];
    };

    bottomValid = true;
  };

  if (offsets.left > 0 && pos.col > offsets.left && pos.col < this->builder.getCols()) {
    for (int i = 1; i <= offsets.topLeft; ++i) {
      int cell = this->grid.at(pos.row).at(pos.col - i);
      neighborStateCount[cell] = ++neighborStateCount[cell];
    };
  };

  if (offsets.right > 0 && pos.col < this->builder.getCols() - offsets.right) {
    for (int i = 1; i <= offsets.topLeft; ++i) {
      int cell = this->grid.at(pos.row).at(pos.col + i);
      neighborStateCount[cell] = ++neighborStateCount[cell];
    };
  };

  if (topValid) {
    if (offsets.topLeft > 0 && pos.col > offsets.topLeft && pos.col < this->builder.getCols()) {
      for (int i = 1; i <= offsets.topLeft; ++i) {
        int cell = this->grid.at(pos.row - offsets.top).at(pos.col - i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };

    if (offsets.topRight > 0 && pos.col < this->builder.getCols() - offsets.topRight) {
      for (int i = 1; i <= offsets.topRight; ++i) {
        int cell = this->grid.at(pos.row - offsets.top).at(pos.col + i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };
  };

  if (bottomValid) {
    if (offsets.bottomLeft > 0 && pos.col > offsets.bottomLeft && pos.col < this->builder.getCols()) {
      for (int i = 1; i <= offsets.bottomLeft; ++i) {
        int cell = this->grid.at(pos.row + offsets.bottomLeft).at(pos.col - i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };

    if (offsets.bottomRight > 0 && pos.col < this->builder.getCols() - offsets.bottomRight) {
      for (int i = 1; i <= offsets.bottomRight; ++i) {
        int cell = this->grid.at(pos.row + offsets.bottomRight).at(pos.col + i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };
  };
};
