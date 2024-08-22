#include "./generator.hpp"
#include <random>

// Simple terrain generator. I plan on making it a tool I can use in some side personal games. Still a work in progress
// though. Multistate

struct TerrainRuleset : CustomGridRuleset<Seeder> {
    void apply(int currentCell, int *cellToUpdate, NeighboringCellStateCount &stateCount) override {
      if (currentCell == State::DIRT && (stateCount[State::WATER] == 1 && stateCount[State::DIRT] > 4)) {
        *cellToUpdate = State::GRASS;
      } else if (currentCell == State::DIRT && (stateCount[State::GRASS] == 3 || stateCount[State::GRASS] == 4)) {
        *cellToUpdate = State::GRASS;
      } else if ((currentCell == State::DIRT || currentCell == State::GRASS) && stateCount[State::WATER] > 4) {
        *cellToUpdate = State::WATER;
      } else if ((currentCell == State::DIRT || currentCell == State::GRASS) && stateCount[State::WATER] == 1 &&
                 this->dependency->getDistributionOutput(*this->distribution) < 1) {
        *cellToUpdate = State::WATER;
      } else if (currentCell == State::WATER && stateCount[State::WATER] < 6 &&
                 stateCount[State::DIRT] + stateCount[State::GRASS] > 6 &&
                 this->dependency->getDistributionOutput(*this->distribution) < 5) {
        *cellToUpdate = State::DIRT;
      } else if (currentCell == State::DIRT && stateCount[State::STONE] >= 1 &&
                 this->dependency->getDistributionOutput(*this->distribution) < 3) {
        *cellToUpdate = State::STONE;
      } else {
        *cellToUpdate = currentCell;
      };
    };
};

struct EnvironmentRuleset : CustomGridRuleset<Seeder> {
    void apply(int currentCell, int *cellToUpdate, NeighboringCellStateCount &stateCount) override {
      if (currentCell == State::GRASS && stateCount[State::GRASS] > 4) {
        int outcome = this->dependency->getDistributionOutput(*this->distribution);

        if (outcome < 10) {
          *cellToUpdate = State::PLANT_TREE;
        } else if (outcome > 10 && outcome < 20) {
          *cellToUpdate = State::PLANT_BUSH;
        } else if (outcome > 20 && outcome < 30) {
          *cellToUpdate = State::PLANT_FLOWER;
        } else {
          *cellToUpdate = currentCell;
        };

      } else if (currentCell == State::STONE && stateCount[State::STONE] > 6 &&
                 this->dependency->getDistributionOutput(*this->distribution) < 20) {
        *cellToUpdate = State::CRYSTAL;
      } else {
        *cellToUpdate = currentCell;
      };
    };
};

TerrainGenerator::TerrainGenerator() {
  this->draw = nullptr;
  this->setNeighborhoodPosition({{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}});
  // Enum States - There is 8 states
  for (int i = 0; i < 8; ++i) {
    this->setState(i);
  };
};

Grid TerrainGenerator::getGrid() {
  return this->grid;
};

void TerrainGenerator::setGridDraw(void (*draw)(Grid &grid, int rows, int cols)) {
    this->draw = draw;
};

void TerrainGenerator::seed(int num) {
  this->seeder.setSeed(num);
};

int TerrainGenerator::seed() {
  return  this->seeder.setRandomSeed();
};

void TerrainGenerator::execute() {
  this->prepopulate();
  std::uniform_int_distribution<int> dist(0, 100);

  for (int i = 0; i <= 500; ++i) {

    if (this->draw != nullptr) {
      this->draw(this->grid, this->builder.getRows(), this->builder.getCols());
    };

    if (i < 499) {
      TerrainRuleset terrainRules;
      terrainRules.inject(&this->seeder);
      terrainRules.setDistribution(&dist);

      // Pass #1 Terrain Generation
      this->pass<Seeder>(terrainRules);
    } else {
      EnvironmentRuleset environmentRules;
      environmentRules.inject(&this->seeder);
      environmentRules.setDistribution(&dist);
      // Do one step environment growth once the terrain has been generated.
      // Pass #2 EnvironmentGeneration
      this->pass<Seeder>(environmentRules);
    };
  };
};

void TerrainGenerator::prepopulate() {
  std::uniform_int_distribution<int> rowDistRange = std::uniform_int_distribution<int>(0, this->builder.getRows());
  std::uniform_int_distribution<int> colDistRange = std::uniform_int_distribution<int>(0, this->builder.getCols());

  for (int i = 0; i <= 10; ++i) {
    Position pos(this->seeder.getDistributionOutput(rowDistRange), this->seeder.getDistributionOutput(colDistRange));
    if (pos.col > 1 && pos.col < this->builder.getCols() - 1 && pos.row > 1 && pos.row < this->builder.getRows() - 1) {
      if (i < 6) {
        this->grid.at(pos.row).at(pos.col) = State::WATER;
        this->grid.at(pos.row).at(pos.col - 1) = State::WATER;
        this->grid.at(pos.row).at(pos.col + 1) = State::WATER;
        this->grid.at(pos.row - 1).at(pos.col - 1) = State::WATER;
        this->grid.at(pos.row - 1).at(pos.col + 1) = State::WATER;
        this->grid.at(pos.row + 1).at(pos.col - 1) = State::WATER;
        this->grid.at(pos.row + 1).at(pos.col + 1) = State::WATER;
      } else {
        this->grid.at(pos.row).at(pos.col) = State::STONE;
        this->grid.at(pos.row).at(pos.col + 1) = State::STONE;
      };
    };
  };
};
