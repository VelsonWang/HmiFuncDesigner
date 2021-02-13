#ifndef QITEMLISTWIDGET_H
#define QITEMLISTWIDGET_H

#include <QWidget>

namespace Ui {
class QItemListWidget;
}

class QToolBarButton;

class QItemListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QItemListWidget(QWidget *parent = 0);
    ~QItemListWidget();
    
    void set_text(const QString &text);
    void set_icon(const QString &icon);

private:
    Ui::QItemListWidget *ui;

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void remove();
    void changed();

protected:
    QToolBarButton *m_changed_button;
    QToolBarButton *m_remove_button;
};

#endif // QITEMLISTWIDGET_H
