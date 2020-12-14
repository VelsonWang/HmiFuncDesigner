#ifndef STYLEDBAR_H
#define STYLEDBAR_H

#include "corelibglobal.h"

#include <QWidget>

class CORELIB_EXPORT StyledBar : public QWidget
{
    Q_OBJECT
public:
    StyledBar(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);

};
#endif // STYLEDBAR_H
