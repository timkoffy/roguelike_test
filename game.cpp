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
    std::array<std::array<char, COLS>, ROWS> entityLayer {' '};
    std::vector<std::unique_ptr<Entity>> entities;

    std::unordered_map<std::pair<int, int>, Chunk, PairHash> buf;
    std::vector<Chunk> chunksEdited;
    int chunkCount = 0;

    int playerX = 0;
    int playerY = 0;
    int direction = 0;
    int ticks = 0;
    int lastTick = 0;

    void runTime(int rows, int cols) {
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

            std::this_thread::sleep_for(FRAME_TIME - delta);
        }
    }

    void move(int dir) {
        direction = dir;
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, dir);

        if (!isBlockSolidPlayer(newCoords)) {
            playerX = newCoords.first;
            playerY = newCoords.second;
        }
    }

    void rotateDirection(bool isClockwise) {
        if (isClockwise) {
            direction = (4 + direction - 1) % 4;
            return;
        } direction = (direction + 1) % 4;
    }

    void spawnEntity(int x, int y) {

    }

    void placeBlockInFrontOfPlayer(char ch) {
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
        int x = newCoords.first, y = newCoords.second;
        auto chunkCoords = getChunkCoords(newCoords);
        Cell* c = buf[{chunkCoords.first, chunkCoords.second}].getCell(x % CHUNK_SIZE, y % CHUNK_SIZE);

        if (c->getChar() != '.')
            return;

        c->setChar(ch);
        c->setColor(Colors::WALL_COLOR);
        lightShader();
    }

    void destroyBlockInFrontOfPlayer() {
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
        int x = newCoords.first, y = newCoords.second;
        auto chunkCoords = getChunkCoords(newCoords);
        Cell* c = buf[{chunkCoords.first, chunkCoords.second}].getCell(x % CHUNK_SIZE, y % CHUNK_SIZE);

        c->setChar('.');
        c->setColor(Colors::FIELD_COLOR);
        lightShader();
    }

    // helper functions
    std::pair<int, int> getCoordinatesInDirection(std::pair<int, int> point, int dir) {
        int x = point.first,
            y = point.second;

        switch (dir) {
            case 0: y++; break;
            case 1: x--; break;
            case 2: y--; break;
            case 3: x++; break;
            default: break;
        }

        return {x, y};
    }

    std::pair<int, int> getChunkCoords(std::pair<int, int> point) {
        int x = point.first;
        int y = point.second;

        int chunkX = x / CHUNK_SIZE;
        if (x < 0) chunkX--;

        int chunkY = y / CHUNK_SIZE;
        if (y < 0) chunkY--;

        return {chunkX, chunkY};
    }

    char getCharOnPoint(const std::pair<int, int>& point) {
        int x = point.first;
        int y = point.second;

        auto chunkCoords = getChunkCoords(point);
        int chunkX = chunkCoords.first;
        int chunkY = chunkCoords.second;

        char ch;
        if (buf.find({chunkX, chunkY}) != buf.end()) {
            ch = buf[{chunkX, chunkY}].getCell(x % CHUNK_SIZE, y % CHUNK_SIZE)->getChar();
        } else ch = ' ';

        return ch;
    }

    bool isBlockSolid(std::pair<int, int> point, Entity* thisEntity) {
        char ch = getCharOnPoint(point);

        if (ch != '.' || (playerX == point.first && playerY == point.second))
            return true;

        for (const auto& entity : entities) {
            if (entity.get() == thisEntity) continue;

            if (point.first == entity->getX() && point.second == entity->getY())
                return true;
        } return false;
    }

    bool isBlockSolidPlayer(std::pair<int, int> point) {
        auto coords = getCoordinatesInDirection(point, direction);
        char ch = getCharOnPoint(coords);

        if (ch != '.' && ch != 'D' && ch != ' ')
            return true;

        for (const auto& entity : entities) {
            if (point.first == entity->getX() && point.second == entity->getY()) {
                entity->onPlayerInteraction(ch);
                if (entity->getIsSolid())
                    return true;
            }
        } return false;
    }
}
