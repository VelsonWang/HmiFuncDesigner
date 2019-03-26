
#ifndef QTSINGLEAPPLICATION_H
#define QTSINGLEAPPLICATION_H

#include <QApplication>

class QtLocalPeer;

class QtSingleApplication : public QApplication
{
    Q_OBJECT

public:
    QtSingleApplication(int &argc,
                        char **argv,
                        bool GUIenabled = true);
    QtSingleApplication(const QString &id,
                        int &argc,
                        char **argv);

    bool isRunning();
    QString id() const;

    void setActivationWindow(QWidget* aw, bool activateOnMessage = true);
    QWidget* activationWindow() const;

    // Obsolete:
    void initialize(bool dummy = true)
        { isRunning(); Q_UNUSED(dummy) }

public slots:
    bool sendMessage(const QString &message, int timeout = 5000);
    void activateWindow();

signals:
    void messageReceived(const QString &message);

private:
    void sysInit(const QString &appId = QString());
    QtLocalPeer *peer;
    QWidget *actWin;
};

#endif // QTSINGLEAPPLICATION_H
