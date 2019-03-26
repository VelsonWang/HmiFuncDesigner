
#ifndef QTSINGLECOREAPPLICATION_H
#define QTSINGLECOREAPPLICATION_H

#include <QCoreApplication>

class QtLocalPeer;

class QtSingleCoreApplication : public QCoreApplication
{
    Q_OBJECT

public:
    QtSingleCoreApplication(int &argc, char **argv);
    QtSingleCoreApplication(const QString &id, int &argc, char **argv);

    bool isRunning();
    QString id() const;

public slots:
    bool sendMessage(const QString &message, int timeout = 5000);


signals:
    void messageReceived(const QString &message);


private:
    QtLocalPeer* peer;
};

#endif // QTSINGLECOREAPPLICATION_H
