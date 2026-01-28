#pragma once
#include <chrono>
#include <utility>

#include "entity.h"

namespace Game {
    constexpr int ROWS = 100;
    constexpr int COLS = 100;
    constexpr int VIEWPORT_ROWS = 11;
    constexpr int VIEWPORT_COLS = 21;
    constexpr auto FRAME_TIME = std::chrono::milliseconds(16);

    namespace Colors {
        constexpr int FIELD_COLOR = 59;
        constexpr int WALL_COLOR = 102;
        constexpr int WHITE_COLOR = 15;

        constexpr int LIGHT_SOURCE_COLOR = 226;
        constexpr int LIGHT_NEAR_COLOR = 172;
        constexpr int LIGHT_FAR_COLOR = 94;

        constexpr int WALL_LIGHT_NEAR_COLOR = 229;
        constexpr int WALL_LIGHT_FAR_COLOR = 181;

        constexpr int ENTITY_ORC_COLOR = 72;
    }

    enum Direction {
        UP = 0,
        LEFT,
        DOWN,
        RIGHT
    };

    extern std::array<std::array<Cell, COLS>, ROWS> baseLayer;
    extern std::array<std::array<char, COLS>, ROWS> entityLayer;
    extern std::array<std::array<int, COLS>, ROWS> colorLayer;
    extern std::array<std::array<bool, COLS>, ROWS> isItalicLayer;

    extern std::vector<std::unique_ptr<Entity>> entities;

    extern int playerX;
    extern int playerY;
    extern int direction;
    extern int ticks;
    extern int lastTick;

    // main functions
    void runTime(int rows, int cols);
    void readFromFile();
    void saveToFile();
    void editByteInFile(int x, int y, char ch);
    void createInitialField();
    void move(int dir);
    void rotateDirection(bool isClockwise);
    void placeBlockInFrontOfPlayer(char block);
    void destroyBlockInFrontOfPlayer();

    // chunk managing
    void loadChunks();

    // rendering functions
    void drawField(int rows, int cols);
    void updateViewportCenterCoordinates(int* leftViewportXPtr, int* topViewportYPtr);
    void lightShader();
    void updateAnimations();
    void updateEntityLayer();

    // helper functions
    std::pair<int, int> getCoordinatesInDirection(std::pair<int, int> point, int dir);
    bool isBlockSolid(std::pair<int, int> point, Entity* thisEntity);
    bool isBlockSolidPlayer(std::pair<int, int> point);
}
