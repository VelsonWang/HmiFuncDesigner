#ifndef QRENAMEDIALOG_H
#define QRENAMEDIALOG_H

#include "corelibglobal.h"

#include <QDialog>

namespace Ui {
class QRenameDialog;
}

class QNameEdit;

class CORELIB_EXPORT QRenameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QRenameDialog(QWidget *parent = 0);
    ~QRenameDialog();

    void set_name_list(const QStringList &list);
    void    set_old_name(const QString &name);
    QString get_new_name();
    int     get_ret();
private slots:
    void on_okbtn_clicked();

    void on_cancelbtn_clicked();

private:
    Ui::QRenameDialog *ui;

    QNameEdit    *m_nameEdit;

    int                 m_ret;
};

#endif // QRENAMEDIALOG_H
