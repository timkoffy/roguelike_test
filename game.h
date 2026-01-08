#ifndef ROGUELIKE_TEST_GAME_H
#define ROGUELIKE_TEST_GAME_H
#include <vector>

constexpr int ROWS = 10;
constexpr int COLS = 20;
constexpr int BUFFER_SIZE = ROWS * COLS;

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

    static std::vector<std::vector<int>> brushedBlocks;
    static int playerX;
    static int playerY;
    static int direction;
    static int blocksCount;
};


#endif //ROGUELIKE_TEST_GAME_H