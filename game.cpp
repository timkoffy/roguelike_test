#include "game.h"

#include <iostream>
#include <unistd.h>

#include "draw.h"

std::vector<std::vector<int>> game::brushedBlocks;
int game::playerX = 0;
int game::playerY = 0;
int game::direction = 0;
int game::blocksCount = 0;

void game::runTime(int rows, int cols) {
    // createInitialField();

    std::cout << "\033[2J\033[1;1H" << "press any key...";
    char* buf = (char*)malloc(BUFFER_SIZE);
    readFromFile(buf);

    brushedBlocks.resize(ROWS, std::vector(COLS, -1));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            brushedBlocks[i][j] = draw::FIELD_COLOR;
        }
    }

    draw::wallShader(buf);

    draw::lightShader(buf);

    while (true) {
        system("stty -icanon -echo");

        int key = getchar();
        switch (key) {
            case 'w': move(buf, 0); break;
            case 'a': move(buf, 1); break;
            case 's': move(buf, 2); break;
            case 'd': move(buf, 3); break;
            case 'f': placeBlockInFrontOfPlayer(buf); break;
            case 'c': destroyBlockInFrontOfPlayer(buf); break;
            case 'q': free(buf); std::cout << "\033[2J\033[1;1H"; return;
            default: break;
        }

        draw::drawField(buf, rows, cols);
    }
}

void game::readFromFile(char* buf) {
    FILE* f = fopen("data.dat", "rb");
    fread(buf, 1, BUFFER_SIZE, f);
    fread(&playerX, sizeof(int), 1, f);
    fread(&playerY, sizeof(int), 1, f);
    fread(&direction, sizeof(int), 1, f);
    fclose(f);
}

void game::saveToFile(char* buf) {
    FILE* f = fopen("data.dat", "wb");
    fwrite(buf, 1, BUFFER_SIZE, f);
    fwrite(&playerX, sizeof(int), 1, f);
    fwrite(&playerY, sizeof(int), 1, f);
    fwrite(&direction, sizeof(int), 1, f);
    fclose(f);
}

void game::editByteInFile(char* buf, int x, int y, char ch) {
    FILE* f = fopen("data.dat", "rb+");
    fseek(f, y * COLS + x, SEEK_SET);
    fwrite(&ch, 1, 1, f);
    buf[y * COLS + x] = ch;
    fclose(f);
}

void game::createInitialField() {
    char* tmp = (char*)malloc(BUFFER_SIZE);

    for (int i = 0; i < BUFFER_SIZE; i++)
        tmp[i] = '.';
    tmp[8 * COLS + 17] = '$';
    tmp[3 * COLS + 5] = '$';
    playerX = 0;
    playerY = 0;
    direction = 0;
    saveToFile(tmp);
    free(tmp);
}

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

void game::move(char *buf, int dir) {
    auto newCoords = getCoordinatesInDirection({playerX, playerY}, dir);
    int x = newCoords.first, y = newCoords.second;

    if (buf[y * COLS + x] != '.') {
        direction = dir;
        return;
    }

    playerX = x;
    playerY = y;
    direction = dir;
    saveToFile(buf);
}


void game::placeBlockInFrontOfPlayer(char* buf) {
    auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
    int x = newCoords.first, y = newCoords.second;

    if (buf[y * COLS + x] != '.')
        return;

    editByteInFile(buf, x, y, '@');
    brushedBlocks[y][x] = draw::WALL_COLOR;
    draw::lightShader(buf);
}

void game::destroyBlockInFrontOfPlayer(char* buf) {
    auto newCoords = getCoordinatesInDirection({playerX, playerY}, direction);
    int x = newCoords.first, y = newCoords.second;

    if (buf[y * COLS + x] != '@')
        return;

    editByteInFile(buf, x, y, '.');
    brushedBlocks[y][x] = draw::FIELD_COLOR;
    draw::lightShader(buf);
}