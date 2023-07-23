#ifndef SCREENSAVER_HPP_
#define SCREENSAVER_HPP_

#include <ncurses.h>

#include "screen_buffer/screen_buffer.hpp"

namespace neo {

/**
 * @brief A Matrix style digital rain screensaver using the ncurses library.
 */
class ScreenSaver {
   public:
    ScreenSaver();
    ~ScreenSaver();

    ScreenSaver(const ScreenSaver&) = default;
    ScreenSaver& operator=(const ScreenSaver&) = default;
    ScreenSaver(ScreenSaver&&) = default;
    ScreenSaver& operator=(ScreenSaver&&) = default;

    /** @brief Draw a single screensaver frame in the current terminal window.
     */
    void Draw();

    /** @brief Returns true if the user has pressed any key. */
    bool Quit() const { return (getch() != ERR); }

   private:
    enum Color {
        kWhite = 1,
        kGreen = 2,
    };

    ScreenBuffer buffer_;
};

}  // namespace neo

#endif
