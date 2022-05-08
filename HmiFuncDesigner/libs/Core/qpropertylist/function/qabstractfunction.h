#ifndef QABSTRACTFUNCTION_H
#define QABSTRACTFUNCTION_H

#include <QObject>
#include <QString>
#include <QStringList>

class QAbstractFunction
{
public:
    QAbstractFunction();

public:
    virtual QString group(); //功能所属的组名：如系统操作、变量操作、控件操作等
    virtual QString name(); //函数名称
    virtual QString showName(); //编辑器显示的函数名称
    virtual QStringList args();
    virtual QStringList showArgs(); //编辑器显示的函数参数
    virtual QString description(); //函数描述
    virtual QString toString();
    virtual QString toShowString();
    virtual bool fromString(const QString func);
};

#endif // QABSTRACTFUNCTION_H
