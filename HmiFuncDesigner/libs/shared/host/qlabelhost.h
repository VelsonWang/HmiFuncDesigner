#ifndef QLABELHOST_H
#define QLABELHOST_H

#include "qframehost.h"

class QLabelHost : public QFrameHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QLabelHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setText(const QString & text);
    QString text();

    void    setWordWrap(bool wordWrap);
    bool    wordWrap();

    void    setAlignment(int alignment);
    int     alignment();
protected:
    void    init_property();
protected:
    void    create_object();
};

#endif // QLABELHOST_H
