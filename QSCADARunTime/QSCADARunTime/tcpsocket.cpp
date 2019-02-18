#include "tcpsocket.h"
#include "FileTansfer.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QApplication>
#include <QProcess>
#include <QProcessEnvironment>
#include <QFile>
#include <QDir>
#include <QDebug>


TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) :
    QTcpSocket(parent),
    socketID(socketDescriptor),
    transferState_(CMD_NONE)
{
    fileBuf_.clear();

    this->setSocketDescriptor(socketDescriptor);
    connect(this, &TcpSocket::readyRead, this, &TcpSocket::readData);
    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            qDebug() << "disconnect" ;
            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    qDebug() << "new connect" ;
}

TcpSocket::~TcpSocket()
{

}


void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::readData()
{
    switch(transferState_)
    {
        case CMD_NONE:
        {
            if (bytesAvailable() < sizeof(TMgsHeader)) return;
            TMgsHeader mgsHeader = {0};
            read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length != sizeof(mgsHeader)) return;

            transferState_ = mgsHeader.cmd;
            switch(transferState_)
            {
            case CMD_DOWNLOAD_PROJECT:
            {
                mgsHeader.cmd = CMD_DOWNLOAD_PROJECT_ACK;
            }
            break;
            case CMD_UPLOAD_PROJECT:
            {
                mgsHeader.cmd = CMD_UPLOAD_PROJECT_ACK;
            }
            break;
            default:
            {
                mgsHeader.cmd = CMD_DONE;
            }
            break;
            }
            write((char *)&mgsHeader, sizeof(mgsHeader));
        }
        break;
        case CMD_DOWNLOAD_PROJECT:
        {
            int waitRecvLen = sizeof(TMgsHeader);
            if (bytesAvailable() < waitRecvLen) return;
            TMgsHeader mgsHeader = {0};
            read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length < (sizeof(mgsHeader)+8) ||
                    mgsHeader.cmd != CMD_DOWNLOAD_PROJECT)
                return;

            TDataPackage dataPackage = {0};
            int readLen = mgsHeader.length - sizeof(mgsHeader);
            read((char *)&dataPackage, readLen);
            fileBuf_.append((char *)dataPackage.data, readLen-8);

            TMgsHeader sendMgsHeader = {0};
            sendMgsHeader.length = sizeof(sendMgsHeader);

            if(dataPackage.total != (dataPackage.index+1))
            {
                sendMgsHeader.cmd = CMD_DOWNLOAD_PROJECT_ACK;
            }
            else
            {
                saveToFile(fileBuf_);
                sendMgsHeader.cmd = CMD_NONE;
                transferState_ = CMD_NONE;
                unTarProject();
            }
            write((char *)&sendMgsHeader, sizeof(sendMgsHeader));
        }
        break;
        case CMD_UPLOAD_PROJECT:
        {

        }
        break;
    }
}

void TcpSocket::uploadProject()
{
    QByteArray outMsgBytes;
    outMsgBytes.clear();
    QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString inMsg = "";
    QString outMsg = "";

    QString projFileName = QCoreApplication::applicationDirPath() + "/Project/RunProject.tar";

    QFile file(projFileName);
    if(!file.exists())
        return;

    // send file name
    QString strTarName = projFileName.right(projFileName.length() - projFileName.lastIndexOf("/") - 1);
    out << strTarName;

    // send file length
    quint32 filesize = file.size();
    out << (quint32)filesize;
    //qDebug()<<strTarName << " "<< filesize;
    write(outMsgBytes, outMsgBytes.length());
    outMsgBytes.clear();

    int iPackageCnt = (filesize%1024)?(filesize/1024+1):(filesize/1024);

    // send file data
    file.open(QIODevice::ReadOnly);
    char *pFileBuf = new char[iPackageCnt*1024];
    int rCnt = file.read(pFileBuf, filesize);
    file.close();

    if(rCnt != filesize)
        return;

    char *pBuf = new char[1024];

    for(int i=0; i<iPackageCnt;i++)
    {
        for(int j=0;j<1024;j++)
        {
            pBuf[j] = pFileBuf[i*1024+j];
        }
        if(i==(iPackageCnt-1))
        {
            if(1024*iPackageCnt>filesize)
            {
                write((const char*)pBuf, 1024*iPackageCnt-filesize);
            }
            else
            {
                write((const char*)pBuf, 1024);
            }
        }
        else
        {
            write((const char*)pBuf, 1024);
        }
    }
    delete[] pBuf;
    delete[] pFileBuf;
}


QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{ 
    QTime time;
    time.start();

    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
    data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    return data;
}

void TcpSocket::saveToFile(QByteArray fileBuf)
{
    QString desDir = QCoreApplication::applicationDirPath() + "/Project";
    QDir dirProj(desDir);
    if(!dirProj.exists())
    {
        dirProj.mkpath(desDir);
    }
    QFile file(desDir + "/RunProject.tar");
    file.open(QIODevice::WriteOnly);
    file.write(fileBuf);
    file.close();
}


/** 解压工程
 * @brief TcpSocket::unTarProject
 */
void TcpSocket::unTarProject()
{
    // 创建tmp目录
    QString tmpDir = QCoreApplication::applicationDirPath() + "/RunProject";
    QDir dir(tmpDir);
    if(!dir.exists())
    {
        dir.mkpath(tmpDir);
    }

#if defined(Q_OS_WIN32)
    QString program = QCoreApplication::applicationDirPath() + "/tar/tar.exe";
#elif defined(Q_OS_LINUX)
    QString program = "/bin/tar";
#endif

    QFile programFile(program);
    if(!programFile.exists())
    {
        return;
    }

    QProcess *tarProc = new QProcess;

#if defined(Q_OS_WIN32)
    // 设置进程工作目录
    tarProc->setWorkingDirectory(QCoreApplication::applicationDirPath() + "/tar");
    QStringList arguments;
    arguments << "-xvf" << "../Project/RunProject.tar" << "-C"<< "../";
#elif defined(Q_OS_LINUX)
    tarProc->setWorkingDirectory(QCoreApplication::applicationDirPath() + "/tar");
    QStringList arguments;
    QString strSrc = QCoreApplication::applicationDirPath()+"/Project/RunProject.tar";
    QString strDes = QCoreApplication::applicationDirPath()+"/RunProject";
    arguments << "-xvf" << strSrc << "-C"<< strDes;
#endif

    tarProc->start(program, arguments);
    if(tarProc->waitForStarted())
    {
        if(tarProc->waitForFinished())
        {
            //qDebug()<< "process finished.";
        }
        if(tarProc->exitStatus() == QProcess::NormalExit)
        {
            //qDebug()<< "process exitStatus is QProcess::NormalExit.";
        }
        else // QProcess::CrashExit
        {
            //qDebug()<< "process exitStatus is QProcess::CrashExit.";
        }
    }
    else
    {

    }
    delete tarProc;
}
