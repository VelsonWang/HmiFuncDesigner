#ifndef QRESOURCESHOWWIDGET_H
#define QRESOURCESHOWWIDGET_H

#include <QWidget>

namespace Ui {
class QResourceShowWidget;
}

class QResourceShowWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QResourceShowWidget(QWidget *parent = 0);
    ~QResourceShowWidget();
    
    void set_image(const QByteArray &data);

signals:
    void ok();
    void cancel();

private slots:
    void on_okbtn_clicked();

    void on_cancelbtn_clicked();

private:
    Ui::QResourceShowWidget *ui;
};

#endif // QRESOURCESHOWWIDGET_H
