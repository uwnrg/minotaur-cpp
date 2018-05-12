#include "global.h"

MainWindow *&Main::get() {
    return instance;
}

MainWindow *Main::instance = nullptr;
