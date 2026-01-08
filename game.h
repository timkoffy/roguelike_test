#ifndef ROGUELIKE_TEST_GAME_H
#define ROGUELIKE_TEST_GAME_H
#include <chrono>
#include <vector>

constexpr int ROWS = 20;
constexpr int COLS = 30;
constexpr int VIEWPORT_ROWS = 11;
constexpr int VIEWPORT_COLS = 19;
constexpr int BUFFER_SIZE = ROWS * COLS;

constexpr std::chrono::milliseconds FRAME_TIME(16);

class game {
public:
    static void runTime(int rows, int cols);
    static void readFromFile(char* buf);
    static void saveToFile(char* buf);
    static void editByteInFile(char* buf, int x, int y, char ch);
    static void createInitialField();

    static void move(char *buf, int dir);
    static void placeBlockInFrontOfPlayer(char* buf);
    static void destroyBlockInFrontOfPlayer(char* buf);

    static std::vector<std::vector<int>> colorLayer;
    static int playerX;
    static int playerY;
    static int direction;
    static int ticks;

};


#endif //ROGUELIKE_TEST_GAME_H