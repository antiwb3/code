#ifndef LUA_SYNTAXHIGHLIGHTER_H
#define LUA_SYNTAXHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>

class LuaSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    LuaSyntaxHighlighter(QTextDocument *parent = 0);

    void Dohighlight(const QString &text);

protected:
    virtual void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentExpression;
    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat tagFormat;
    QTextCharFormat attributeFormat;
    QTextCharFormat attributeContentFormat;
    QTextCharFormat commentFormat;
};

#endif