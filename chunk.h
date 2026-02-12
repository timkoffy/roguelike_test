#ifndef ROGUELIKE_TEST_CHUNK_H
#define ROGUELIKE_TEST_CHUNK_H

#include "cell.h"
#include "entities.h"
#include "entity.h"

constexpr int CHUNK_SIZE = 32;

class Chunk final {
public:
    Chunk() = default;
    Chunk(const int x, const int y) : x(x), y(y) {}

    ~Chunk() = default;

    [[nodiscard]] int getX() const { return x; }
    [[nodiscard]] int getY() const { return y; }
    [[nodiscard]] int getOffset() const { return offset; }
    [[nodiscard]] Cell* getCell(const int cellX, const int cellY) {
        if (cellX < 0 || cellX >= CHUNK_SIZE || cellY < 0 || cellY >= CHUNK_SIZE) {
            return nullptr;
        }
        return &cells.at(cellY).at(cellX);
    }
    [[nodiscard]] std::array<std::array<Cell, CHUNK_SIZE>, CHUNK_SIZE> getAllCells() {
        return cells;
    }

    void setPosition(const int newX, const int newY) {
        x = newX; y = newY;
    }
    void setCell(const Cell& cell) {
        cells.at(cell.getY()).at(cell.getX()) = cell;
    }
    void setOffset(const int offsetInFile) {
        offset = offsetInFile;
    }

protected:
    int x = 0;
    int y = 0;
    int offset = -1;
    std::array<std::array<Cell, CHUNK_SIZE>, CHUNK_SIZE> cells;
};


#endif //ROGUELIKE_TEST_CHUNK_H