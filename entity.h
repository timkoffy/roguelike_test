#ifndef ROGUELIKE_TEST_ENTITY_H
#define ROGUELIKE_TEST_ENTITY_H

#include "cell.h"

class Entity : public Cell {
public:
    Entity(int x, int y, char ch, int color, int id)
        : Cell(x, y, ch, color), id(id), isAnimated(false), isSolid(false) {}

    virtual ~Entity() = default;

    int getId() const { return id; }
    bool getIsAnimated() const { return isAnimated; }
    bool getIsSolid() const { return isSolid; }

    virtual int update(int ticks)  {}
    virtual void onPlayerInteraction(char ch) {}

protected:
    int id;
    bool isAnimated;
    bool isSolid;
};


#endif //ROGUELIKE_TEST_ENTITY_H