#include "game.hpp"
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <unordered_map>

#include "chunk.h"

int main() {
    printf("%d", -4 % 2);

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
            cell.setPosition(y, x);
            chunk.setCell(y, x, cell);
        }
    } chunk.setPosition(0, 0);
    buf[std::make_pair(0, 0)] = chunk;

    Chunk chunk1;
    for (int y = 0; y < chunkSize; y++) {
        for (int x = 0; x < chunkSize; x++) {
            Cell cell;
            cell.setChar('.');
            cell.setPosition(y, x);
            chunk1.setCell(y, x, cell);
        }
    } chunk1.setPosition(1, 0);
    buf[std::make_pair(1, 0)] = chunk1;

    int rows = w.ws_row;
    int cols = w.ws_col;

    int leftViewportX, topViewportY;
    Game::updateViewportCenterCoordinates(&leftViewportX, &topViewportY);
    int rightViewportX = leftViewportX + Game::VIEWPORT_COLS,
            bottomViewportY = topViewportY + Game::VIEWPORT_ROWS;

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

    for (int y = topViewportY; y < bottomViewportY; y++) {
        // left side padding
        for (int x = 0; x < cols / 2 - Game::VIEWPORT_COLS; x++)
            std::cout << ' ';
        // paste buffer
        for (int x = leftViewportX; x < rightViewportX; x++) {

            int chunkX = x / CHUNK_SIZE;
            int chunkY = y / CHUNK_SIZE;

            char displayedChar;
            auto it = buf.find({chunkX, chunkY});
            if (it != buf.end()) {
                displayedChar = buf[{chunkX, chunkY}].getCell(y % chunkSize, x % chunkSize).getChar();
            } else {
                displayedChar = ' ';
            }

            std::cout << displayedChar << ' ';
        } std::cout << '\n';
    }
    std::cout.flush();

    // Game::createInitialField();
    //
    // Game::runTime(w.ws_row, w.ws_col);
}
