#include "game.hpp"
#include <iostream>

namespace Game {
    // draw viewport field (segment from buffer)
    void drawField(int rows, int cols) {
        std::cout << "\033[2J\033[1;1H\033[?25l";
        std::cout << "ticks: " << ticks << " x: " << playerX << " y: " << playerY;
        {
            // top side padding
            int i = (rows - VIEWPORT_ROWS) / 2;
            while (i > 0) {
                std::cout << '\n';
                i--;
            }
        }

        int leftViewportX, topViewportY;
        updateViewportCenterCoordinates(&leftViewportX, &topViewportY);
        int rightViewportX = leftViewportX + VIEWPORT_COLS,
            bottomViewportY = topViewportY + VIEWPORT_ROWS;

        for (int i = topViewportY; i < bottomViewportY; i++) {
            // left side padding
            for (int j = 0; j < cols / 2 - VIEWPORT_COLS; j++)
                std::cout << ' ';
            // paste buffer
            for (int j = leftViewportX; j < rightViewportX; j++) {
                if (j == playerX && i == playerY ) {
                    std::cout << '&' << ' ';
                    continue;
                }
                const auto& directionPointer = getCoordinatesInDirection({playerX, playerY}, direction);
                if (j == directionPointer.first && i == directionPointer.second) {
                    std::cout << "\033[38;5;" << Colors::DIRECTION_POINTER_COLOR << "m" << buf[i][j] << "\033[0m" << ' ';
                    continue;
                }
                int color = colorLayer[i][j];
                if (isItalicLayer[i][j]) {
                    std::cout << "\033[3m\033[38;5;" << color << "m" << buf[i][j] << "\033[0m\033[23m" << ' ';
                    continue;
                }
                std::cout << "\033[38;5;" << color << "m" << buf[i][j] << "\033[0m" << ' ';
            } std::cout << '\n';
        }
        std::cout.flush();
    }

    // update viewport range
    void updateViewportCenterCoordinates(int* leftViewportXPtr, int* topViewportYPtr) {
        viewportCenterX = playerX;
        viewportCenterY = playerY;

        int leftViewportX = viewportCenterX - VIEWPORT_COLS / 2;
        int topViewportY = viewportCenterY - VIEWPORT_ROWS / 2;

        if (leftViewportX < 0) leftViewportX = 0;
        else if (leftViewportX + VIEWPORT_COLS >= COLS) leftViewportX = COLS - VIEWPORT_COLS;

        if (topViewportY < 0) topViewportY = 0;
        else if (topViewportY + VIEWPORT_ROWS >= ROWS) topViewportY = ROWS - VIEWPORT_ROWS;

        *leftViewportXPtr = leftViewportX;
        *topViewportYPtr = topViewportY;
    }

    void resetShader() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                colorLayer[i][j] = Colors::FIELD_COLOR;
            }
        }
    }

    // shader for walls
    void wallShader() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (buf[i][j] != '.') {
                    colorLayer[i][j] = Colors::WALL_COLOR;
                }
            }
        }
    }

    // shader for light
    void lightShader() {
        resetShader();
        wallShader();

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (buf[i][j] == '$') {
                    for (int k = 0; k < 25; k++) {
                        int row = i - 2 + (k - k%5) / 5,
                            col = j - 2 + k % 5;
                        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                            if (buf[row][col] == '$') continue;
                            if (buf[row][col] != '.') {
                                if (colorLayer[row][col] != Colors::WALL_LIGHT_NEAR_COLOR)
                                    colorLayer[row][col] = Colors::WALL_LIGHT_FAR_COLOR;
                                continue;
                            }
                            if (colorLayer[row][col] != Colors::LIGHT_NEAR_COLOR)
                                colorLayer[row][col] = Colors::LIGHT_FAR_COLOR;
                        }
                    }
                    for (int k = 0; k < 9; k++) {
                        int row = i - 1 + (k - k%3) / 3,
                            col = j - 1 + k % 3;
                        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                            if (buf[row][col] == '$') continue;
                            if (buf[row][col] != '.') {
                                colorLayer[row][col] = Colors::WALL_LIGHT_NEAR_COLOR;
                                continue;
                            } colorLayer[row][col] = Colors::LIGHT_NEAR_COLOR;
                        }
                    }
                    colorLayer[i][j] = Colors::LIGHT_SOURCE_COLOR;
                }
            }
        }
    }

    void updateAnimations() {
        if (lastTick + 50 == ticks) {
            lastTick = ticks;
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (buf[i][j] == '$') {
                        isItalicLayer[i][j] = !isItalicLayer[i][j];
                        continue;
                    }
                    isItalicLayer[i][j] = false;
                }
            }
        }
    }
}