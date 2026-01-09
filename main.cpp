#include "game.hpp"
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    system("stty -icanon -echo");

    Game::createInitialField();

    Game::runTime(w.ws_row, w.ws_col);
}