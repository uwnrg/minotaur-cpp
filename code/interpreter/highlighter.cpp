#include "highlighter.h"

QString keywordPattern(const QString &k) {
    return "\\b" + k + "\\b";
}

static const QString KEYWORDS[] = {
        "and", "as", "assert", "break", "class", "continue", "def", "del",
        "elif", "else", "except", "finally", "for", "from", "global",
        "if", "import", "in", "is", "lambda", "nonlocal", "not", "or",
        "pass", "raise", "return", "try", "while", "with", "yield"
};

static const QString CONSTANTS[] = {
        "None", "True", "False"
};

static const QString FUNCTIONS[] = {
        "abs", "all", "any", "ascii", "bin", "bool", "bytearray", "bytes",
        "callable", "chr", "classmethod", "compile", "complex", "delattr",
        "dict", "dir", "divmod", "enumerate", "eval", "exec", "filter",
        "float", "format", "frozenset", "getattr", "globals", "hasattr",
        "hash", "help", "hex", "id", "input", "int", "isinstance", "issubclass",
        "iter", "len", "list", "locals", "map", "max", "memoryview", "min",
        "next", "object", "oct", "open", "ord", "pow", "print", "property",
        "range", "repr", "reversed", "round", "set", "setattr", "slice",
        "sorted", "staticmethod", "str", "sum", "super", "tuple", "type",
        "vars", "zip"
};

static const QString SYMBOLS[] = {
        "=", "+", "-", "(", ")", "*", "&", "^", "%", "~", "!",
        "{", "}", "[", "]", ":", ";", ">", "<", ",", ".", "|"
};

HighLighter::HighLighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent) {
    Rule rule;
    m_keyword_format.setForeground(QColor(249, 38, 114));
    m_keyword_format.setFontWeight(QFont::Bold);
    for (auto &keyword : KEYWORDS) {
        rule.pattern = Regex(keywordPattern(keyword));
        rule.format = m_keyword_format;
        m_rules.append(rule);
    }

    m_symbol_format.setForeground(QColor(249, 38, 114));
    for (auto &symbol : SYMBOLS) {
        rule.pattern = Regex("\\" + symbol);
        rule.format = m_symbol_format;
        m_rules.append(rule);
    }

    m_number_format.setForeground(QColor(174, 129, 255));
    rule.pattern = Regex("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
    rule.format = m_number_format;
    m_rules.append(rule);

    m_constant_format.setForeground(QColor(213, 28, 255));
    m_constant_format.setFontWeight(QFont::Bold);
    for (auto &constant : CONSTANTS) {
        rule.pattern = Regex(keywordPattern(constant));
        rule.format = m_constant_format;
        m_rules.append(rule);
    }

    m_builtin_format.setForeground(QColor(181, 255, 36));
    m_builtin_format.setFontItalic(true);
    for (auto &builtin : FUNCTIONS) {
        rule.pattern = Regex(keywordPattern(builtin));
        rule.format = m_builtin_format;
        m_rules.append(rule);
    }

    m_quote_format.setForeground(QColor::fromRgb(230, 219, 116));
    rule.pattern = Regex("\".*\"");
    rule.format = m_quote_format;
    m_rules.append(rule);

    m_comment_format.setForeground(QColor(224, 216, 180));
    rule.pattern = Regex("#[^\\n]*");
    rule.format = m_comment_format;
    m_rules.append(rule);
}

void HighLighter::highlightBlock(const QString &text) {
    for (auto &rule : m_rules) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
