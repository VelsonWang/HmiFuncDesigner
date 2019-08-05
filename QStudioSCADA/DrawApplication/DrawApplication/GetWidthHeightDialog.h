#ifndef GETWIDTHHEIGHTDIALOG_H
#define GETWIDTHHEIGHTDIALOG_H

#include <QDialog>

namespace Ui {
class GetWidthHeightDialog;
}

class GetWidthHeightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetWidthHeightDialog(QWidget *parent = nullptr);
    ~GetWidthHeightDialog();

    void setWidth(int iWidth);
    int getWidth();

    void setHeight(int iHeight);
    int getHeight();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    Ui::GetWidthHeightDialog *ui;
    int iWidth_;
    int iHeight_;
};

#endif // GETWIDTHHEIGHTDIALOG_H
