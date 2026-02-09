#pragma once
#include <chrono>
#include <unordered_map>

#include "chunk.h"
#include "entity.h"

namespace Game {
    struct PairHash {
        size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    constexpr int ROWS = 90;
    constexpr int COLS = 90;
    constexpr int VIEWPORT_ROWS = 11;
    constexpr int VIEWPORT_COLS = 21;
    constexpr auto FRAME_TIME = std::chrono::milliseconds(16);

    enum Direction {
        UP = 0,
        LEFT,
        DOWN,
        RIGHT
    };

    extern std::unordered_map<std::pair<int, int>, Chunk, PairHash> buf;
    extern std::vector<Chunk> chunksEdited;
    extern int chunkCount;

    extern int playerX;
    extern int playerY;
    extern int direction;
    extern int ticks;
    extern int lastTick;

    // game field functions
    void runTime(int rows, int cols);
    void placeBlockInFrontOfPlayer(char ch);
    void destroyBlockInFrontOfPlayer();

    // entity functions
    void move(int dir);
    void rotateDirection(bool isClockwise);
    void spawnEntity(int x, int y);

    // chunk managing
    void readFromFile();
    void saveToFile();
    void createInitialField();
    void createChunk(int chunkX, int chunkY);
    void loadChunks();

    // rendering functions
    void drawField(int rows, int cols);
    void updateViewportCenterCoordinates(int* leftViewportXPtr, int* topViewportYPtr);
    void lightShader();
    void fixCoordOutOfChunk(int* coord, int* chunkCoord);
    void updateAnimations();
    void updateEntityLayer();

    // helper functions
    std::pair<int, int> getCoordinatesInDirection(std::pair<int, int> point, int dir);
    std::pair<int, int> getChunkCoords(std::pair<int, int> point);
    char getCharOnPoint(const std::pair<int, int> &point);
    bool isBlockSolid(std::pair<int, int> point, Entity* thisEntity);
    bool isBlockSolidPlayer(std::pair<int, int> point);
}
