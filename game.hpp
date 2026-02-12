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

    constexpr int VIEWPORT_ROWS = 15; // 11 canon
    constexpr int VIEWPORT_COLS = VIEWPORT_ROWS * 2 - 1;
    constexpr auto FRAME_TIME = std::chrono::milliseconds(16); // 16 = 60fps

    enum Direction {
        UP = 0,
        LEFT,
        DOWN,
        RIGHT
    };

    inline std::unordered_map<std::pair<int, int>, Chunk, PairHash> buf;
    inline std::vector<std::pair<int, int>> chunksEdited;
    inline int chunkCount;

    inline std::vector<std::unique_ptr<Entity>> entities;

    inline int playerX;
    inline int playerY;
    inline int direction;
    inline int ticks;
    inline int lastTick;

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
    std::pair<int, int> getChunkCoords(const std::pair<int, int>& point);
    std::pair<int, int> getCoordinatesInDirection(const std::pair<int, int>& point, int dir);
    char getCharOnPoint(std::pair<int, int>& point);
    Cell* getCellFromChunk(Chunk& chunk, int x, int y);
    bool isBlockSolid(std::pair<int, int>& point, const Entity* thisEntity);
    bool isBlockSolidPlayer(const std::pair<int, int>& point);
    void writeInt32(std::fstream* file, int value);
}