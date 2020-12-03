#ifndef QFORMWIDGETVIEW_H
#define QFORMWIDGETVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QUndoStack>

class QFormListWidget;

class StyledBar;
class QFormHost;
class QAbstractHost;
class QAbstractProperty;

class QFormWidgetView : public QWidget
{
    Q_OBJECT
public:
    explicit QFormWidgetView(QWidget *parent = Q_NULLPTR);
    ~QFormWidgetView();

public slots:
    void setSelectObj(QAbstractHost* host);

signals:
    void selectObj(QAbstractHost* host);

protected:
    QFormListWidget *m_formWidget;

};

#endif // QFORMWIDGETVIEW_H
