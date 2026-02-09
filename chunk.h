#ifndef ROGUELIKE_TEST_CHUNK_H
#define ROGUELIKE_TEST_CHUNK_H

#include "cell.h"
#include "entities.h"
#include "entity.h"

constexpr int CHUNK_SIZE = 32;

class Chunk {
public:
    Chunk() : x(0), y(0) {}
    Chunk(const int x, const int y) : x(x), y(y) {}

    virtual ~Chunk() = default;

    int getX() const { return x; }
    int getY() const { return y; }
    Cell* getCell(const int cellX, const int cellY) { return &cells.at(cellY).at(cellX); }

    void setPosition(int newX, int newY) {
        x = newX; y = newY;
    }
    void setCell(const Cell& cell) {
        cells.at(cell.getY()).at(cell.getX()) = cell;
    }
    void addEntity(const Entity& entity) {
        entities.emplace_back(entity);
    }

    void addEntity(Entity entity);

protected:
    int x;
    int y;
    std::array<std::array<Cell, CHUNK_SIZE>, CHUNK_SIZE> cells;
    std::vector<Entity> entities;
};


#endif //ROGUELIKE_TEST_CHUNK_H