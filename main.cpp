#include "game.hpp"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int main() {
    // int colorBG = 94;
    // int colorFG = 226;
    // std::cout << "\033[48;5;" << colorBG << "m" << "\033[38;5;" << colorFG << "m" << '=' << "\033[0m" << '\n';
    winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    system("stty -icanon -echo");

    // Game::createInitialField();

    Game::runTime(w.ws_row, w.ws_col);
}
