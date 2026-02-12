#include "game.hpp"
#include <iostream>
#include <ranges>

#include "cell.h"
#include "chunk.h"

namespace Game {
    // draw viewport field (segment from buffer)
    void drawField(int rows, int cols) {
        std::cout << "\033[2J\033[1;1H\033[?25l";
        std::cout << "WASD / HJKL - move, QE - rotate, F - block, R - light, C - destroy, Z - exit\n"
                     "ticks: " << ticks << " x: " << playerX << " y: " << playerY;
        {
            // top side padding
            int y = (rows - VIEWPORT_ROWS) / 2 - 1;
            while (y > 0) {
                std::cout << '\n';
                y--;
            }
        }

        int leftViewportX = playerX - VIEWPORT_COLS / 2;
        int topViewportY = -playerY - VIEWPORT_ROWS / 2;

        int rightViewportX = leftViewportX + VIEWPORT_COLS;
        int bottomViewportY = topViewportY + VIEWPORT_ROWS;

        for (int negY = topViewportY; negY < bottomViewportY; negY++) {
            int y = -negY;
            // left side padding
            for (int x = 0; x < cols / 2 - VIEWPORT_COLS; x++)
                std::cout << ' ';
            // paste buffer
            for (int x = leftViewportX; x < rightViewportX; x++) {
                std::pair coords{x, y};
                const auto [chunkX, chunkY] = getChunkCoords(coords);
                char displayedChar;
                int color;

                if (buf.contains({chunkX, chunkY})) {
                    const int cellLocalX = x - (chunkX * CHUNK_SIZE);
                    const int cellLocalY = y - (chunkY * CHUNK_SIZE);
                    const Cell* displayedCell = buf.at({chunkX, chunkY}).getCell(cellLocalX, cellLocalY);
                    displayedChar = displayedCell->getChar();
                    color = displayedCell->getColor();
                } else {
                    displayedChar = ' ';
                    color = 0;
                }

                if (x == playerX && y == playerY) {
                    displayedChar = '&';
                    color = Colors::WHITE_COLOR;
                }

                std::pair playerPos{playerX, playerY};
                const auto [dirPointerX, dirPointerY] = getCoordinatesInDirection(playerPos, direction);
                if (x == dirPointerX && y == dirPointerY) {
                    color = Colors::WHITE_COLOR;
                }

                std::cout << "\033[38;5;" << color << "m" << displayedChar << "\033[0m" << ' ';
            } std::cout << '\n';
        }
        std::cout.flush();
        //
        // for (int y = topViewportY; y < bottomViewportY; y++) {
        //     // left side padding
        //     for (int x = 0; x < cols / 2 - VIEWPORT_COLS; x++)
        //         std::cout << ' ';
        //     // paste buffer
        //     for (int x = leftViewportX; x < rightViewportX; x++) {
        //         if (x == playerX && y == playerY ) {
        //             std::cout << '&' << ' ';
        //             continue;
        //         }
        //
        //         char displayedChar = baseLayer.at(y).at(x).getChar();
        //         if (entityLayer.at(y).at(x) != ' ') {
        //             displayedChar = entityLayer.at(y).at(x);
        //         }
        //
        //         const auto& directionPointer = getCoordinatesInDirection({playerX, playerY}, direction);
        //         if (x == directionPointer.first && y == directionPointer.second) {
        //             std::cout << "\033[38;5;" << Colors::WHITE_COLOR << "m" << displayedChar << "\033[0m" << ' ';
        //             continue;
        //         }
        //
        //         int color = colorLayer.at(y).at(x);
        //         if (isItalicLayer.at(y).at(x)) {
        //             std::cout << "\033[3m\033[38;5;" << color << "m" << displayedChar << "\033[0m\033[23m" << ' ';
        //             continue;
        //         }
        //         std::cout << "\033[38;5;" << color << "m" << displayedChar << "\033[0m" << ' ';
        //     } std::cout << '\n';
        // }
        // std::cout.flush();
    }

    void resetShader() {
        for (auto& chunk: buf | std::views::values) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    Cell* c = chunk.getCell(x, y);
                    c->setColor(Colors::FIELD_COLOR);
                }
            }
        }
    }

    // shader for walls
    void wallShader() {
        for (auto& chunk: buf | std::views::values) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    Cell* cell = chunk.getCell(x, y);
                    if (cell->getChar() != '.') {
                        cell->setColor(Colors::WALL_COLOR);
                    }
                }
            }
        }
    }

    // shader for light
    void lightShader() {
        resetShader();
        wallShader();
        for (auto& chunk: buf | std::views::values) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    if (chunk.getCell(x, y)->getChar() == '$') {
                        for (int k = 0; k < 25; k++) {
                            const int lightFarX = x - 2 + k % 5;
                            const int lightFarY = y - 2 + (k - k % 5) / 5;

                            Cell* cell = getCellFromChunk(chunk, lightFarX, lightFarY);
                            if (!cell) continue;

                            const char ch = cell->getChar();
                            if (ch == '$') continue;
                            if (ch != '.') {
                                if (cell->getColor() != Colors::WALL_LIGHT_NEAR_COLOR)
                                    cell->setColor(Colors::WALL_LIGHT_FAR_COLOR);
                                continue;
                            }
                            if (cell->getColor() != Colors::LIGHT_NEAR_COLOR)
                                cell->setColor(Colors::LIGHT_FAR_COLOR);
                        }

                        for (int k = 0; k < 9; k++) {
                            const int lightNearX = x - 1 + k % 3;
                            const int lightNearY = y - 1 + (k - k % 3) / 3;

                            Cell* cell = getCellFromChunk(chunk, lightNearX, lightNearY);
                            if (!cell) continue;

                            const char ch = cell->getChar();
                            if (ch == '$') continue;
                            if (ch != '.') {
                                cell->setColor(Colors::WALL_LIGHT_NEAR_COLOR);
                                continue;
                            } cell->setColor(Colors::LIGHT_NEAR_COLOR);
                        }

                        chunk.getCell(x, y)->setColor(Colors::LIGHT_SOURCE_COLOR);
                    }
                }
            }
        }
    }

    void updateAnimations() {
        // for (auto& entity : entities) {
        //     entity->update();
        //     int x = entity->getChunkX();
        //     int y = entity->getChunkY();
        //     entityLayer.at(y).at(x) = entity->getChar();
        // }
        //
        // if (lastTick + 50 <= ticks) {
        //     lastTick = ticks;
        //     for (int y = 0; y < ROWS; y++) {
        //         for (int x = 0; x < COLS; x++) {
        //             if (baseLayer.at(y).at(x).getChar() == '$') {
        //                 isItalicLayer.at(y).at(x) = !isItalicLayer[y][x];
        //                 continue;
        //             }
        //             isItalicLayer.at(y).at(x) = false;
        //         }
        //     }
        // }
        //
        // lightShader();
    }

    void updateEntityLayer() {
        // for (int y = 0; y < ROWS; y++) {
        //     for (int x = 0; x < COLS; x++) {
        //         entityLayer.at(y).at(x) = ' ';
        //     }
        // }
        // for (const auto& entity : entities) {
        //     int x = entity->getChunkX();
        //     int y = entity->getChunkY();
        //     entityLayer.at(y).at(x) = entity->getChar();
        //     colorLayer.at(y).at(x) = entity->getColor();
        // }
    }
}
