#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../core/generator.hpp"

// emscripten::val is Emscripten's type for JavaScript values
// It allows C++ to interact with JavaScript objects, arrays, functions, etc.

class TerrainGeneratorWrapper {
private:
    TerrainGenerator generator;

public:
    TerrainGeneratorWrapper() = default;

    // Helper to convert Grid to 2D array
    emscripten::val getGrid2DFromGrid(Grid &grid) const {
        const int rows = grid.getRows();
        const int cols = grid.getCols();
        
        emscripten::val result = emscripten::val::array();
        for (int r = 0; r < rows; ++r) {
            emscripten::val row = emscripten::val::array();
            for (int c = 0; c < cols; ++c) {
                row.call<void>("push", grid.getContent()->at(r).at(c));
            }
            result.call<void>("push", row);
        }
        return result;
    }

    // Get grid dimensions
    int getRows() {
        return generator.getGrid().getRows();
    }

    int getCols() {
        return generator.getGrid().getCols();
    }

    // Get cell state at position
    int getCellState(int row, int col) {
        Grid grid = generator.getGrid();
        if (row >= 0 && row < grid.getRows() && col >= 0 && col < grid.getCols()) {
            return grid.getContent()->at(row).at(col);
        }
        return 0; // Return default state if out of bounds
    }

    // Get entire grid as a flat array for JavaScript
    // Returns JavaScript Array: [cell0, cell1, cell2, ...]
    emscripten::val getGridData() {
        Grid grid = generator.getGrid();
        const int rows = grid.getRows();
        const int cols = grid.getCols();
        
        emscripten::val array = emscripten::val::array();
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                array.call<void>("push", grid.getContent()->at(r).at(c));
            }
        }
        return array;
    }

    // Get grid as 2D array structure
    // Returns JavaScript Array: [[row0], [row1], ...]
    emscripten::val getGrid2D() {
        Grid grid = generator.getGrid();
        return getGrid2DFromGrid(grid);
    }

    // Seed with specific number
    void setSeed(int num) {
        generator.seed(num);
    }

    // Seed randomly and return the seed used
    int setRandomSeed() {
        return generator.seed();
    }

    // Initialize the grid with dimensions and initial state
    void initGrid(int rows, int cols, int initialState) {
        generator.initGrid(rows, cols, initialState);
    }

    // Execute terrain generation
    void execute() {
        generator.execute();
    }

    // Execute with a custom draw function that does nothing
    // (since we can't pass lambdas with captures as function pointers)
    void executeWithDraw() {
        // Simple no-op draw function
        generator.setGridDraw([](Grid &grid, int rows, int cols) {
            // This could log or do simple operations that don't require state
        });
        generator.execute();
    }
};

// Expose State enum values as constants
EMSCRIPTEN_BINDINGS(States) {
    emscripten::constant("STATE_DIRT", static_cast<int>(State::DIRT));
    emscripten::constant("STATE_WATER", static_cast<int>(State::WATER));
    emscripten::constant("STATE_GRASS", static_cast<int>(State::GRASS));
    emscripten::constant("STATE_STONE", static_cast<int>(State::STONE));
    emscripten::constant("STATE_CRYSTAL", static_cast<int>(State::CRYSTAL));
    emscripten::constant("STATE_PLANT_TREE", static_cast<int>(State::PLANT_TREE));
    emscripten::constant("STATE_PLANT_BUSH", static_cast<int>(State::PLANT_BUSH));
    emscripten::constant("STATE_PLANT_FLOWER", static_cast<int>(State::PLANT_FLOWER));
}

// Expose the TerrainGenerator wrapper to JavaScript
EMSCRIPTEN_BINDINGS(TerrainGenerator) {
    emscripten::class_<TerrainGeneratorWrapper>("TerrainGenerator")
        .constructor<>()
        .function("initGrid", &TerrainGeneratorWrapper::initGrid)
        .function("getRows", &TerrainGeneratorWrapper::getRows)
        .function("getCols", &TerrainGeneratorWrapper::getCols)
        .function("getCellState", &TerrainGeneratorWrapper::getCellState)
        .function("getGridData", &TerrainGeneratorWrapper::getGridData)
        .function("getGrid2D", &TerrainGeneratorWrapper::getGrid2D)
        .function("setSeed", &TerrainGeneratorWrapper::setSeed)
        .function("setRandomSeed", &TerrainGeneratorWrapper::setRandomSeed)
        .function("execute", &TerrainGeneratorWrapper::execute)
        .function("executeWithDraw", &TerrainGeneratorWrapper::executeWithDraw);
}

