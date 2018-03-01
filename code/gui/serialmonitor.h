#ifndef MINOTAUR_CPP_SERIALMONITOR_H
#define MINOTAUR_CPP_SERIALMONITOR_H

#include <QDialog>

#include <memory>

class QVBoxLayout;
class QPlainTextEdit;

class SerialMonitor : public QDialog {
    Q_OBJECT
public:
    explicit SerialMonitor(QWidget *parent = nullptr);

    Q_SLOT void append_text(const std::string &text);

private:
    std::unique_ptr<QVBoxLayout> m_layout;
    std::unique_ptr<QPlainTextEdit> m_display;
};

#endif //MINOTAUR_CPP_SERIALMONITOR_H
