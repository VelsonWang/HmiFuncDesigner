#ifndef QRUNNINGMANAGER_H
#define QRUNNINGMANAGER_H

#include "runninglibglobal.h"
#include <QObject>
#include <QMainWindow>

class QAbstractHost;
class QProjectCore;
class QBaseDialogWidget;

class RUNNINGLIB_EXPORT QRunningManager : public QObject
{
    Q_OBJECT
public:
    explicit QRunningManager(QObject *parent = 0);
    ~QRunningManager();

    bool load(QString proj);
    void release();
    void start();
    void stop();

    QProjectCore *projCore()
    {
        return m_pProjCoreObj;
    }

protected:
    bool eventFilter(QObject *, QEvent *);

signals:
    void notifyStart();
    void notifyStop();

public slots:
    void onShowWidget(QWidget* widget);
    void onShowDialog(QAbstractHost* host);

protected:
    QProjectCore *m_pProjCoreObj;
    QMainWindow *m_pMainWindowObj;
    QBaseDialogWidget *m_pDlgBaseWidgetObj;
    QWidget *m_pLastWidgetObj;
};

#endif // QRUNNINGMANAGER_H
