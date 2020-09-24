#ifndef CHILDINTERFACE_H
#define CHILDINTERFACE_H

#include <QtPlugin>

class QMainWindow;
class QString;

class ChildInterface
{
public:
    virtual ~ChildInterface() {}

    enum TypeDocument
    {
        td_None,
        td_SystemParameters,

    };


public:
    virtual TypeDocument typeDocument() const = 0;
    virtual void buildUserInterface(QMainWindow* pMainWin) = 0;
    virtual void removeUserInterface(QMainWindow* pMainWin) = 0;

    virtual bool open() = 0;
    virtual bool save() = 0;
    virtual bool saveAs() = 0;

    virtual QString userFriendlyCurrentFile() = 0;
    virtual QString currentFile() const = 0;
    virtual QString wndTitle() const = 0;

    virtual bool hasSelection() const = 0;

public:
    QString m_szProjectName;
    QString m_szItemName;
};
Q_DECLARE_INTERFACE(ChildInterface, "HmiFuncDesigner.ProjectManager.ChildInterface")




#define DEL_OBJ(obj_ptr) do\
    if(obj_ptr != Q_NULLPTR) {\
    delete obj_ptr;\
    obj_ptr = Q_NULLPTR;\
} while(0)

#endif // INTERFACECHILD_H
