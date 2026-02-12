#ifndef ROGUELIKE_TEST_CHUNK_H
#define ROGUELIKE_TEST_CHUNK_H

#include "cell.h"
#include "entities.h"
#include "entity.h"

constexpr int CHUNK_SIZE = 4;

class Chunk {
public:
    Chunk() : x(0), y(0) {}
    Chunk(const int x, const int y) : x(x), y(y) {}

    virtual ~Chunk() = default;

    int getX() const { return x; }
    int getY() const { return y; }
    Cell* getCell(int cellX, int cellY) {
        if (cellX < 0 || cellX >= CHUNK_SIZE || cellY < 0 || cellY >= CHUNK_SIZE) {
            return nullptr;
        }
        return &cells.at(cellY).at(cellX);
    }
    // Entity* getEntityOnPoint(const std::pair<int, int>& point) const {
    //     for (const auto& entity : entities) {
    //         if (entity->getX() == point.first && entity->getY() == point.second) {
    //             return entity.get();
    //         }
    //     } return nullptr;
    // }

    void setPosition(int newX, int newY) {
        x = newX; y = newY;
    }
    void setCell(const Cell& cell) {
        cells.at(cell.getY()).at(cell.getX()) = cell;
    }

    void addEntity(Entity entity);

protected:
    int x;
    int y;
    std::array<std::array<Cell, CHUNK_SIZE>, CHUNK_SIZE> cells;
    std::vector<Entity> entities;
};


#endif //ROGUELIKE_TEST_CHUNK_H