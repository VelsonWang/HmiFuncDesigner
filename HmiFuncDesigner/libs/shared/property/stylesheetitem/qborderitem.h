#ifndef QBORDERITEM_H
#define QBORDERITEM_H

#include "qabstractstylesheetitem.h"
#include "qgradientitem.h"

class QGradientItem;

class SHAREDLIB_EXPORT QBorderItem : public QAbstractStylesheetItem
{
    Q_OBJECT
public:
    QBorderItem(QAbstractStylesheetItem *parent=NULL);

    void setValue(const QVariant &value);

    QString makeStylesheet();
protected:
    void subValueChanged();
protected:
    QGradientItem           *m_left_color;
    QGradientItem           *m_top_color;
    QGradientItem           *m_right_color;
    QGradientItem           *m_bottom_color;

    QAbstractStylesheetItem *m_left_width;
    QAbstractStylesheetItem *m_right_width;
    QAbstractStylesheetItem *m_bottom_width;
    QAbstractStylesheetItem *m_top_width;

    QAbstractStylesheetItem *m_left_style;
    QAbstractStylesheetItem *m_right_style;
    QAbstractStylesheetItem *m_bottom_style;
    QAbstractStylesheetItem *m_top_style;


    QAbstractStylesheetItem *m_color_same;
    QAbstractStylesheetItem *m_style_same;
    QAbstractStylesheetItem *m_width_same;

    QAbstractStylesheetItem *m_image;
};

#endif // QBORDERITEM_H
