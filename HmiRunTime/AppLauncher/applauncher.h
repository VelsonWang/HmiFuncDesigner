#ifndef APP_LAUNCHER_H
#define APP_LAUNCHER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QProcess>
#include <QString>
#include <QTimer>

class AppLauncher : public QObject
{
    Q_OBJECT
public:
    AppLauncher(QObject *parent = Q_NULLPTR);
    ~AppLauncher();

    bool execute(const QString &szAppName, const QStringList &args);

    bool isRunning(const QString &szAppName);
    const QStringList &running() const { return runningList; }

    qint64 pidForName( const QString & );

public slots:
    void criticalKill(const QString &szAppName);
    void onMessageReceived(const QString &szMsg);

private slots:
    void timeToCheckProcess();
    void processTerminated(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void launched(qint64 pid, const QString &szAppName);
    void terminated(qint64 pid, const QString &szAppName);

private:
    void appLaunched(QProcess *proc, const QString &);
    void kill(qint64 pid);

private:
    struct RunningApp {
        QString name;
        QProcess *proc;
    };

    QMap<qint64, RunningApp> runningApps;
    QStringList runningList;
    QTimer m_tmrCheckProcess;
};

#endif

