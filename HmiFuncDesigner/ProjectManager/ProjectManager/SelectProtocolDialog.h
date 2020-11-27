#ifndef SELECTPROTOCOLDIALOG_H
#define SELECTPROTOCOLDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class SelectProtocolDialog;
}

class SelectProtocolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectProtocolDialog(QWidget *parent = Q_NULLPTR);
    ~SelectProtocolDialog();

    // 设置支持的协议列表
    void SetProtocolList(QStringList list);
    // 获取选中的协议名
    QString GetProtocolName();

private slots:
    void on_btnOk_clicked();
    void on_btnExit_clicked();
    void on_ProtocolListWidget_currentTextChanged(const QString &currentText);

private:
    Ui::SelectProtocolDialog *ui;
    QString m_strProtocolName;
};

#endif // SELECTPROTOCOLDIALOG_H
