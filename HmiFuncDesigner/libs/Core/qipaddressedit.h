#ifndef QIPADDRESSEDIT_H
#define QIPADDRESSEDIT_H

#include "corelibglobal.h"
#include <QWidget>

namespace Ui
{
class QIPAddressEdit;
}

class CORELIB_EXPORT QIPAddressEdit : public QWidget
{
    Q_OBJECT

public:
    explicit QIPAddressEdit(QWidget *parent = NULL);
    ~QIPAddressEdit();

    void setIPString(const QString &ip);
    QString getIPString() const;

private:
    Ui::QIPAddressEdit *ui;
};

#endif // QIPADDRESSEDIT_H
