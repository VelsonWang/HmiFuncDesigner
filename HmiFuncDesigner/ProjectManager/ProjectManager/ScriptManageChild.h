#ifndef ScriptManageChild_H
#define ScriptManageChild_H

#include "ChildInterface.h"
#include "../../Public/Public.h"
#include "Script.h"
#include <QWidget>
#include <QListWidget>
#include <QList>
#include <QVBoxLayout>
#include <QStringList>
#include <QJsonObject>

class ScriptManageChild : public QWidget, public ChildInterface
{
    Q_OBJECT
    Q_INTERFACES(ChildInterface)
public:
    explicit ScriptManageChild(QWidget *parent = Q_NULLPTR);
    ~ScriptManageChild();
    void init();


public:
    void buildUserInterface(QMainWindow* pMainWin);
    void removeUserInterface(QMainWindow* pMainWin);
    QString userFriendlyCurrentFile();
    QString currentFile() const;
    QString wndTitle() const;

private:
    void updateUI();

public slots:
    void ListWidgetClicked(QListWidgetItem *item);
    void NewScript();
    void ModifyScript();
    void DeleteScript();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private:
    QListWidget *m_pListWidgetObj;
    QVBoxLayout *m_pVLayoutObj;
};


#endif // ScriptManageChild_H
