﻿#ifndef MDICHILDWINDOW_H
#define MDICHILDWINDOW_H

#include <QWidget>
#include <QList>
#include <QTableView>
#include <QVBoxLayout>
#include <QContextMenuEvent>
#include "RtdbTag.h"

class MdiChildWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MdiChildWindow(QList<TagItem *> tagList,
                            QString tittle = QString(),
                            QWidget *parent = Q_NULLPTR);
    ~MdiChildWindow();
    void SetTagLogicValueAndStatus(QString id,
                                   QString logicValue,
                                   QString status);

protected:
    void contextMenuEvent(QContextMenuEvent * event);

signals:
    void writeRtdbTag(QString cmdline);


public slots:
    void writeRtdbTagAct();

private:
    QList<TagItem *> mTagList;
    TagTableModel *pTagTableModel;
    QTableView *pTableView;
    QVBoxLayout *verticalLayout;
};

#endif // MDICHILDWINDOW_H
