#ifndef ROGUELIKE_TEST_CELL_H
#define ROGUELIKE_TEST_CELL_H

class Cell {
public:
    Cell() : chunkX(0), chunkY(0), ch(' ') {}

    Cell(int x, int y, char ch) : chunkX(x), chunkY(y), ch(ch) {}

    virtual ~Cell() {}

    int getChunkX() const { return chunkX; }
    int getChunkY() const { return chunkY; }
    char getChar() const { return ch; }

    void setChunkPosition(int newX, int newY) {
        chunkX = newX;
        chunkY = newY;
    }

    void setChar(char newChar) {
        ch = newChar;
    }

protected:
    int chunkX;
    int chunkY;
    char ch;
};


#endif //ROGUELIKE_TEST_CELL_H