#ifndef QWIDGETBOXVIEW_H
#define QWIDGETBOXVIEW_H

#include <QWidget>

class StyledBar;
class WidgetBoxTreeWidget;

class QWidgetBoxView : public QWidget
{
    Q_OBJECT
public:
    explicit QWidgetBoxView(QWidget *parent = 0);

signals:

public slots:

protected:
    WidgetBoxTreeWidget *m_widgetBox;
    StyledBar           *m_styledBar;
};


#endif // QWIDGETBOXVIEW_H
