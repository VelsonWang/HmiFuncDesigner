#ifndef QPROGRESSBARHOST_H
#define QPROGRESSBARHOST_H

#include "qwidgethost.h"

class QProgressBarHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QProgressBarHost(QAbstractHost *parent = 0);

    static QString get_show_name();
    static QString get_show_icon();
    static QString get_show_group();
public slots:
    void    setValue(int value);
    int     value();

    void    setAlignment(int alignment);
    int     alignment();

    void    setOrientation(int orientation);
    int     orientation();

    void    setFormat(const QString & format);
    QString format();

    void    setInvertedAppearance(bool invertedAppearance);
    bool    invertedAppearance();

    void    setTextVisible(bool textVisible);
    bool    textVisible();
protected slots:
    void    valueChanged(int value);
protected:
    void init_property();
protected:
    void    create_object();
};


#endif // QPROGRESSBARHOST_H
