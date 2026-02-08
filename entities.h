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
            isSolid = false;

            framesCount = 2;
            frames = {'o', 'O'};
            lastFrame = 0;
            lastAnimationTick = 0;
            lastMovingTick = 0;
        }

        void update() override {
            if (lastAnimationTick + 75 <= Game::ticks) {
                lastAnimationTick = Game::ticks;
                lastFrame = (lastFrame + 1) % framesCount;
                setChar(frames[lastFrame]);
            }

                srand(chunkX*chunkY + time(nullptr));
                if (lastMovingTick + (rand() % 200 + 200) <= Game::ticks) {
                    lastMovingTick = Game::ticks;
                    const auto newCoords = Game::getCoordinatesInDirection({chunkX, chunkY}, rand() % 4);
                    if (!Game::isBlockSolid(newCoords, this)) {
                        chunkX = newCoords.first;
                        chunkY = newCoords.second;
                    }
                }
        }

        void onPlayerInteraction() override {
            // orc have been pushed by player
            const auto newCoords = Game::getCoordinatesInDirection({chunkX, chunkY}, Game::direction);
            char ch = Game::baseLayer.at(newCoords.second).at(newCoords.first).getChar();
            if (ch != '.' && ch != 'D') {
                return;
            }
            chunkX = newCoords.first;
            chunkY = newCoords.second;
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