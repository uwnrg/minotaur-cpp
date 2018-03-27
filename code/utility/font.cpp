#include "font.h"

#include <QFontMetrics>

#define FONT_FAMILY_COURIER "Courier"

QFont qfont::courier(int pt_size) {
    QFont font;
    font.setFamily(FONT_FAMILY_COURIER);
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(pt_size);
    return font;
}

int qfont::tab_stop(const QFont &font, int tab_width) {
    QFontMetrics metrics(font);
    return tab_width * metrics.width(' ');
}
