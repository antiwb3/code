#include "stdafx.h"
#include "luasyntaxhighlighter.h"

LuaSyntaxHighlighter::LuaSyntaxHighlighter(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    tagFormat.setForeground(Qt::blue);
    tagFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("(\\b+and\\b|\\b+\\b+break+\\b|\\b+\\b+do+\\b|\\b+\\b+else+\\b|\\b+elseif+\\b|\\b+end+\\b|\\b+false+\\b|\\b+for+\\b|\\b+function+\\b|\\b+if+\\b|\\b+in+\\b|\\b+local+\\b|\\b+nil+\\b|\\b+not+\\b|\\b+or+\\b|\\b+repeat+\\b|\\b+return+\\b|\\b+then+\\b|\\b+true+\\b|\\b+until+\\b|\\b+while+\\b)");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    tagFormat.setForeground(Qt::darkBlue);
    tagFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("(\\b+_VERSION+\\b|\\b+assert+\\b|\\b+collectgarbage+\\b|\\b+dofile+\\b|\\b+error+\\b|\\b+gcinfo+\\b|\\b+loadfile+\\b|\\b+loadstring+\\b|\\b+print+\\b|\\b+tonumber+\\b|\\b+tostring+\\b|\\b+type+\\b|\\b+unpack+\\b|\\b+_ALERT+\\b|\\b+_ERRORMESSAGE+\\b|\\b+_INPUT+\\b|\\b+_PROMPT+\\b|\\b+_OUTPUT+\\b|\\b+_STDERR+\\b|\\b+_STDIN+\\b|\\b+_STDOUT+\\b|\\b+call+\\b|\\b+dostring+\\b|\\b+foreach+\\b|\\b+foreachi+\\b|\\b+getn+\\b|\\b+globals+\\b|\\b+newtype+\\b|\\b+rawget+\\b|\\b+rawset+\\b|\\b+require+\\b|\\b+sort+\\b|\\b+tinsert+\\b|\\b+tremove+\\b|\\b+_G+\\b|\\b+getfenv+\\b|\\b+getmetatable+\\b|\\b+ipairs+\\b|\\b+loadlib+\\b|\\b+next+\\b|\\b+pairs+\\b|\\b+pcall+\\b|\\b+rawegal+\\b|\\b+rawget+\\b|\\b+rawset+\\b|\\b+require+\\b|\\b+setfenv+\\b|\\b+setmetatable+\\b|\\b+xpcall+\\b|\\b+string+\\b|\\b+table+\\b|\\b+math+\\b|\\b+coroutine+\\b|\\b+io+\\b|\\b+os+\\b|\\b+debug+\\b)");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    tagFormat.setForeground(Qt::cyan);
    tagFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("(\\b+abs+\\b|\\b+acos+\\b|\\b+asin+\\b|\\b+atan+\\b|\\b+atan2+\\b|\\b+ceil+\\b|\\b+cos+\\b|\\b+deg+\\b|\\b+exp+\\b|\\b+floor+\\b|\\b+format+\\b|\\b+frexp+\\b|\\b+gsub+\\b|\\b+ldexp+\\b|\\b+log+\\b|\\b+log10+\\b|\\b+max+\\b|\\b+min+\\b|\\b+mod+\\b|\\b+rad+\\b|\\b+random+\\b|\\b+randomseed+\\b|\\b+sin+\\b|\\b+sqrt+\\b|\\b+strbyte+\\b|\\b+strchar+\\b|\\b+strfind+\\b|\\b+strlen+\\b|\\b+strlower+\\b|\\b+strrep+\\b|\\b+strsub+\\b|\\b+strupper+\\b|\\b+tan+\\b|\\b+string.byte+\\b|\\b+string.char+\\b|\\b+string.dump+\\b|\\b+string.find+\\b|\\b+string.len+\\b|\\b+string.lower+\\b|\\b+string.rep+\\b|\\b+string.sub+\\b|\\b+string.upper+\\b|\\b+string.format+\\b|\\b+string.gfind+\\b|\\b+string.gsub+\\b|\\b+table.concat+\\b|\\b+table.foreach+\\b|\\b+table.foreachi+\\b|\\b+table.getn+\\b|\\b+table.sort+\\b|\\b+table.insert+\\b|\\b+table.remove+\\b|\\b+table.setn+\\b|\\b+math.abs+\\b|\\b+math.acos+\\b|\\b+math.asin+\\b|\\b+math.atan+\\b|\\b+math.atan2+\\b|\\b+math.ceil+\\b|\\b+math.cos+\\b|\\b+math.deg+\\b|\\b+math.exp+\\b|\\b+math.floor+\\b|\\b+math.frexp+\\b|\\b+math.ldexp+\\b|\\b+math.log+\\b|\\b+math.log10+\\b|\\b+math.max+\\b|\\b+math.min+\\b|\\b+math.mod+\\b|\\b+math.pi+\\b|\\b+math.rad+\\b|\\b+math.random+\\b|\\b+math.randomseed+\\b|\\b+math.sin+\\b|\\b+math.sqrt+\\b|\\b+math.tan+\\b)");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    tagFormat.setForeground(Qt::red);
    tagFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("(\\b+openfile+\\b|\\b+closefile+\\b|\\b+readfrom+\\b|\\b+writeto+\\b|\\b+appendto+\\b|\\b+remove+\\b|\\b+rename+\\b|\\b+flush+\\b|\\b+seek+\\b|\\b+tmpfile+\\b|\\b+tmpname+\\b|\\b+read+\\b|\\b+write+\\b|\\b+clock+\\b|\\b+date+\\b|\\b+difftime+\\b|\\b+execute+\\b|\\b+exit+\\b|\\b+getenv+\\b|\\b+setlocale+\\b|\\b+time+\\b|\\b+coroutine.create+\\b|\\b+coroutine.resume+\\b|\\b+coroutine.status+\\b|\\b+coroutine.wrap+\\b|\\b+coroutine.yield+\\b|\\b+io.close+\\b|\\b+io.flush+\\b|\\b+io.input+\\b|\\b+io.lines+\\b|\\b+io.open+\\b|\\b+io.output+\\b|\\b+io.read+\\b|\\b+io.tmpfile+\\b|\\b+io.type+\\b|\\b+io.write+\\b|\\b+io.stdin+\\b|\\b+io.stdout+\\b|\\b+io.stderr+\\b|\\b+os.clock+\\b|\\b+os.date+\\b|\\b+os.difftime+\\b|\\b+os.execute+\\b|\\b+os.exit+\\b|\\b+os.getenv+\\b|\\b+os.remove+\\b|\\b+os.rename+\\b|\\b+os.setlocale+\\b|\\b+os.time+\\b|\\b+os.tmpname+\\bopenfile+\\b|\\b+closefile+\\b|\\b+readfrom+\\b|\\b+writeto+\\b|\\b+appendto+\\b|\\b+remove+\\b|\\b+rename+\\b|\\b+flush+\\b|\\b+seek+\\b|\\b+tmpfile+\\b|\\b+tmpname+\\b|\\b+read+\\b|\\b+write+\\b|\\b+clock+\\b|\\b+date+\\b|\\b+difftime+\\b|\\b+execute+\\b|\\b+exit+\\b|\\b+getenv+\\b|\\b+setlocale+\\b|\\b+time+\\b|\\b+coroutine.create+\\b|\\b+coroutine.resume+\\b|\\b+coroutine.status+\\b|\\b+coroutine.wrap+\\b|\\b+coroutine.yield+\\b|\\b+io.close+\\b|\\b+io.flush+\\b|\\b+io.input+\\b|\\b+io.lines+\\b|\\b+io.open+\\b|\\b+io.output+\\b|\\b+io.read+\\b|\\b+io.tmpfile+\\b|\\b+io.type+\\b|\\b+io.write+\\b|\\b+io.stdin+\\b|\\b+io.stdout+\\b|\\b+io.stderr+\\b|\\b+os.clock+\\b|\\b+os.date+\\b|\\b+os.difftime+\\b|\\b+os.execute+\\b|\\b+os.exit+\\b|\\b+os.getenv+\\b|\\b+os.remove+\\b|\\b+os.rename+\\b|\\b+os.setlocale+\\b|\\b+os.time+\\b|\\b+os.tmpname+\\b)");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    // attribute content format
    attributeContentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("(\"[^\"]*\"|'[^']*')");
    rule.format = attributeContentFormat;
    highlightingRules.append(rule);

    commentFormat.setForeground(Qt::lightGray);
    commentFormat.setFontItalic(true);

    commentExpression = QRegExp("--");
    commentStartExpression = QRegExp("--\\[\\[");
    commentEndExpression = QRegExp("\\]\\]");
}

void LuaSyntaxHighlighter::Dohighlight(const QString &text)
{
    highlightBlock(text);
}

void LuaSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
    setCurrentBlockState(0);

    int bFind = false;
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        bFind = true;
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = text.indexOf(commentStartExpression,
            startIndex + commentLength);
    }
    if (bFind)
        return;

    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentExpression);

    if (startIndex >= 0) {
        int commentLength;
        commentLength = text.length() - startIndex;
        setFormat(startIndex, commentLength, commentFormat);
        setCurrentBlockState(0);
    }
}
