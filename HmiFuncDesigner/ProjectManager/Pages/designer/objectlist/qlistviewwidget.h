#ifndef QLISTVIEWWIDGET_H
#define QLISTVIEWWIDGET_H

#include <QWidget>

namespace Ui {
class QListViewWidget;
}

class QToolBarButton;

class QListViewWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QListViewWidget(QWidget *parent = 0);
    ~QListViewWidget();
    void set_text(const QString &text);
    void set_icon(const QString &icon);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void remove();

private:
    Ui::QListViewWidget *ui;
    QToolBarButton *m_removeButton;
};

#endif // QLISTVIEWWIDGET_H
