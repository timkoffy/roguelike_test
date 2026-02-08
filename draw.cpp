#include "game.hpp"
#include <iostream>

#include "cell.h"
#include "chunk.h"

namespace Game {
    struct PairHash {
        size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

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

        for (int testY = topViewportY; testY < bottomViewportY; testY++) {
            int y = -testY;
            // left side padding
            for (int x = 0; x < cols / 2 - VIEWPORT_COLS; x++)
                std::cout << ' ';
            // paste buffer
            for (int x = leftViewportX; x < rightViewportX; x++) {

                int chunkX = x / CHUNK_SIZE;
                if (x < 0) chunkX--;

                int chunkY = y / CHUNK_SIZE;
                if (y < 0) chunkY--;

                char displayedChar;

                if (buf.find({chunkX, chunkY}) != buf.end()) {
                    int cellChunkX = (x - (chunkX * CHUNK_SIZE));
                    int cellChunkY = (y - (chunkY * CHUNK_SIZE));
                    displayedChar = buf[{chunkX, chunkY}].getCell(cellChunkX, cellChunkY).getChar();
                } else {
                    displayedChar = ' ';
                }

                if (x == playerX && y == playerY) {
                    displayedChar = '&';
                }

                std::cout << displayedChar << ' ';
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
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                colorLayer.at(y).at(x) = Colors::FIELD_COLOR;
            }
        }
    }

    // shader for walls
    void wallShader() {
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                if (baseLayer.at(y).at(x).getChar() != '.') {
                    colorLayer.at(y).at(x) = Colors::WALL_COLOR;
                }
            }
        }
    }

    // shader for light
    void lightShader() {
        resetShader();
        wallShader();

        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                if (baseLayer.at(y).at(x).getChar() == '$') {
                    for (int k = 0; k < 25; k++) {
                        int row = y - 2 + (k - k%5) / 5,
                            col = x - 2 + k % 5;
                        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                            char ch = baseLayer.at(row).at(col).getChar();
                            if (ch == '$') continue;
                            if (ch != '.') {
                                if (colorLayer.at(row).at(col) != Colors::WALL_LIGHT_NEAR_COLOR)
                                    colorLayer.at(row).at(col) = Colors::WALL_LIGHT_FAR_COLOR;
                                continue;
                            }
                            if (colorLayer.at(row).at(col) != Colors::LIGHT_NEAR_COLOR)
                                colorLayer.at(row).at(col) = Colors::LIGHT_FAR_COLOR;
                        }
                    }
                    for (int k = 0; k < 9; k++) {
                        int row = y - 1 + (k - k%3) / 3,
                            col = x - 1 + k % 3;
                        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                            char ch = baseLayer.at(row).at(col).getChar();
                            if (ch == '$') continue;
                            if (ch != '.') {
                                colorLayer.at(row).at(col) = Colors::WALL_LIGHT_NEAR_COLOR;
                                continue;
                            } colorLayer.at(row).at(col) = Colors::LIGHT_NEAR_COLOR;
                        }
                    }
                    colorLayer.at(y).at(x) = Colors::LIGHT_SOURCE_COLOR;
                }
            }
        }

        updateEntityLayer();
    }

    void updateAnimations() {
        for (auto& entity : entities) {
            entity->update();
            int x = entity->getX();
            int y = entity->getY();
            entityLayer.at(y).at(x) = entity->getChar();
        }

        if (lastTick + 50 <= ticks) {
            lastTick = ticks;
            for (int y = 0; y < ROWS; y++) {
                for (int x = 0; x < COLS; x++) {
                    if (baseLayer.at(y).at(x).getChar() == '$') {
                        isItalicLayer.at(y).at(x) = !isItalicLayer[y][x];
                        continue;
                    }
                    isItalicLayer.at(y).at(x) = false;
                }
            }
        }

        lightShader();
    }

    void updateEntityLayer() {
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                entityLayer.at(y).at(x) = ' ';
            }
        }
        for (const auto& entity : entities) {
            int x = entity->getX();
            int y = entity->getY();
            entityLayer.at(y).at(x) = entity->getChar();
            colorLayer.at(y).at(x) = entity->getColor();
        }
    }
}
