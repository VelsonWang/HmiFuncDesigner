#ifndef QFILELISTITEMWIDGET_H
#define QFILELISTITEMWIDGET_H

#include <QWidget>

namespace Ui
{
class QFileListItemWidget;
}

class QToolBarButton;
class QTreeWidgetItem;
class QFileListView;

class QFileListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QFileListItemWidget(QFileListView* view, QTreeWidgetItem* item, QWidget *parent = 0);
    ~QFileListItemWidget();

    void setText(const QString &text);
    void setIcon(const QString &icon);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void remove();

private:
    Ui::QFileListItemWidget *ui;
    QToolBarButton      *m_removeButton;
    QTreeWidgetItem     *m_item;
    QFileListView       *m_view;
};

#endif // QFILELISTITEMWIDGET_H
