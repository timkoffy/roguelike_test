#ifndef ROGUELIKE_TEST_ENTITY_H
#define ROGUELIKE_TEST_ENTITY_H
#include "cell.h"


class Entity : public Cell {
public:
    Entity(int x, int y, char ch, int id, int color)
        : Cell(x, y, ch), id(id), color(color), isAnimated(false), isSolid(false) {
    }

    virtual ~Entity() = default;

    int getColor() const { return color; }
    int getId() const { return id; }
    bool getIsAnimated() const { return isAnimated; }
    bool getIsSolid() const { return isSolid; }

    virtual void update() {}
    virtual void onPlayerInteraction() {}

protected:
    int color;
    int id;
    bool isAnimated;
    bool isSolid;
};


#endif //ROGUELIKE_TEST_ENTITY_H