#ifndef ROGUELIKE_TEST_CELL_H
#define ROGUELIKE_TEST_CELL_H

class Cell {
public:
    Cell() : chunkX(0), chunkY(0), ch(' '), color(0) {}

    Cell(int x, int y, char ch, int color) : chunkX(x), chunkY(y), ch(ch), color(color) {}
    Cell(int x, int y, char ch) : chunkX(x), chunkY(y), ch(ch), color(0) {}

    virtual ~Cell() {}

    int getChunkX() const { return chunkX; }
    int getChunkY() const { return chunkY; }
    char getChar() const { return ch; }
    int getColor() const { return color; }

    void setChunkPosition(int newX, int newY) {
        chunkX = newX;
        chunkY = newY;
    }

    void setChar(char newChar) { ch = newChar; }
    void setColor(int newColor) { color = newColor; }

protected:
    int chunkX;
    int chunkY;
    char ch;
    int color;
};


#endif //ROGUELIKE_TEST_CELL_H