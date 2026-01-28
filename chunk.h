#ifndef ROGUELIKE_TEST_CHUNK_H
#define ROGUELIKE_TEST_CHUNK_H
#include <iostream>
#include "cell.h"

constexpr int SIZE_CHUNK = 16;

class Chunk {
public:
    Chunk() : x(0), y(0) {}

    Chunk(int x, int y) : x(x), y(y) {}

    virtual ~Chunk() {}

    int getX() const { return x; }
    int getY() const { return y; }
    void setCell(Cell cell) const { cells.insert(cells.end(), cell); }

protected:
    int x;
    int y;
    std::array<std::array<Cell, SIZE_CHUNK>, SIZE_CHUNK> cells;
};


#endif //ROGUELIKE_TEST_CHUNK_H