#ifndef ROGUELIKE_TEST_CELL_H
#define ROGUELIKE_TEST_CELL_H

class Cell {
public:
    Cell() : xChunk(0), yChunk(0), ch(' ') {}

    Cell(int x, int y, char ch) : xChunk(x), yChunk(y), ch(ch) {}

    virtual ~Cell() {}

    int getX() const { return xChunk; }
    int getY() const { return yChunk; }
    char getChar() const { return ch; }

    void setPosition(int newX, int newY) {
        xChunk = newX;
        yChunk = newY;
    }

    void setChar(char newChar) {
        ch = newChar;
    }

protected:
    int xChunk;
    int yChunk;
    char ch;
};


#endif //ROGUELIKE_TEST_CELL_H