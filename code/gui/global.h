#ifndef MINOTAUR_CPP_GLOBAL_H
#define MINOTAUR_CPP_GLOBAL_H

#include "mainwindow.h"

struct Main {
    static MainWindow *&get();

private:
    static MainWindow *instance;
};

#endif //MINOTAUR_CPP_GLOBAL_H
