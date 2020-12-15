#ifndef QLABELHOST_H
#define QLABELHOST_H

#include "qframehost.h"

class QLabelHost : public QFrameHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QLabelHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();
public slots:
    void    setText(const QString & text);
    QString text();

    void    setWordWrap(bool wordWrap);
    bool    wordWrap();

    void    setAlignment(int alignment);
    int     alignment();
protected:
    void    initProperty();
protected:
    void    createObject();
};

#endif // QLABELHOST_H
