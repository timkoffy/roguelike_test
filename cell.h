#ifndef ROGUELIKE_TEST_CELL_H
#define ROGUELIKE_TEST_CELL_H

class Cell {
public:
    Cell() : x(0), y(0), ch(' ') {}

    Cell(int x, int y, char ch) : x(x), y(y), ch(ch) {}

    virtual ~Cell() {}

    int getX() const { return x; }
    int getY() const { return y; }
    char getChar() const { return ch; }

    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    void setChar(char newChar) {
        ch = newChar;
    }

protected:
    int x;
    int y;
    char ch;
};


#endif //ROGUELIKE_TEST_CELL_H