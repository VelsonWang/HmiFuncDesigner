#ifndef TAGTMPEDITDIALOG_H
#define TAGTMPEDITDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class TagTmpEditDialog;
}

class TagTmpEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagTmpEditDialog(QWidget *parent = Q_NULLPTR);
    ~TagTmpEditDialog();

    int createTagNum();
    void setCreateTagNum(int num);

    QString tagName();
    void setTagName(const QString &name);

    QString tagDescription();
    void setTagDescription(const QString &desc);

    QString tagDataType();
    void setTagDataType(const QString &dataType);

    QString tagInitValue();
    void setTagInitValue(const QString &initValue);

    QString tagMinValue();
    void setTagMinValue(const QString &minValue);

    QString tagMaxValue();
    void setTagMaxValue(const QString &maxValue);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    Ui::TagTmpEditDialog *ui;
    int m_iCreateTagNum = 1;
    QString m_szTagName = "";
    QString m_szTagDescription = "";
    QString m_szTagDataType = "";
    QString m_szTagInitValue = "";
    QString m_szTagMinValue = "";
    QString m_szTagMaxValue = "";
};

#endif // TAGTMPEDITDIALOG_H



