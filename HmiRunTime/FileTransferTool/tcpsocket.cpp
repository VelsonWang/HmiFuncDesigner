#include "tcpsocket.h"
#include <QHostAddress>
#include <QApplication>
#include <QProcess>
#include <QProcessEnvironment>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QDebug>


TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) :
    QTcpSocket(parent),
    socketID(socketDescriptor),
    transferState_(CMD_NONE),
    dataPackage_({0}),
    fileSize_(0)
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
                fileBuf_.clear();
                mgsHeader.cmd = CMD_DOWNLOAD_PROJECT_ACK;
            }
            break;
            case CMD_UPLOAD_PROJECT:
            {
                mgsHeader.cmd = CMD_UPLOAD_PROJECT;

                fileBuf_.clear();
                dataPackage_ = {0};

                QString projFileName = QCoreApplication::applicationDirPath() + "/Project/RunProject.tar";
                QFile file(projFileName);
                fileSize_ = file.size();

                dataPackage_.total = (fileSize_%512)?(fileSize_/512+1):(fileSize_/512);

                file.open(QIODevice::ReadOnly);
                fileBuf_.resize(dataPackage_.total*512);
                file.read(fileBuf_.data(), fileSize_);
                file.close();
                transferState_ = CMD_UPLOAD_PROJECT_ACK;
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
                fileBuf_.clear();
                sendMgsHeader.cmd = CMD_NONE;
                transferState_ = CMD_NONE;
                unTarProject();
            }
            write((char *)&sendMgsHeader, sizeof(sendMgsHeader));
        }
        break;
        case CMD_UPLOAD_PROJECT_ACK:
        {
            if (bytesAvailable() < sizeof(TMgsHeader)) return;
            TMgsHeader mgsHeader = {0};
            read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length != sizeof(mgsHeader)) return;
            if(mgsHeader.cmd == CMD_NONE)
                transferState_ = CMD_NONE;
            transferFilePackage();
        }
        break;
    }
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
    if(dir.exists())
    {
        dir.removeRecursively();
    }
    if(!dir.exists())
    {
        dir.mkpath(tmpDir);
    }

#if defined(Q_OS_WIN32)
    QString program = QCoreApplication::applicationDirPath() + "/tar/tar.exe";
#elif defined(Q_OS_LINUX)
    QString program = "tar";
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
    QString strDes = QCoreApplication::applicationDirPath();
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

    emit reStartRuntime();
}

void TcpSocket::transferFilePackage()
{
    int waitSendLen = sizeof(TDataPackage)+sizeof(TMgsHeader);
    TMgsHeader mgsHeader = {0};
    mgsHeader.length = waitSendLen;
    mgsHeader.cmd = CMD_UPLOAD_PROJECT;

    TDataPackage dataPack = {0};
    dataPack.total = dataPackage_.total;
    dataPack.index = dataPackage_.index;

    int blockSize = sizeof(dataPackage_.data)/sizeof(quint8);
    for(int i=0; i<blockSize; i++)
    {
        dataPack.data[i] = fileBuf_[dataPack.index*blockSize+i];
    }
    if(dataPack.index == (dataPackage_.total-1))
    {
        if(blockSize*dataPackage_.total>fileSize_)
        {
            mgsHeader.length = sizeof(TDataPackage)+sizeof(TMgsHeader)-blockSize+(blockSize*dataPackage_.total-fileSize_);
        }
        else
        {
            mgsHeader.length = waitSendLen;
        }
        transferState_ = CMD_NONE;
        fileBuf_.clear();
    }
    else
    {
        mgsHeader.length = waitSendLen;
    }

    write((char *)&mgsHeader, sizeof(mgsHeader));
    write((char *)&dataPack, sizeof(dataPack));

    dataPackage_.index++; 
}


