#ifndef GETRTDBWRITEVALUEDIALOG_H
#define GETRTDBWRITEVALUEDIALOG_H

#include "RtdbTag.h"
#include <QDialog>

namespace Ui {
class GetRtdbWriteValueDialog;
}

class GetRtdbWriteValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetRtdbWriteValueDialog(TagItem *tag, QWidget *parent = Q_NULLPTR);
    ~GetRtdbWriteValueDialog();
    QString getWriteValue();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    Ui::GetRtdbWriteValueDialog *ui;
    QString m_szWriteValue;
};

#endif // GETRTDBWRITEVALUEDIALOG_H
