#include "serialmonitor.h"

#include <QVBoxLayout>
#include <QPlainTextEdit>

SerialMonitor::SerialMonitor(QWidget *parent)
    : QDialog(parent),
      m_layout(new QVBoxLayout(this)),
      m_display(new QPlainTextEdit(this)) {
    setWindowTitle("Serial Monitor");
    m_layout->addWidget(m_display.get());
    setMinimumSize(400, 500);
    setLayout(m_layout.get());
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(12);
    m_display->setFont(font);
    m_display->setReadOnly(true);
}

void SerialMonitor::append_text(const std::string &text) {
    m_display->moveCursor(QTextCursor::End);
    m_display->insertPlainText(QString::fromStdString(text));
    m_display->moveCursor(QTextCursor::End);
}
