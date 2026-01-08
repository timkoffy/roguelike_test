#include "draw.h"
#include <iostream>
#include "game.h"

int draw::viewportCenterX = game::playerX;
int draw::viewportCenterY = game::playerY;

// draw viewport field (segment from buffer)
void draw::drawField(char* buf, int rows, int cols) {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "ticks: " << game::ticks;
    {
        // top side padding
        int i = (rows - VIEWPORT_ROWS) / 2;
        while (i > 0) {
            std::cout << '\n';
            i--;
        }
    }
    updateViewportCenterCoordinates();
    int leftViewportX = viewportCenterX - VIEWPORT_COLS / 2;
    int rightViewportX = leftViewportX + VIEWPORT_COLS;

    if (leftViewportX < 0)
        leftViewportX = 0;
    if (rightViewportX >= COLS)
        leftViewportX = COLS - VIEWPORT_COLS;

    int leftViewportY = viewportCenterY - VIEWPORT_ROWS / 2;
    int rightViewportY = leftViewportY + VIEWPORT_ROWS;

    if (leftViewportY < 0)
        leftViewportY = 0;
    if (rightViewportY >= ROWS)
        leftViewportY = ROWS - VIEWPORT_ROWS;

    for (int i = leftViewportY; i < leftViewportY + VIEWPORT_ROWS; i++) {
        // left side padding
        for (int j = 0; j < cols / 2 - VIEWPORT_COLS; j++)
            std::cout << ' ';
        // paste buffer
        for (int j = leftViewportX; j < leftViewportX + VIEWPORT_COLS; j++) {
            if (j == game::playerX && i == game::playerY ) {
                std::cout << '&' << ' ';
                continue;
            }
            int color = game::colorLayer[i][j];
            std::cout << "\033[38;5;" << color << "m" << buf[i * COLS + j] << "\033[0m" << ' ';
        } std::cout << '\n';
    }
}

void draw::updateViewportCenterCoordinates() {
    viewportCenterX = game::playerX;
    viewportCenterY = game::playerY;
}

// shader for walls
void draw::wallShader(char* buf) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (buf[i * COLS + j] != '.') {
                game::colorLayer[i][j] = WALL_COLOR;
            }
        }
    }
}

// shader for light
void draw::lightShader(char* buf) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (buf[i * COLS + j] == '$') {
                for (int k = 0; k < 25; k++) {
                    int row = i - 2 + (k - k%5) / 5,
                        col = j - 2 + k % 5;
                    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                        game::colorLayer[row][col] = 130;
                        if (buf[row * COLS + col] != '.')
                            game::colorLayer[row][col] = 223;
                    }
                }
                for (int k = 0; k < 9; k++) {
                    int row = i - 1 + (k - k%3) / 3,
                        col = j - 1 + k % 3;
                    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                        game::colorLayer[row][col] = 172;
                        if (buf[row * COLS + col] != '.')
                            game::colorLayer[row][col] = 229;
                    }
                }
                game::colorLayer[i][j] = 226;
            }
        }
    }
}

