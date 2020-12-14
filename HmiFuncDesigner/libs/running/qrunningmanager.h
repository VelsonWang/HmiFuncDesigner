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
    
    bool    load(const QString &path);

    void    release();

    void    start();
    void    stop();
protected:
    bool    eventFilter(QObject *, QEvent *);
signals:
    void    start_signal();
    void    stop_signal();
public slots:
    void    show_widget(QWidget* widget);
    void    show_dialog(QAbstractHost* host);
protected:
    QProjectCore        *m_project;
    QMainWindow         *m_main_window;
    QBaseDialogWidget   *m_dialog_base_widget;
    QWidget*             m_last_widget;
};

#endif // QRUNNINGMANAGER_H
