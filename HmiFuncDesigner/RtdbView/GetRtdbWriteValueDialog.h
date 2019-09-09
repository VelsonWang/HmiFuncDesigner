#ifndef GETRTDBWRITEVALUEDIALOG_H
#define GETRTDBWRITEVALUEDIALOG_H

#include "Tag.h"
#include <QDialog>

namespace Ui {
class GetRtdbWriteValueDialog;
}

class GetRtdbWriteValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetRtdbWriteValueDialog(TagItem *tag, QWidget *parent = nullptr);
    ~GetRtdbWriteValueDialog();
    QString GetWriteValue();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

private:
    Ui::GetRtdbWriteValueDialog *ui;
    QString m_sWriteValue;
};

#endif // GETRTDBWRITEVALUEDIALOG_H
