#ifndef RTDBCONNECTDIALOG_H
#define RTDBCONNECTDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QCloseEvent>


namespace Ui {
class RtdbConnectDialog;
}

class RtdbConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RtdbConnectDialog(QWidget *parent = nullptr);
    ~RtdbConnectDialog();
    QString GetIPAddress();
    int GetOption();
    void SetConnectStatus(QString s);

private:
    void WriteConfig();

private slots:
    void on_editAddress_textChanged(const QString &arg1);
    void on_btnConnect_clicked();
    void on_btnDisConnect_clicked();
    void on_btnExit_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::RtdbConnectDialog *ui;
    int mOpt;
};

#endif // RTDBCONNECTDIALOG_H
