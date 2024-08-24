#ifndef ASCII_TERRAIN_GENERATOR_HPP
#define ASCII_TERRAIN_GENERATOR_HPP

#include "lib/ecautomata/ecautomata.hpp"

/*
  Rules:

  VEGETATION:

    1. If current cell is dirt and neighbors are more than 4 dirt and one 1 water cells, we create a grass cell -
  NEW VEGETATION
    2. If current cell is dirt and neighbors are equal to 3 OR 4 grass cells, we create a grass cell - WATER
  TRANSPORTATION VIA ROOTS
    3. If a dirt or grass cell is encompassed by more than 4 neighboring water cells, convert the cell into a
  water cell. - Over Irrigation
    4. If current cell is dirt OR grass and water neighbors are equal to 1, there
  will be a 1% percent chance that the given cell turns into water and a 99% chance that it will remain the same.
  - RANDOM EROSION (THE DEGRADATION OF THE CELL)
    5. If a current cell is water and its neighbors are less than 6 waters cell and a combination of a total of
  more than 6 dirt OR grass cells are around it, there will be a 5% chance that the cell turns into dirt and a 95%
  chance nothing occurs - Evaporation

  ENVIRONMENT:

    6. If current cell is dirt and there is 1 or 2 neighbor stone cells, there is a 3%
  chance that we will create a new stone, and a 97% chance that everything stays the same

    7. If a current cell is grass and its neighbors are more than 4 grass cells, there will be a 10%
  chance we generate a tree, a 10% chance we generate a bush, a 10% chance we generate a rose, lastly a 70% chance
  nothing occurs. - Plant Growth

  8. If a current cell is stone, and its neighbors are more than 6 stone cells, there will be a 20% chance we generate a
  crystal. - Crystal Growth

*/

enum State {
  DIRT,
  GRASS,
  WATER,
  STONE,
  PLANT_TREE,
  PLANT_BUSH,
  PLANT_FLOWER,
  CRYSTAL
};

class TerrainGenerator : public ECAutomata {
  public:
  
  TerrainGenerator();

  /*
    Sets custom grid draw method to call.
  */
  
  void setGridDraw(void (*draw)(Grid &grid, int rows, int cols));
  
  /*
    Retreives the internal modified grid.
  */
  
  Grid getGrid();
  
  /*
    Prepopulates the grid with random cells with fixed states.
  */

  void prepopulate();
  
  /*
    Sets a seed provided by the user.
  */

  void seed(int num);
  
  /*
    Sets a random seed and returns the value.
  */
    
  int seed();
  void execute() override;

  
  private:
    void (*draw)(Grid &grid, int rows, int cols);
};


#endif
