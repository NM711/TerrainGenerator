#include "./generator.hpp"
#include <chrono>
#include <iostream>
#include <thread>

typedef std::map<unsigned int, std::string> CellStateASCIIMap;

static CellStateASCIIMap asciimap = {
  {State::DIRT, "\e[38;5;130m" + std::string("&") + "\e[0;0m"},
  {State::GRASS, "\e[38;5;76m"+ std::string("#") + "\e[0;0m"},
  {State::WATER, "\e[38;5;45m" + std::string("%") + "\e[0;0m"},
  {State::STONE, "\e[38;5;240m" + std::string("0") + "\e[0;0m"},
  {State::PLANT_BUSH, "\e[38;5;100m" + std::string("\"") + "\e[0;0m"},
  {State::PLANT_TREE, "\e[38;5;100m"+ std::string("¥") + "\e[0;0m"},
  {State::PLANT_FLOWER, "\e[38;5;88m" + std::string("@") + "\e[0;0m"},
  {State::CRYSTAL, "\e[38;5;190m"+ std::string("1") + "\e[0;0m"}
};

void printGrid(Grid &grid, int rows, int cols) {
  system("clear");
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      int cell = grid.getContent()->at(row).at(col);
      auto iterator = asciimap.find(cell);
      if (iterator == asciimap.end()) {
        std::cout << "Given cell state of \"" << cell << "\" does not exist in the provided map!" << std::endl;
        exit(1);
      };
      std::cout << asciimap[cell];
    };
    std::cout << std::endl;
  };

  std::this_thread::sleep_for(std::chrono::milliseconds(5));
};


int main() {
  TerrainGenerator generator;
  generator.initGrid(36, 154, State::DIRT);
  int generatedSeed = generator.seed();
  generator.setGridDraw(printGrid);
  generator.execute();
};
