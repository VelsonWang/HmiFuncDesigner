#ifndef QRUNNINGMANAGER_H
#define QRUNNINGMANAGER_H

#include "sharedlibglobal.h"
#include <QObject>
#include <QMainWindow>
#include <QTimer>

class QProjectCore;
class QBaseDialogWidget;

class SHAREDLIB_EXPORT QRunningManager : public QObject
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
    void onShowDialog(QWidget* widget);

protected:
    QProjectCore *m_pProjCoreObj;
    QMainWindow *m_pMainWindowObj;
    QBaseDialogWidget *m_pDlgBaseWidgetObj;
    QWidget *m_pLastWidgetObj;

private:
    QTimer m_timer;
};

#endif // QRUNNINGMANAGER_H
