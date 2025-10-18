#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../core/generator.hpp"
#include <vector>
using namespace emscripten;
class TerrainGeneratorWrapper {
private:
    TerrainGenerator generator;
    std::vector<std::vector<std::vector<int>>> capturedFrames;

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

public:
    TerrainGeneratorWrapper() {}

    void initGrid(int rows, int cols, int initialState) {
        generator.initGrid(rows, cols, initialState);
    }

    int setRandomSeed() {
        return generator.seed();
    }

    void executeAndCapture() {
        capturedFrames.clear();
        
        generator.setGridDraw([this](Grid &grid, int rows, int cols) {
            // Deep copy the grid for this frame
            std::vector<std::vector<int>> frameCopy(rows);
            for (int r = 0; r < rows; ++r) {
                frameCopy[r] = grid.getContent()->at(r);
            }
            capturedFrames.push_back(frameCopy);
        });
        
        generator.execute();
    }

    emscripten::val getGridPlayback() {
        emscripten::val frames = emscripten::val::array();
        
        for (const auto& frame : capturedFrames) {
            emscripten::val frameArray = emscripten::val::array();
            for (const auto& row : frame) {
                emscripten::val rowArray = emscripten::val::array();
                for (int cell : row) {
                    rowArray.call<void>("push", cell);
                }
                frameArray.call<void>("push", rowArray);
            }
            frames.call<void>("push", frameArray);
        }
        
        return frames;
    }

    int getRows() {
        return generator.getGrid().getRows();
    }

    int getCols() {
        return generator.getGrid().getCols();
    }
};


// Expose State enum values as constants
EMSCRIPTEN_BINDINGS(States) {
    constant("STATE_DIRT", static_cast<int>(State::DIRT));
    constant("STATE_WATER", static_cast<int>(State::WATER));
    constant("STATE_GRASS", static_cast<int>(State::GRASS));
    constant("STATE_STONE", static_cast<int>(State::STONE));
    constant("STATE_CRYSTAL", static_cast<int>(State::CRYSTAL));
    constant("STATE_PLANT_TREE", static_cast<int>(State::PLANT_TREE));
    constant("STATE_PLANT_BUSH", static_cast<int>(State::PLANT_BUSH));
    constant("STATE_PLANT_FLOWER", static_cast<int>(State::PLANT_FLOWER));
}

// Expose the TerrainGenerator wrapper to JavaScript
EMSCRIPTEN_BINDINGS(TerrainGenerator) {
    class_<TerrainGeneratorWrapper>("TerrainGenerator")
        .constructor<>()
        .function("initGrid", &TerrainGeneratorWrapper::initGrid)
        .function("setRandomSeed", &TerrainGeneratorWrapper::setRandomSeed)
        .function("executeAndCapture", &TerrainGeneratorWrapper::executeAndCapture)
        .function("getGridPlayback", &TerrainGeneratorWrapper::getGridPlayback)
        .function("getRows", &TerrainGeneratorWrapper::getRows)
        .function("getCols", &TerrainGeneratorWrapper::getCols);
}
