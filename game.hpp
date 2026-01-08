#pragma once
#include <chrono>
#include <utility>

namespace Game {
    constexpr int ROWS = 50;
    constexpr int COLS = 50;
    constexpr int VIEWPORT_ROWS = 11;
    constexpr int VIEWPORT_COLS = 19;
    constexpr auto FRAME_TIME = std::chrono::milliseconds(16);

    namespace Colors {
        constexpr int FIELD_COLOR = 59;
        constexpr int WALL_COLOR = 138;
        constexpr int LIGHT_SOURCE_COLOR = 226;
        constexpr int LIGHT_NEAR_COLOR = 172;
        constexpr int LIGHT_FAR_COLOR = 130;
        constexpr int WALL_LIGHT_NEAR_COLOR = 229;
        constexpr int WALL_LIGHT_FAR_COLOR = 223;
    }

    enum Direction {
        UP = 0,
        LEFT,
        DOWN,
        RIGHT
    };

    extern char buf[ROWS][COLS];
    extern int colorLayer[ROWS][COLS];
    extern int playerX;
    extern int playerY;
    extern int direction;
    extern int ticks;
    extern int viewportCenterX;
    extern int viewportCenterY;

    // main functions
    void runTime(int rows, int cols);
    void readFromFile();
    void saveToFile();
    void editByteInFile(int x, int y, char ch);
    void createInitialField();
    void move(int dir);
    void placeBlockInFrontOfPlayer();
    void destroyBlockInFrontOfPlayer();

    // rendering functions
    void drawField(int rows, int cols);
    void updateViewportCenterCoordinates(int* leftViewportXPtr, int* topViewportYPtr);
    void wallShader();
    void lightShader();

    // side functions
    std::pair<int, int> getCoordinatesInDirection(std::pair<int, int> point, int dir);
}