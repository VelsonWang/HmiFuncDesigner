#include "qform.h"

QForm::QForm(QWidget *parent) : QWidget(parent)
{

}

QStringList QForm::getOpenFuncs() const
{
    return openFuncs;
}

void QForm::setOpenFuncs(const QStringList &value)
{
    if(value != openFuncs) {
        openFuncs = value;
        this->update();
    }
}

QStringList QForm::getCloseFuncs() const
{
    return closeFuncs;
}

void QForm::setCloseFuncs(const QStringList &value)
{
    if(value != closeFuncs) {
        closeFuncs = value;
        this->update();
    }
}
