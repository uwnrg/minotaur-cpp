#ifndef MINOTAUR_CPP_FONT_H
#define MINOTAUR_CPP_FONT_H

#include <QFont>

namespace qfont {

    QFont courier(int pt_size = 12);

    int tab_stop(const QFont &font, int tab_width = 4);

}

#endif //MINOTAUR_CPP_FONT_H
