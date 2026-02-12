#ifndef ROGUELIKE_TEST_ENTITY_H
#define ROGUELIKE_TEST_ENTITY_H

#include "cell.h"

class Entity : public Cell {
public:
    Entity(const int x, const int y, const char ch, const int color, const int id)
        : Cell(x, y, ch, color), id(id), isAnimated(false), isSolid(false) {}

    ~Entity() override = default;

    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] bool getIsAnimated() const { return isAnimated; }
    [[nodiscard]] bool getIsSolid() const { return isSolid; }

    virtual void update(int ticks)  {}
    virtual void onPlayerInteraction(char ch) {}

protected:
    int id;
    bool isAnimated;
    bool isSolid;
};


#endif //ROGUELIKE_TEST_ENTITY_H