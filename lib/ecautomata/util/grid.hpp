#ifndef ECAUTOMATA_UTIL_GRID_HPP
#define ECAUTOMATA_UTIL_GRID_HPP

#include <vector>

/*
  Struct that contains information about a position within the grid.
*/

struct Position {
    int row;
    int col;

    Position(int row, int col) {
      this->row = row;
      this->col = col;
    };
};

typedef std::vector<std::vector<int>> Grid;

class GridBuilder {
  private:
    Grid grid;
    int rows;
    int cols;
    unsigned int value;

  public:
    /*
      Sets the rows and columns in our grid
    */

    void setSize(int rows, int cols);

    /*
      Initial element used to populate the grid columns.
    */

    void setPopValue(unsigned int value);

    int getRows();
    int getCols();

    /*
      Builds and returns grid
    */

    Grid build();
};

#endif
