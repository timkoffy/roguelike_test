#ifndef ROGUELIKE_TEST_CELL_H
#define ROGUELIKE_TEST_CELL_H

class Cell {
public:
    Cell() : x(0), y(0), ch(' '), color(0) {}
    Cell(int x, int y, char ch, int color) : x(x), y(y), ch(ch), color(color) {}
    Cell(int x, int y, char ch) : x(x), y(y), ch(ch), color(0) {}

    virtual ~Cell() {}

    int getX() const { return x; }
    int getY() const { return y; }
    char getChar() const { return ch; }
    int getColor() const { return color; }

    void setChunkPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    void setChar(char newChar) { ch = newChar; }
    void setColor(int newColor) { color = newColor; }

protected:
    int x;
    int y;
    char ch;
    int color;
};


#endif //ROGUELIKE_TEST_CELL_H