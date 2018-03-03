#ifndef MINOTAUR_CPP_CODEEDITOR_H
#define MINOTAUR_CPP_CODEEDITOR_H

#include <memory>

#include <QPlainTextEdit>

#include "highlighter.h"
#include "../utility/utility.h"

class CodeEditor;

class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(CodeEditor *editor);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *m_code_editor;
};

class CodeEditor : public QPlainTextEdit {
Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor() override;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private Q_SLOTS:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    std::unique_ptr<LineNumberArea> m_line_number_area;
    std::unique_ptr<HighLighter> m_highlighter;
};


#endif //MINOTAUR_CPP_CODEEDITOR_H
