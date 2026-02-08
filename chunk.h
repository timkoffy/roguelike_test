#ifndef ROGUELIKE_TEST_CHUNK_H
#define ROGUELIKE_TEST_CHUNK_H
#include <iostream>
#include "cell.h"

constexpr int CHUNK_SIZE = 16;

class Chunk {
public:
    Chunk() : m_x(0), m_y(0) {}

    Chunk(const int x, const int y) : m_x(x), m_y(y) {}

    virtual ~Chunk() = default;

    int getX() const { return m_x; }
    int getY() const { return m_y; }
    Cell getCell(const int x, const int y) { return cells.at(y).at(x); }
    int getChunkSize() const { return CHUNK_SIZE; }

    void setPosition(int newX, int newY) {
        m_x = newX;
        m_y = newY;
    }
    void setCell(const int x, const int y, const Cell& cell) {
        cells.at(y).at(x) = cell;
    }

protected:
    int m_x;
    int m_y;
    std::array<std::array<Cell, CHUNK_SIZE>, CHUNK_SIZE> cells;
};


#endif //ROGUELIKE_TEST_CHUNK_H