#include <sys/ioctl.h>
#include <unistd.h>

#include "game.h"

int main() {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    game::runTime(w.ws_row, w.ws_col);
}
