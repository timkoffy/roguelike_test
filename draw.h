//
// Created by timkoffy on 1/7/26.
//

#ifndef ROGUELIKE_TEST_DRAW_H
#define ROGUELIKE_TEST_DRAW_H

class draw {
public:
    static void drawField(char* buf, int rows, int cols);
    static void lightShader(char* buf);
    static void wallShader(char* buf);
    static constexpr int FIELD_COLOR = 59;
    static constexpr int WALL_COLOR = 138;
};


#endif //ROGUELIKE_TEST_DRAW_H