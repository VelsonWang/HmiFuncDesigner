#include "qabstractfunction.h"

QAbstractFunction::QAbstractFunction()
{

}

QString QAbstractFunction::group()
{
    return QString();
}

QString QAbstractFunction::name()
{
    return QString();
}

QString QAbstractFunction::showName()
{
    return QString();
}

QStringList QAbstractFunction::args()
{
    return QStringList();
}

QStringList QAbstractFunction::showArgs()
{
    return QStringList();
}

QString QAbstractFunction::description()
{
    return QString();
}

QString QAbstractFunction::toString()
{
    return QString();
}

QString QAbstractFunction::toShowString()
{
    return QString();
}

bool QAbstractFunction::fromString(const QString func)
{
    Q_UNUSED(func)
    return false;
}

