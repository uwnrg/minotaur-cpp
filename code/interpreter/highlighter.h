#ifndef MINOTAUR_CPP_HIGHLIGHTER_H
#define MINOTAUR_CPP_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class HighLighter : public QSyntaxHighlighter {
Q_OBJECT

    typedef QRegularExpression Regex;
    typedef QTextCharFormat Format;

public:
    explicit HighLighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct Rule {
        Regex pattern;
        Format format;
    };
    QVector<Rule> m_rules;

    Format m_keyword_format;
    Format m_symbol_format;
    Format m_comment_format;
    Format m_constant_format;
    Format m_number_format;
    Format m_quote_format;
    Format m_builtin_format;
};


#endif //MINOTAUR_CPP_HIGHLIGHTER_H
