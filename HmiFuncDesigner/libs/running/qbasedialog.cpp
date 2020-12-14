#include "qbasedialog.h"

QBaseDialog::QBaseDialog(QWidget *parent) :
    QDialog(parent)
{
}

void QBaseDialog::set_widget(QWidget *wid)
{
    m_widget=wid;
    this->resize(wid->size());
    wid->setParent(this);
    wid->move(0,0);
}
