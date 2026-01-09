#ifndef ROGUELIKE_TEST_ENTITY_H
#define ROGUELIKE_TEST_ENTITY_H
#include "cell.h"


class Entity : public Cell {
public:
    Entity(int x, int y, char ch, int id)
        : Cell(x, y, ch), id(id) {
    }

    int getId() const { return id; }

protected:
    int id;
};


#endif //ROGUELIKE_TEST_ENTITY_H