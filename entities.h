#ifndef ROGUELIKE_TEST_ENTITIES_H
#define ROGUELIKE_TEST_ENTITIES_H
#include <array>

#include "entity.h"
#include "game.hpp"


namespace Entities {
    class Orc : public Entity {
    public:
        Orc(int x, int y) : Entity(x, y, 'o', 0, Game::Colors::ENTITY_ORC_COLOR) {
            isAnimated = true;
            isWalkable = false;

            framesCount = 2;
            frames = {'o', 'O'};
            lastFrame = 0;
            lastTick = 0;
        }

        void update() override {
            if (lastTick + 75 == Game::ticks) {
                lastTick = Game::ticks;
                lastFrame = (lastFrame + 1) % framesCount;
                setChar(frames[lastFrame]);
            }
        }

    private:
        int framesCount;
        std::array<char, 2> frames;
        int lastFrame;
        int lastTick;
    };
}


#endif //ROGUELIKE_TEST_ENTITIES_H