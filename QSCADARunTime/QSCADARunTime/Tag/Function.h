#ifndef FUNCTION_H
#define FUNCTION_H

#include "../Event/Event.h"
#include <QList>
#include <QDebug>


typedef struct
{
    QString arg;
    QString type;
} TArgItem;

class FuncObjectItem
{
public:
    TFuncObjectItem()
    {
        name = "";
        event = "";
        argList.clear();
    }

    void ShowInfo()
    {
        qDebug() << "\n[";
        qDebug() << name;
        for(int i=0; i<argList.count();i++)
        {
            TArgItem it = argList.at(i);
            qDebug() << it.arg << " " << it.type;
        }
        qDebug() << event;
        qDebug() << "]\n";
    }

public:
    QString name;
    QList<TArgItem> argList;
    QString event;
};

typedef FuncObjectItem TFuncObjectItem;
typedef FuncObjectItem* PFuncObjectItem;

//////////////////////////////////////////////////////////////////


class Function
{
public:
    Function();
    ~Function();

public:
    void LoadFuncObjects(const QString projectConverter);
    bool ExecFunctions(RuntimeEvent e);

private:
    bool SetValueToVariable(RuntimeEvent e, QString var1, QString value);
    bool SetVariableToVariable(RuntimeEvent e, QString var1, QString var2);
    bool GetVariableValue(RuntimeEvent e, QString var1);

private:
    QList<PFuncObjectItem> m_listFuncObject;
};

#endif // FUNCTION_H
