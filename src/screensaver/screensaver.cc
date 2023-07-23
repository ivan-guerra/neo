#include "screensaver/screensaver.hpp"

#include <ncurses.h>

namespace neo {

ScreenSaver::ScreenSaver() : buffer_(0, 0) {
    initscr();
    cbreak();    /* disable line buffering */
    noecho();    /* disable character echoing */
    curs_set(0); /* hide the cursor */

    /* color setup, we assume the user has a color terminal */
    start_color();
    init_pair(Color::kWhite, COLOR_WHITE, COLOR_BLACK);
    init_pair(Color::kGreen, COLOR_GREEN, COLOR_BLACK);

    /* setup the delay time used when reading user keypresses */
    const int kDefaultInputDelayMs = 50;
    timeout(kDefaultInputDelayMs);

    /* fetch the screen dimensions */
    int width = 0;
    int height = 0;
    getmaxyx(stdscr, height, width);

    buffer_ = ScreenBuffer(width, height); /* create the screen buffer */
}

ScreenSaver::~ScreenSaver() { endwin(); }

void ScreenSaver::Draw() {
    clear();

    buffer_.Update(); /* refresh the screen buffer */

    auto raw_buffer = buffer_.GetBuffer();
    for (std::size_t i = 0; i < raw_buffer.size(); ++i) {
        for (std::size_t j = 0; j < raw_buffer.front().size(); ++j) {
            if (raw_buffer[i][j].c == '\0') {
                mvaddch(i, j, ' ');
            } else {
                if (raw_buffer[i][j].first) {
                    attron(COLOR_PAIR(Color::kWhite) | A_BOLD);
                    mvaddch(i, j, raw_buffer[i][j].c);
                    attroff(COLOR_PAIR(Color::kGreen) | A_BOLD);
                } else {
                    attron(COLOR_PAIR(Color::kGreen));
                    mvaddch(i, j, raw_buffer[i][j].c);
                    attroff(COLOR_PAIR(Color::kGreen));
                }
            }
        }
    }
    refresh();
}

}  // namespace neo
