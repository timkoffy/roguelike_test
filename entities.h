#ifndef ROGUELIKE_TEST_ENTITIES_H
#define ROGUELIKE_TEST_ENTITIES_H
#include <array>

#include "entity.h"
#include "colors.hpp"

namespace Entities {
    class Orc : public Entity {
    public:
        Orc(int chunkX, int chunkY) : Entity(chunkX, chunkY, 'o', 0, Colors::ENTITY_ORC_COLOR) {
            isAnimated = true;
            isSolid = false;

            framesCount = 2;
            frames = {'o', 'O'};
            lastFrame = 0;
            lastAnimationTick = 0;
            lastMovingTick = 0;
        }

        // todo: создать вспомогательный неймспейс с хелпер функциями
        void update(int ticks) override {
            if (lastAnimationTick + 75 <= ticks) {
                lastAnimationTick = ticks;
                lastFrame = (lastFrame + 1) % framesCount;
                setChar(frames[lastFrame]);
            }
            srand(x * y + time(nullptr));
            if (lastMovingTick + (rand() % 200 + 200) <= ticks) {
                lastMovingTick = ticks;
                const auto newCoords = Game::getCoordinatesInDirection({x, y}, rand() % 4);
                if (!Game::isBlockSolid(newCoords, this)) {
                    x = newCoords.first;
                    y = newCoords.second;
                }
            }
        }

        void onPlayerInteraction(char ch) override {
            // orc have been pushed by player
            const auto newCoords = Game::getCoordinatesInDirection({x, y}, Game::direction);
            if (ch != '.' && ch != 'D') {
                return;
            }
            x = newCoords.first;
            y = newCoords.second;
        }

    private:
        int framesCount;
        std::array<char, 2> frames;
        int lastFrame;
        int lastAnimationTick;
        int lastMovingTick;
    };
}


#endif //ROGUELIKE_TEST_ENTITIES_H