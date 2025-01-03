#include "./ecautomata.hpp"
#include "util/grid.hpp"
#include <iostream>

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

void ECAutomata::setScaleFactor(float rfactor, float cfactor, std::set<unsigned int> *rareStateSet) {
  Grid freshGrid;
  freshGrid.setConfig(static_cast<int>(this->grid.getRows() * rfactor), static_cast<int>(this->grid.getCols() * cfactor), 0);
  // A majority rules algorithm is biased towards dominant states in a neighborhood.
  // The only way to combat this is to introduce a map or set, that allows us to view which states are explicitly deemed
  // "rare" by the user. "rare" states will be given a multiplier that will allow them to compete against a obviously
  // dominant common state neighborhood

  for (int row = 0; row < freshGrid.getRows(); ++row) {
    for (int col = 0; col < freshGrid.getCols(); ++col) {
      int originalRow = static_cast<int>(row / rfactor);
      int originalCol = static_cast<int>(col / cfactor);

      this->computeNeighbors({originalRow, originalCol}, this->neighborStateCountMap);

      unsigned int majorityState = 0;
      unsigned int majorityCount = 0;

      for (auto [state, count] : this->neighborStateCountMap) {
        if (rareStateSet != nullptr && rareStateSet->find(state) != rareStateSet->end()) {
          count = count * 2;
        };

        if (count > majorityCount) {
          majorityState = state;
          majorityCount = count;
        };
      };

      freshGrid.getContent()->at(row).at(col) = majorityState;

      // iterate through each state and find the one that has the most neighbors.
      this->resetNeighborStateMap();
    };
  };

  this->grid.setContent(*freshGrid.getContent());
};

void ECAutomata::initGrid(int rowSize, int colSize, unsigned int popValue) {
  this->grid.setConfig(rowSize, colSize, popValue);
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

  if (offsets.top > 0 && pos.row > offsets.top && pos.row < this->grid.getRows()) {
    for (int i = 1; i <= offsets.top; ++i) {
      int top = this->grid.getContent()->at(pos.row - i).at(pos.col);
      neighborStateCount[top] = ++neighborStateCount[top];
    };

    topValid = true;
  };

  // See if we have enough space to expand downwards.

  if (offsets.bottom > 0 && pos.row < this->grid.getRows() - offsets.bottom) {
    for (int i = 1; i <= offsets.top; ++i) {
      int top = this->grid.getContent()->at(pos.row + i).at(pos.col);
      neighborStateCount[top] = ++neighborStateCount[top];
    };

    bottomValid = true;
  };

  if (offsets.left > 0 && pos.col > offsets.left && pos.col < this->grid.getCols()) {
    for (int i = 1; i <= offsets.topLeft; ++i) {
      int cell = this->grid.getContent()->at(pos.row).at(pos.col - i);
      neighborStateCount[cell] = ++neighborStateCount[cell];
    };
  };

  if (offsets.right > 0 && pos.col < this->grid.getCols() - offsets.right) {
    for (int i = 1; i <= offsets.topLeft; ++i) {
      int cell = this->grid.getContent()->at(pos.row).at(pos.col + i);
      neighborStateCount[cell] = ++neighborStateCount[cell];
    };
  };

  if (topValid) {
    if (offsets.topLeft > 0 && pos.col > offsets.topLeft && pos.col < this->grid.getCols()) {
      for (int i = 1; i <= offsets.topLeft; ++i) {
        int cell = this->grid.getContent()->at(pos.row - offsets.top).at(pos.col - i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };

    if (offsets.topRight > 0 && pos.col < this->grid.getCols() - offsets.topRight) {
      for (int i = 1; i <= offsets.topRight; ++i) {
        int cell = this->grid.getContent()->at(pos.row - offsets.top).at(pos.col + i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };
  };

  if (bottomValid) {
    if (offsets.bottomLeft > 0 && pos.col > offsets.bottomLeft && pos.col < this->grid.getCols()) {
      for (int i = 1; i <= offsets.bottomLeft; ++i) {
        int cell = this->grid.getContent()->at(pos.row + offsets.bottomLeft).at(pos.col - i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };

    if (offsets.bottomRight > 0 && pos.col < this->grid.getCols() - offsets.bottomRight) {
      for (int i = 1; i <= offsets.bottomRight; ++i) {
        int cell = this->grid.getContent()->at(pos.row + offsets.bottomRight).at(pos.col + i);
        neighborStateCount[cell] = ++neighborStateCount[cell];
      };
    };
  };
};
