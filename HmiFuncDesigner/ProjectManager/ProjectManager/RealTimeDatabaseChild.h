#ifndef REALTIMEDATABASECHILD_H
#define REALTIMEDATABASECHILD_H

#include "ChildInterface.h"
#include "ListViewEx.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QContextMenuEvent>
#include <QProcess>
#include <QScopedPointer>


class RealTimeDatabaseChild : public QWidget, public ChildInterface
{
    Q_OBJECT
    Q_INTERFACES(ChildInterface)
public:
    explicit RealTimeDatabaseChild(QWidget *parent = Q_NULLPTR);
    ~RealTimeDatabaseChild();

public:
    TypeDocument typeDocument() const {return td_RealTimeDatabase;}

    void buildUserInterface(QMainWindow* pMainWin);
    void removeUserInterface(QMainWindow* pMainWin);

    bool open();
    bool save();
    bool saveAs();

    QString userFriendlyCurrentFile();
    QString currentFile() const;
    QString wndTitle() const;


private:
    void ListViewInitUi();

private slots:
    void onSlotListViewProjectDoubleClicked(const QModelIndex &index);

private:
    ListViewEx *m_pListViewObj = Q_NULLPTR;
    QStandardItemModel *m_pListViewModelObj = Q_NULLPTR;
    QMainWindow* m_pMainWinObj = Q_NULLPTR;
    QString m_szProjPath;
};

#endif // REALTIMEDATABASECHILD_H
