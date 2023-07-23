#include <unistd.h>

#include "screensaver/screensaver.hpp"

int main() {
    const int kDefaultRefreshRateUsec = 75000;

    neo::ScreenSaver screensaver;
    while (!screensaver.Quit()) {
        screensaver.Draw();
        usleep(kDefaultRefreshRateUsec);
    }
    return 0;
}
