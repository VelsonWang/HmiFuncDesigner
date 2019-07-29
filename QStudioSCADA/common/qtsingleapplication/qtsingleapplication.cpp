
#include "qtsingleapplication.h"
#include "qtlocalpeer.h"
#include <QWidget>
#include <QTimer>

void QtSingleApplication::sysInit(const QString &appId)
{
    actWin = 0;
    peer = new QtLocalPeer(this, appId);
    connect(peer, SIGNAL(messageReceived(const QString&)),
            SIGNAL(messageReceived(const QString&)));
}

QtSingleApplication::QtSingleApplication(int &argc,
                                         char **argv,
                                         bool GUIenabled)
    : QApplication(argc, argv, GUIenabled)
{
    sysInit();
}

QtSingleApplication::QtSingleApplication(const QString &appId,
                                         int &argc,
                                         char **argv)
    : QApplication(argc, argv)
{
    sysInit(appId);
}

/*
Returns true if another instance of this application is running; otherwise false.
This function does not find instances of this application that are being
run by a different user (on Windows: that are running in another session).
*/
bool QtSingleApplication::isRunning()
{
    return peer->isClient();
}

bool QtSingleApplication::sendMessage(const QString &message,
                                      int timeout)
{
    return peer->sendMessage(message, timeout);
}

QString QtSingleApplication::id() const
{
    return peer->applicationId();
}

void QtSingleApplication::setActivationWindow(QWidget* aw,
                                              bool activateOnMessage)
{
    actWin = aw;
    if (activateOnMessage)
        connect(peer, SIGNAL(messageReceived(const QString&)),
                this, SLOT(activateWindow()));
    else
        disconnect(peer, SIGNAL(messageReceived(const QString&)),
                   this, SLOT(activateWindow()));
}

QWidget* QtSingleApplication::activationWindow() const
{
    return actWin;
}

void QtSingleApplication::activateWindow()
{
    if (actWin) {
        actWin->setWindowState(actWin->windowState() & ~Qt::WindowMinimized);
        actWin->raise();
        actWin->activateWindow();
    }
}
