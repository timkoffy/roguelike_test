#include "game.hpp"
#include <chrono>
#include <iostream>
#include <cmath>
#include <thread>
#include <unordered_map>

#include "cell.h"
#include "entities.h"
#include "entity.h"
#include "chunk.h"
#include "colors.hpp"

namespace Game {
    void runTime(const int rows, const int cols) {
        std::cout << "\033[2J\033[1;1H" << "press any key...";

        // readFromFile();

        entities.push_back(std::make_unique<Entities::Orc>(2, 6));
        entities.push_back(std::make_unique<Entities::Orc>(5, 8));

        lightShader();

        // game loop
        while (true) {
            int key = getchar();
            if (key != EOF) {
                switch (key) {
                    case 'w': case 'k': move(UP); break;
                    case 'a': case 'h': move(LEFT); break;
                    case 's': case 'j': move(DOWN); break;
                    case 'd': case 'l': move(RIGHT); break;
                    case 'q': rotateDirection(false); break;
                    case 'e': rotateDirection(true); break;
                    case 'f': placeBlockInFrontOfPlayer('@'); break;
                    case 'r': placeBlockInFrontOfPlayer('$'); break;
                    case 't': placeBlockInFrontOfPlayer('D'); break;
                    case 'v': spawnEntity(playerX, playerY); break;
                    case 'c': destroyBlockInFrontOfPlayer(); break;
                    case 'z': saveToFile(); std::cout << "\033[2J\033[1;1Hsuccessfully exit!\n"; return;
                    default: break;
                }
            }

            ticks++;
            // updateAnimations();

            // sync fps to 60
            const auto start = std::chrono::high_resolution_clock::now();
            drawField(rows, cols);
            const auto end = std::chrono::high_resolution_clock::now();
            const auto delta = end - start;
            // std::cout << delta << ' ' << chunkCount;
            // std::cout.flush();

            std::this_thread::sleep_for(FRAME_TIME - delta);
        }
    }

    void move(const int dir) {
        direction = dir;
        const std::pair playerPos{playerX, playerY};
        const auto newCoords = getCoordinatesInDirection(playerPos, direction);

        std::pair chunkCoords = getChunkCoords(newCoords);
        if (!buf.contains(chunkCoords)) {
            createChunk(chunkCoords.first, chunkCoords.second);
        }

        if (!isBlockSolidPlayer(newCoords)) {
            playerX = newCoords.first;
            playerY = newCoords.second;
        }
    }

    void rotateDirection(const bool isClockwise) {
        if (isClockwise) {
            direction = (4 + direction - 1) % 4;
            return;
        } direction = (direction + 1) % 4;
    }

    void placeBlockInFrontOfPlayer(const char ch) {
        const std::pair playerPos{playerX, playerY};
        const auto [x, y] = getCoordinatesInDirection(playerPos, direction);

        const std::pair newCoords{x, y};
        const auto [chunkX, chunkY] = getChunkCoords(newCoords);

        if (!buf.contains({chunkX, chunkY}))
            return;

        const int cellLocalX = x - chunkX * CHUNK_SIZE;
        const int cellLocalY = y - chunkY * CHUNK_SIZE;
        Cell* cell = buf.at({chunkX, chunkY}).getCell(cellLocalX, cellLocalY);

        if (cell->getChar() != '.')
            return;

        cell->setChar(ch);
        cell->setColor(Colors::WALL_COLOR);
        lightShader();
    }

    void destroyBlockInFrontOfPlayer() {
        const std::pair playerPos{playerX, playerY};
        const auto [x, y] = getCoordinatesInDirection(playerPos, direction);

        const std::pair newCoords{x, y};
        const auto [chunkX, chunkY] = getChunkCoords(newCoords);

        if (!buf.contains({chunkX, chunkY}))
            return;

        Cell* cell = buf.at({chunkX, chunkY}).getCell(x - (chunkX * CHUNK_SIZE), y - (chunkY * CHUNK_SIZE));

        cell->setChar('.');
        cell->setColor(Colors::FIELD_COLOR);
        lightShader();
    }

    void spawnEntity(int x, int y) {

    }

    // helper functions
    std::pair<int, int> getCoordinatesInDirection(const std::pair<int, int>& point, const int dir) {
        auto [x, y] = point;

        switch (dir) {
            case 0: y++; break;
            case 1: x--; break;
            case 2: y--; break;
            case 3: x++; break;
            default: break;
        }

        return {x, y};
    }

    char getCharOnPoint(const std::pair<int, int>& point) {
        const auto [x, y] = point;
        const auto [chunkX, chunkY] = getChunkCoords(point);

        if (buf.contains({chunkX, chunkY})) {
            const int cellLocalX = x - chunkX * CHUNK_SIZE;
            const int cellLocalY = y - chunkY * CHUNK_SIZE;
            Cell* cell = buf.at({chunkX, chunkY}).getCell(cellLocalX, cellLocalY);
            if (cell) {
                return cell->getChar();
            }
        } return ' ';
    }

    bool isBlockSolid(const std::pair<int, int>& point, Entity* thisEntity) {
        const auto chunkCoords = getChunkCoords(point);
        if (!buf.contains(chunkCoords)) {
            return true;
        }

        const char ch = getCharOnPoint(point);
        if (ch != '.' || (playerX == point.first && playerY == point.second))
            return true;

        // for (const auto& entity : entities) {
        //     if (entity.get() == thisEntity) continue;
        //
        //     if (point.first == entity->getX() && point.second == entity->getY())
        //         return true;
        // }
        return false;
    }

    bool isBlockSolidPlayer(const std::pair<int, int>& point) {
        const auto chunkCoords = getChunkCoords(point);
        if (!buf.contains(chunkCoords)) {
            return false;
        }

        const char ch = getCharOnPoint(point);
        if (ch != '.' && ch != 'D') {
            return true;
        }

        // if (auto entity = buf[chunkCoords].getEntityOnPoint(point); entity != nullptr) {
        //     entity->onPlayerInteraction(ch);
        //     if (entity->getIsSolid())
        //         return true;
        // }
        return false;
    }
}
