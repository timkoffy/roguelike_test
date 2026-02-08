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

namespace Game {
    std::array<std::array<Cell, COLS>, ROWS> baseLayer;
    std::array<std::array<char, COLS>, ROWS> entityLayer {' '};
    std::array<std::array<int, COLS>, ROWS> colorLayer;
    std::array<std::array<bool, COLS>, ROWS> isItalicLayer {false};

    struct PairHash {
        size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    std::unordered_map<std::pair<int, int>, Chunk, PairHash> buf;
    std::vector<std::unique_ptr<Entity>> entities;

    int playerX = 0;
    int playerY = 0;
    int direction = 0;
    int ticks = 0;
    int lastTick = 0;

    std::vector<int> chunkIndexes;
    std::vector<Cell> chunkData;
    int chunkCount = 0;

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
                    case 'v': entities.push_back(std::make_unique<Entities::Orc>(playerX, playerY));
                    case 'c': destroyBlockInFrontOfPlayer(); break;
                    case 'z': saveToFile(); std::cout << "\033[2J\033[1;1Hsuccessfully exit!\n"; return;
                    default: break;
                }
            }

            ticks++;
            // updateAnimations();
            lightShader();

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

        // deserialize chunk data
        // fread(&chunkCount, sizeof(int), 1, f);
        // fread(chunkIndexes.data(), sizeof(int), chunkCount, f);

        // for (int i = 0; i < chunkCount; i++) {
        // }

        // deserialize field data
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                char ch;
                fread(&ch, 1, 1, f);
                baseLayer.at(y).at(x) = Cell(x, y, ch, 0);
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
        baseLayer.at(y ).at(x).setChar(ch);
        fclose(f);
    }

    // reset all data
    void createInitialField() {
        // int seed = 1003940421;
        // srand(seed);
        //
        // // for (int y = 0; y < ROWS; y++) {
        // //     for (int x = 0; x < COLS; x++) {
        // //         if (((y / 10) % 10) % 2 == 0 ^ ((x / 10) % 10) % 2 == 0) {
        // //             baseLayer.at(y).at(x).setChar('.');
        // //             continue;
        // //         } baseLayer.at(y).at(x).setChar('@');
        // //     }
        // // }
        //
        // // for (int y = 1; y < ROWS; y++) {
        // //     for (int x = 1; x < COLS; x++) {
        // //         if (seed ) {
        // //             baseLayer.at(y).at(x).setChar('@');
        // //             continue;
        // //         } baseLayer.at(y).at(x).setChar('.');
        // //     }
        // // }
        //
        // for (int y = 0; y < ROWS; y++) {
        //     for (int x = 0; x < COLS; x++) {
        //         if (rand() % 100 == rand() % 100) {
        //             baseLayer.at(y).at(x).setChar('/');
        //             continue;
        //         } baseLayer.at(y).at(x).setChar('@');
        //     }
        // }
        //
        // for (int y = 0; y < ROWS; y++) {
        //     for (int x = 0; x < COLS; x++) {
        //         if (baseLayer.at(y).at(x).getChar() == '/') {
        //             int offset = rand() % 10 + 3;
        //             for (int xSub = x - offset; xSub < x + offset; xSub++) {
        //                 if (xSub < 0 || xSub >= COLS)
        //                     continue;
        //
        //                 int ySub = sqrt((offset - 1) * (offset - 1) - (xSub - x) * (xSub - x)) + y;
        //
        //                 if (ySub < 0 || ySub >= ROWS)
        //                     continue;
        //
        //                 baseLayer.at(ySub).at(xSub).setChar('.');
        //                 for (int yFill = ySub - 1; yFill > y; yFill--) {
        //                     baseLayer.at(yFill).at(xSub).setChar('.');
        //                 }
        //
        //                 ySub = -sqrt((offset - 1) * (offset - 1) - (xSub - x) * (xSub - x)) + y;
        //
        //                 if (ySub < 0 || ySub >= ROWS)
        //                     continue;
        //
        //                 baseLayer.at(ySub).at(xSub).setChar('.');
        //                 for (int yFill = ySub; yFill <= y; yFill++) {
        //                     baseLayer.at(yFill).at(xSub).setChar('.');
        //                 }
        //             }
        //         }
        //     }
        // }
        //
        // // for (int y = 1; y < ROWS - 1; y++) {
        // //     for (int x = 1; x < COLS - 1; x++) {
        // //         if ((baseLayer.at(y).at(x).getChar() != baseLayer.at(y-1).at(x).getChar() &&
        // //             baseLayer.at(y).at(x).getChar() != baseLayer.at(y+1).at(x).getChar()) ||
        // //             (baseLayer.at(y).at(x).getChar() != baseLayer.at(y).at(x-1).getChar() &&
        // //             baseLayer.at(y).at(x).getChar() != baseLayer.at(y).at(x+1).getChar())) {
        // //             if (baseLayer.at(y).at(x).getChar() == '.')
        // //                 baseLayer.at(y).at(x).setChar('@');
        // //             else baseLayer.at(y).at(x).setChar('.');
        // //             x++;
        // //         }
        // //     }
        // // }
        //
        // baseLayer.at(8).at(2).setChar('$');
        // baseLayer.at(3).at(5).setChar('$');
        //
        // // for (int y = 0; y < ROWS; y++) {
        // //     for (int x = 0; x < COLS; x++) {
        // //         std::cout << baseLayer.at(y).at(x).getChar() << ' ';
        // //     } std::cout << '\n';
        // // }

        createChunk(0, 0);
        createChunk(1, 0);

        // Cell c(0, 3, '$');
        // buf[{0, 0}].setCell(c);
        Cell c1(15, 2, '$');
        buf[{0, 0}].setCell(c1);

        playerX = 0;
        playerY = 0;
        direction = 0;
        // saveToFile();
    }

    void createChunk(int chunkX, int chunkY) {
        Chunk chunk;
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                Cell cell;
                cell.setChar('.');
                cell.setChunkPosition(x, y);
                chunk.setCell(cell);
            }
        }
        chunk.setPosition(chunkX, chunkY);
        buf[{chunkX, chunkY}] = chunk;
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

    bool isBlockSolid(std::pair<int, int> point, Entity* thisEntity) {
        char ch = baseLayer.at(point.second).at(point.first).getChar();
        if (ch != '.' || (playerX == point.first && playerY == point.second))
            return true;

        for (const auto& entity : entities) {
            if (entity.get() == thisEntity) continue;

            if (point.first == entity->getChunkX() && point.second == entity->getChunkY())
                return true;
        } return false;
    }

    bool isBlockSolidPlayer(std::pair<int, int> point) {
        // char ch = baseLayer.at(point.second).at(point.first).getChar();
        // if (ch != '.' && ch != 'D')
        //     return true;
        //
        // for (const auto& entity : entities) {
        //     if (point.first == entity->getX() && point.second == entity->getY()) {
        //         entity->onPlayerInteraction();
        //         lightShader();
        //         if (entity->getIsSolid())
        //             return true;
        //     }
        // } return false;
        return false;
    }
}
