
#include "qtsinglecoreapplication.h"
#include "qtlocalpeer.h"

QtSingleCoreApplication::QtSingleCoreApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    peer = new QtLocalPeer(this);
    connect(peer, SIGNAL(messageReceived(const QString&)),
            SIGNAL(messageReceived(const QString&)));
}

QtSingleCoreApplication::QtSingleCoreApplication(const QString &appId,
                                                 int &argc,
                                                 char **argv)
    : QCoreApplication(argc, argv)
{
    peer = new QtLocalPeer(this, appId);
    connect(peer, SIGNAL(messageReceived(const QString&)),
            SIGNAL(messageReceived(const QString&)));
}


/*
Returns true if another instance of this application is running; otherwise false.
This function does not find instances of this application that are being
run by a different user (on Windows: that are running in another session).
*/

bool QtSingleCoreApplication::isRunning()
{
    return peer->isClient();
}

bool QtSingleCoreApplication::sendMessage(const QString &message,
                                          int timeout)
{
    return peer->sendMessage(message, timeout);
}

QString QtSingleCoreApplication::id() const
{
    return peer->applicationId();
}


