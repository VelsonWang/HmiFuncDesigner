#ifndef QIPADDRESSEDIT_H
#define QIPADDRESSEDIT_H

#include <QWidget>

namespace Ui {
class QIPAddressEdit;
}

class QIPAddressEdit : public QWidget
{
    Q_OBJECT

public:
    explicit QIPAddressEdit(QWidget *parent = 0);
    ~QIPAddressEdit();

    void setIPString(const QString &ip);
    QString getIPString() const;

private:
    Ui::QIPAddressEdit *ui;
};

#endif // QIPADDRESSEDIT_H
