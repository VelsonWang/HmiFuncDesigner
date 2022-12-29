#ifndef QSHOWWIDGET_H
#define QSHOWWIDGET_H

#include <QWidget>

namespace Ui
{
class QShowWidget;
}

class QAbstractHost;

class QShowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QShowWidget(QWidget *parent = 0);
    ~QShowWidget();

    void setItemSheet(const QString &sheet);
    void setHost(QAbstractHost* host);

protected:
    bool eventFilter(QObject *, QEvent *);

private slots:
    void on_okbtn_clicked();
    void on_cancelbtn_clicked();
    void on_clear_clicked();
    void on_clearall_clicked();

signals:
    void ok();
    void cancel();
    void clear();
    void clearall();

private:
    Ui::QShowWidget *ui;

protected:
    QAbstractHost* m_host;
};

#endif // QSHOWWIDGET_H
