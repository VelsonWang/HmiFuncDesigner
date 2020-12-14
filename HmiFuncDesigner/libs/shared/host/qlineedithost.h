#ifndef QLINEEDITHOST_H
#define QLINEEDITHOST_H

#include "qwidgethost.h"

class QLineEditHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QLineEditHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setText(const QString text);
    QString text();

    void    setAlignment(int alignment);
    int     alignment();

    void    setFrame(bool frame);
    bool     frame();

    void    setReadOnly(bool readOnly);
    bool     readOnly();
protected slots:
    void    textChanged(const QString &text);
    void    editFinish();
protected:
    void    init_property();
protected:
    void    create_object();
};


#endif // QLINEEDITHOST_H
