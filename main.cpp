#include "game.hpp"
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <unordered_map>

#include "chunk.h"

int main() {
    // printf("%d", -3 / 2);

    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    system("stty -icanon -echo");

    struct PairHash {
        size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    std::unordered_map<std::pair<int, int>, Chunk, PairHash> buf;

    Chunk chunk;
    const int chunkSize = chunk.getChunkSize();
    for (int y = 0; y < chunkSize; y++) {
        for (int x = 0; x < chunkSize; x++) {
            Cell cell;
            cell.setChar('.');
            cell.setPosition(x, y);
            chunk.setCell(x, y, cell);
        }
    }
    chunk.setPosition(0, 0);
    buf[std::make_pair(0, 0)] = chunk;

    Chunk chunk1;
    for (int y = 0; y < chunkSize; y++) {
        for (int x = 0; x < chunkSize; x++) {
            Cell cell;
            cell.setChar('.');
            cell.setPosition(x, y);
            chunk1.setCell(x, y, cell);
        }
    }
    chunk1.setPosition(-1, 0);
    buf[std::make_pair(-1, 0)] = chunk1;

    int rows = w.ws_row;
    int cols = w.ws_col;

    int playerX = 0;
    int playerY = 1;

    int leftViewportX = playerX - Game::VIEWPORT_COLS / 2;
    int topViewportY = playerY - Game::VIEWPORT_ROWS / 2;

    int rightViewportX = leftViewportX + Game::VIEWPORT_COLS;
    int bottomViewportY = topViewportY + Game::VIEWPORT_ROWS;

    std::cout << "\033[2J\033[1;1H\033[?25l";
    std::cout << "WASD / HJKL - move, QE - rotate, F - block, R - light, C - destroy, Z - exit\n\n";
    {
        // top side padding
        int y = (rows - Game::VIEWPORT_ROWS) / 2 - 1;
        while (y > 0) {
            std::cout << '\n';
            y--;
        }
    }

    for (int testY = topViewportY; testY < bottomViewportY; testY++) {
        int y = -testY;
        // left side padding
        for (int x = 0; x < cols / 2 - Game::VIEWPORT_COLS; x++)
            std::cout << ' ';
        // paste buffer
        for (int x = leftViewportX; x < rightViewportX; x++) {

            int chunkX = x / CHUNK_SIZE;
            if (x < 0) chunkX--;

            int chunkY = y / CHUNK_SIZE;
            if (y < 0) chunkY--;

            char displayedChar;
            auto it = buf.find({chunkX, chunkY});
            if (it != buf.end()) {
                int cellChunkX = (x - (chunkX * chunkSize));
                int cellChunkY = (y - (chunkY * chunkSize));
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

    // Game::createInitialField();
    //
    // Game::runTime(w.ws_row, w.ws_col);
}
