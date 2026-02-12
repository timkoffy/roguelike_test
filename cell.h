#ifndef ROGUELIKE_TEST_CELL_H
#define ROGUELIKE_TEST_CELL_H

class Cell {
public:
    Cell() = default;
    Cell(const int x, const int y, const char ch, const int color) : x(x), y(y), ch(ch), color(color) {}
    Cell(const int x, const int y, const char ch) : x(x), y(y), ch(ch) {}

    virtual ~Cell() = default;

    [[nodiscard]] int getX() const { return x; }
    [[nodiscard]] int getY() const { return y; }
    [[nodiscard]] char getChar() const { return ch; }
    [[nodiscard]] int getColor() const { return color; }

    void setChunkPosition(const int newX, const int newY) {
        x = newX;
        y = newY;
    }

    void setChar(const char newChar) { ch = newChar; }
    void setColor(const int newColor) { color = newColor; }

protected:
    int x = 0;
    int y = 0;
    char ch = ' ';
    int color = 0;
};


#endif //ROGUELIKE_TEST_CELL_H