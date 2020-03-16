#include <QApplication>
#include <QStorageInfo>
#include "tcpserver.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "edncrypt.h"
#include "../HmiRunTime/Public/Global.h"

#ifdef USE_FTP_SERVICE
#include "ftpserver.h"
#endif

#include <QDebug>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef USE_FTP_SERVICE
    QString configPath = "FileTransferTool.ini";
    //////////////////////////////////////////////////////////////////////////////
    ///  启动ftp服务
    QString strInput = ConfigUtils::getCfgStr(configPath, "FtpServer", "UserName", "admin");
    const QString &userName = EDncrypt::Dncrypt(strInput, AES, KEY_CODE);
    strInput = ConfigUtils::getCfgStr(configPath, "FtpServer", "PassWord", "admin");
    const QString &password = EDncrypt::Dncrypt(strInput, AES, KEY_CODE);
    QStorageInfo storageRoot = QStorageInfo::root();
    const QString &rootPath = "/"; //storageRoot.rootPath();
    quint32 port = ConfigUtils::getCfgInt(configPath, "FtpServer", "Port", 60001);

    FtpServer ftpServer(&app, rootPath, port, userName, password, false, false);
    if (ftpServer.isListening()) {
        QString ftpServerInfo = QString("\nFtpServer Listening at %1:%2").arg(FtpServer::lanIp()).arg(port);
        ftpServerInfo += QString("\nFtpServer User: %1").arg(userName);
        ftpServerInfo += QString("\nFtpServer Password: %1").arg(password);
        qDebug() << ftpServerInfo;
    } else {
        qDebug() << "Failed to start FtpServer.";
    }
#endif

    TcpServer server;
    server.listen(QHostAddress::Any, 6000);

    return app.exec();
}
