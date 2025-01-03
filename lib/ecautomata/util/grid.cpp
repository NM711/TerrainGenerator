#include "./grid.hpp"

void Grid::setConfig(int rows, int cols, unsigned int initialPopValue) {
  this->content = std::vector<std::vector<int>>(rows, std::vector<int>(cols, initialPopValue));
};

void Grid::setContent(std::vector<std::vector<int>> content) {
  this->content = content;
};

std::vector<std::vector<int>> *Grid::getContent() {
  return &this->content;
};

int Grid::getRows() {
  return this->content.size();
};

int Grid::getCols() {
  return this->content.at(0).size();
};
