#ifndef QITEMLISTWIDGET_H
#define QITEMLISTWIDGET_H

#include <QWidget>

namespace Ui
{
class QItemListWidget;
}

class QToolBarButton;

class QItemListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QItemListWidget(QWidget *parent = 0);
    ~QItemListWidget();

    void setText(const QString &text);
    void setIcon(const QString &icon);

private:
    Ui::QItemListWidget *ui;

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void remove();
    void changed();

protected:
    QToolBarButton *m_changedButton;
    QToolBarButton *m_removeButton;
};

#endif // QITEMLISTWIDGET_H
