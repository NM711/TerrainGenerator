#include "./generator.hpp"
#include <chrono>
#include <iostream>
#include <thread>

typedef std::map<unsigned int, std::string> CellStateASCIIMap;

static CellStateASCIIMap asciimap = {
  {State::DIRT, BROWN + std::string("&") + DEFAULT},
  {State::GRASS, DARK_GREEN + std::string("#") + DEFAULT},
  {State::WATER, BLUE + std::string("%") + DEFAULT},
  {State::STONE, GRAY + std::string("0") + DEFAULT},
  {State::PLANT_BUSH, GREEN + std::string("\"") + DEFAULT},
  {State::PLANT_TREE, GREEN + std::string("¥") + DEFAULT},
  {State::PLANT_FLOWER, DARK_RED + std::string("@") + DEFAULT},
  {State::CRYSTAL, LIGHT_YELLOW + std::string("1") + DEFAULT}
};

void printGrid(Grid &grid, int rows, int cols) {
  system("clear");
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      int cell = grid.at(row).at(col);
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
  generator.seed();
  generator.setGridDraw(printGrid);
  generator.execute();
};
