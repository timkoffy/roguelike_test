#include "game.hpp"
#include <chrono>
#include <iostream>
#include <thread>

#include "cell.h"
#include "entities.h"
#include "entity.h"

namespace Game {
    std::array<std::array<Cell, COLS>, ROWS> baseLayer;
    std::array<std::array<char, COLS>, ROWS> entityLayer {' '};
    std::array<std::array<int, COLS>, ROWS> colorLayer;
    std::array<std::array<bool, COLS>, ROWS> isItalicLayer {false};

    std::vector<std::unique_ptr<Entity>> entities;

    int playerX = 0;
    int playerY = 0;
    int direction = 0;
    int ticks = 0;
    int lastTick = 0;

    void runTime(int rows, int cols) {
        std::cout << "\033[2J\033[1;1H" << "press any key...";

        readFromFile();

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
                    case 'c': destroyBlockInFrontOfPlayer(); break;
                    case 'z': std::cout << "\033[2J\033[1;1H"; return;
                    default: break;
                }
            }

            ticks++;
            updateAnimations();

            // sync fps to 60
            const auto start = std::chrono::high_resolution_clock::now();
            drawField(rows, cols);
            const auto end = std::chrono::high_resolution_clock::now();
            const auto delta = end - start;

            std::this_thread::sleep_for(FRAME_TIME - delta);
        }
    }

    // data managing
    void readFromFile() {
        FILE* f = fopen("data.dat", "rb");

        // deserialize field data
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                char ch;
                fread(&ch, 1, 1, f);
                baseLayer.at(y).at(x) = Cell(x, y, ch);
            }
        }

        // deserialize player data
        fread(&playerX, sizeof(int), 1, f);
        fread(&playerY, sizeof(int), 1, f);
        fread(&direction, sizeof(int), 1, f);

        // deserialize entities
        int entityId, entityX, entityY;
        while (fread(&entityId, sizeof(int), 1, f) == 1) {
            if (entityId == -1) break;
            fread(&entityX, sizeof(int), 1, f);
            fread(&entityY, sizeof(int), 1, f);
            // entities.emplace_back();
        }

        fclose(f);
    }

    void saveToFile() {
        FILE* f = fopen("data.dat", "wb");

        // serialize field data
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                char ch = baseLayer.at(y).at(x).getChar();
                fwrite(&ch, 1, 1, f);
            }
        }

        // serialize player data
        fwrite(&playerX, sizeof(int), 1, f);
        fwrite(&playerY, sizeof(int), 1, f);
        fwrite(&direction, sizeof(int), 1, f);

        // serialize entities
        int entityId, entityX, entityY;

        // if (entities.empty()) {
        //     entityId = -1;
        //     fwrite(&entityId, sizeof(int), 1, f);
        //     fclose(f);
        //     return;
        // }
        //
        // for (const auto& entity : entities) {
        //     entityId = entity.getId();
        //     entityX = entity.getX();
        //     entityY = entity.getY();
        //
        //     fwrite(&entityId, sizeof(int), 1, f);
        //     fwrite(&entityX, sizeof(int), 1, f);
        //     fwrite(&entityY, sizeof(int), 1, f);
        // }

        entityId = -1;
        fwrite(&entityId, sizeof(int), 1, f);
        fclose(f);
    }

    void editByteInFile(int x, int y, char ch) {
        FILE* f = fopen("data.dat", "rb+");
        fseek(f, y * COLS + x, SEEK_SET);
        fwrite(&ch, 1, 1, f);
        baseLayer.at(y).at(x).setChar(ch);
        fclose(f);
    }

    // reset all data
    void createInitialField() {
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                baseLayer.at(y).at(x).setChar('.');
            }
        }

        baseLayer.at(8).at(17).setChar('$');
        baseLayer.at(3).at(5).setChar('$');

        playerX = 0;
        playerY = 0;
        direction = 0;
        saveToFile();
    }

    // help function
    std::pair<int, int> getCoordinatesInDirection(std::pair<int, int> point, int dir) {
        int x = point.first,
            y = point.second;

        switch (dir) {
            case 0: y--; break;
            case 1: x--; break;
            case 2: y++; break;
            case 3: x++; break;
            default: break;
        }

        if (x < 0 || x >= COLS || y < 0 || y >= ROWS)
            return {point.first, point.second};
        return {x, y};
    }

    void move(int dir) {
        direction = dir;

        auto newCoords = getCoordinatesInDirection({playerX, playerY}, dir);
        int x = newCoords.first, y = newCoords.second;

        bool canMove = true;
        for (const auto& entity : entities) {
            if (x == entity->getX() && y == entity->getY()) {
                if (entity->getIsSolid())
                    canMove = false;
                entity->onPlayerInteraction();
                lightShader();
                break;
            }
        }

        char ch = baseLayer.at(y).at(x).getChar();
        if (ch != '.' && ch != 'D') {
            return;
        }

        if (canMove) {
            playerX = x;
            playerY = y;
        }

        saveToFile(); // soon: save only on exit
    }

    void rotateDirection(bool isClockwise) {
        if (isClockwise) {
            direction = (4 + direction - 1) % 4;
            return;
        } direction = (direction + 1) % 4;
    }


    void placeBlockInFrontOfPlayer(char block) {
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
        int x = newCoords.first, y = newCoords.second;

        if (baseLayer.at(y).at(x).getChar() != '.')
            return;

        editByteInFile(x, y, block);
        colorLayer.at(y).at(x) = Colors::WALL_COLOR;
        lightShader();
    }

    void destroyBlockInFrontOfPlayer() {
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
        int x = newCoords.first, y = newCoords.second;

        editByteInFile(x, y, '.');
        colorLayer.at(y).at(x) = Colors::FIELD_COLOR;
        lightShader();
    }
}
