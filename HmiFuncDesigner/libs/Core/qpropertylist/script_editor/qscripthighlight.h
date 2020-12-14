#ifndef QSCRIPTHIGHLIGHT_H
#define QSCRIPTHIGHLIGHT_H

#include <QSyntaxHighlighter>
#include <QTimer>

class QScriptHighLight : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    QScriptHighLight(QTextDocument *document =0 );
    ~QScriptHighLight();
protected:
    void highlightBlock(const QString &text);

private:
    void highlightWord(int currentPos ,const QString &buffer);
    enum ScriptFormats
    {
        ScriptTextFormat,ScriptNumberFormat,
        ScriptStringFormat,
        ScriptKeywordFormat,ScriptPreprocessorFormat,
        ScriptLabelFormat,ScriptCommentFormat,
        NumScriptFormats
    };
    QTextCharFormat m_formats[NumScriptFormats];
};

#endif // QSCRIPTHIGHLIGHT_H
