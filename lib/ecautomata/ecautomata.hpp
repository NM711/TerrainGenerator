#ifndef EC_AUTOMATA_HPP
#define EC_AUTOMATA_HPP

#include <map>
#include <random>
#include "./util/seeder.hpp"
#include "./util/grid.hpp"

struct NeighborSpecification {
    int start;
    int end;

    NeighborSpecification(int start = 0, int end = 0) {
      this->start = start;
      this->end = end;
    };
};

// Given a state maps to a number of neighboring cell states.

typedef std::map<unsigned int, unsigned int> NeighboringCellStateCount;

template <typename T = int>

struct BasePosition {
    T top;
    T topLeft;
    T topRight;
    T left;
    T right;
    T bottom;
    T bottomLeft;
    T bottomRight;
};

typedef BasePosition<int> NeighborhoodPositionOffset;

/*
  This object is used to indicate what a "neighborhood" is to be considered in your automaton.

  For example: A neighbor on the top is anything between the next cell and 5 cells upwards.
*/

typedef BasePosition<NeighborSpecification> NeighborhoodPosition;

/*
  Abstract struct that contains an overridable "apply" method for applying custom rulesets.

  @template @typedef T = Dynamic dependency that MAY be needed in order to smoothly implement the apply method.
*/

template <typename T>

class CustomGridRuleset {
  public:
    CustomGridRuleset<T>() {
      this->dependency = nullptr;
      this->distribution = nullptr;
    };

    /*
      Injects an address to a single dependency into the object.
    */

    void inject(T *depen) {
      this->dependency = depen;
    };

    /*
      It may be necessary to set a INT distribution when dealing with percent and chance.
      This I believe to be something common in the implementation of rules, so if we need to set a range for a given number
      we can simply pass a distribution into this setter.
    */

    void setDistribution(std::uniform_int_distribution<int> *distribution) {
      this->distribution = distribution;
    };

    /*
      Apply is a method used to "apply" different rules to the grid. The user is provided 3 parameters, which be filled
      when the "pass" method is called.

      @param current - The cell in the LAST updated grid.
      @param update - The cell that is meant to be updated within the NEW grid.
      @param stateCount - This is a map that uses the given provided states as keys, and the values are the computed
      neighbors that contain that state.
    */
    virtual void apply(int currentCell, int *cellToUpdate, NeighboringCellStateCount &stateCount) = 0;

  protected:
    T *dependency;
    std::uniform_int_distribution<int> *distribution;
};

/*
   ECAutomata is a extendable framework meant to be used by no one other than me (though others can lol). Its supposed to
   speed the development of two dimensional cellular automata by offering ways to initialize the grid, set different states,
   and create new functionality without all the annoying boilerplate.
*/

class ECAutomata {

  public:
    void virtual execute() = 0;
    void initGrid(int rowSize, int colSize, unsigned int popValue = 0);

  protected:
    Grid grid;
    GridBuilder builder;
    Seeder seeder;
    NeighboringCellStateCount neighborStateCountMap;

    void setState(unsigned int state);

    /*
      Sets the object that contains information on what a neighborhood should be considered.
    */

    void setNeighborhoodPosition(NeighborhoodPosition np);

    /*
      Method that performs a pass through the grid
    */

    template <typename T> void pass(CustomGridRuleset<T> &ruleset) {
      Grid freshGrid = this->builder.build();
      for (int row = 0; row < this->builder.getRows(); ++row) {
        for (int col = 0; col < this->builder.getCols(); ++col) {
          this->computeNeighbors({row, col}, this->neighborStateCountMap);
          int current = this->grid.at(row).at(col);
          int *update = &freshGrid.at(row).at(col);
          ruleset.apply(current, update, this->neighborStateCountMap);
          this->resetNeighborStateMap();
        };
      };

      this->grid = freshGrid;
    };

  private:
    NeighborhoodPosition neighborhoodPosition;
    void resetNeighborStateMap();
    /*
      Method that calculates the states of neighboring cells, from the provided row and col position in the grid.
      @param pos - Positional data of the current cell in the grid.
      @param neighborStateCount - Unordered map that contains a count of the nearby neighbor cell states.
    */

    void computeNeighbors(Position pos, NeighboringCellStateCount &neighborStateCount);
    int computePositionalOffset(NeighborSpecification &spec);
    NeighborhoodPositionOffset computeNeighborhoodOffsets();
};

#endif
