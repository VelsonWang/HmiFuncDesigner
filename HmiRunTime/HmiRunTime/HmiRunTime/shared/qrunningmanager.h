#ifndef QRUNNINGMANAGER_H
#define QRUNNINGMANAGER_H

#include "sharedlibglobal.h"
#include "qblinkwidgettimer.h"
#include <QObject>
#include "mainwindow.h"
#include <QTimer>
#include <QStack>

class QProjectCore;
class QBaseDialogWidget;

class SHAREDLIB_EXPORT QRunningManager : public QObject
{
    Q_OBJECT
public:
    static QRunningManager *instance() {
        static QRunningManager obj;
        return &obj;
    }

    void showGraphPage(const QString &pageId);
    void showLastGraphPage();
    void showControlElement(const QString &eleId);
    void hideControlElement(const QString &eleId);
    void enableControlElement(const QString &eleId);
    void disableControlElement(const QString &eleId);
    void moveControlElement(const QString &eleId, int x, int y);
    void blinkControlElement(const QString &eleId);
    void stopBlinkControlElement(const QString &eleId);

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

private:
    explicit QRunningManager(QObject *parent = 0);
    ~QRunningManager();

protected:
    QProjectCore *m_pProjCoreObj;
    MainWindow *m_pMainWindowObj;
    QBaseDialogWidget *m_pDlgBaseWidgetObj;
    QWidget *m_pLastWidgetObj;

private:
    QTimer m_timer;
    QStack<QWidget *> m_stackShowWidget;
    QBlinkWidgetTimer m_blinkTimer;
};

#endif // QRUNNINGMANAGER_H
