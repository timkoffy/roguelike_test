#include "draw.h"
#include <iostream>
#include "game.h"

void draw::drawField(char* buf, int rows, int cols) {
    std::cout << "\033[2J\033[1;1H";
    {
        int i = (rows - ROWS) / 2 - 1;
        while (i > 0) {
            std::cout << '\n';
            i--;
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < cols / 2 - COLS; j++)
            std::cout << ' ';
        for (int j = 0; j < COLS; j++) {
            if (j == game::playerX && i == game::playerY ) {
                std::cout << '&' << ' ';
                continue;
            }
            int color = game::brushedBlocks[i][j];
            std::cout << "\033[38;5;" << color << "m" << buf[i * COLS + j] << "\033[0m" << ' ';
        } std::cout << '\n';
    }
}

void draw::wallShader(char* buf) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (buf[i * COLS + j] != '.') {
                game::brushedBlocks[i][j] = WALL_COLOR;
            }
        }
    }
}

void draw::lightShader(char* buf) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (buf[i * COLS + j] == '$') {
                for (int k = 0; k < 25; k++) {
                    int row = i - 2 + (k - k%5) / 5,
                        col = j - 2 + k % 5;
                    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                        game::brushedBlocks[row][col] = 130;
                        if (buf[row * COLS + col] != '.')
                            game::brushedBlocks[row][col] = 223;
                    }
                }
                for (int k = 0; k < 9; k++) {
                    int row = i - 1 + (k - k%3) / 3,
                        col = j - 1 + k % 3;
                    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                        game::brushedBlocks[row][col] = 172;
                        if (buf[row * COLS + col] != '.')
                            game::brushedBlocks[row][col] = 229;
                    }
                }
                game::brushedBlocks[i][j] = 226;
            }
        }
    }
}

