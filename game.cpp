#include "game.hpp"
#include <chrono>
#include <iostream>
#include <thread>

namespace Game {
    char buf[ROWS][COLS];
    int colorLayer[ROWS][COLS];
    int playerX = 0;
    int playerY = 0;
    int direction = 0;
    int ticks = 0;
    int viewportCenterX = 0;
    int viewportCenterY = 0;

    void runTime(int rows, int cols) {
        std::cout << "\033[2J\033[1;1H" << "press any key...";

        readFromFile();
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
                    case 'c': destroyBlockInFrontOfPlayer(); break;
                    case 'z': std::cout << "\033[2J\033[1;1H"; return;
                    default: break;
                }
            }

            ticks++;

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
        fread(buf, 1, ROWS * COLS, f);
        fread(&playerX, sizeof(int), 1, f);
        fread(&playerY, sizeof(int), 1, f);
        fread(&direction, sizeof(int), 1, f);
        fclose(f);
    }

    void saveToFile() {
        FILE* f = fopen("data.dat", "wb");
        fwrite(buf, 1, ROWS * COLS, f);
        fwrite(&playerX, sizeof(int), 1, f);
        fwrite(&playerY, sizeof(int), 1, f);
        fwrite(&direction, sizeof(int), 1, f);
        fclose(f);
    }

    void editByteInFile(int x, int y, char ch) {
        FILE* f = fopen("data.dat", "rb+");
        fseek(f, y * COLS + x, SEEK_SET);
        fwrite(&ch, 1, 1, f);
        buf[y][x] = ch;
        fclose(f);
    }

    // reset all data
    void createInitialField() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                buf[i][j] = '.';
            }
        }

        buf[8][17] = '$';
        buf[3][5] = '$';

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
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, dir);
        int x = newCoords.first, y = newCoords.second;

        if (buf[y][x] != '.') {
            direction = dir;
            return;
        }
        playerX = x;
        playerY = y;
        direction = dir;
        saveToFile();
    }

    void rotateDirection(bool isClockwise) {
        if (isClockwise) {
            direction = (4 + direction - 1) % 4;
            return;
        }
        direction = (direction + 1) % 4;
    }


    void placeBlockInFrontOfPlayer(char block) {
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
        int x = newCoords.first, y = newCoords.second;

        if (buf[y][x] != '.')
            return;

        editByteInFile(x, y, block);
        colorLayer[y][x] = Colors::WALL_COLOR;
        lightShader();
    }

    void destroyBlockInFrontOfPlayer() {
        auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
        int x = newCoords.first, y = newCoords.second;

        // if (buf[y][x] != '@')
        //     return;

        editByteInFile(x, y, '.');
        colorLayer[y][x] = Colors::FIELD_COLOR;
        lightShader();
    }
}