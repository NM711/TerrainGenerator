#include "./grid.hpp"

void GridBuilder::setSize(int rows, int cols) {
  this->rows = rows;
  this->cols = cols;
  this->grid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, this->value));
};

void GridBuilder::setPopValue(unsigned int value) {
  this->value = value;
};

int GridBuilder::getRows() {
  return this->rows;
};

int GridBuilder::getCols() {
  return this->cols;
};

Grid GridBuilder::build() {
  return this->grid;
};
