#include "tcpsocket.h"
#include "CommandLineParser.h"
#include "RealTimeDB.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QApplication>
#include <QProcess>
#include <QProcessEnvironment>
#include <QFile>
#include <QDir>
#include <QDebug>


TcpSocket::TcpSocket(SCADARunTime *pRunTime, qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),
    socketID(socketDescriptor),
    m_pRunTime(pRunTime)
{
    DataTransferType = None;
    offset = 0;
    pFileBuf = 0;
    fileName = "";
    fileSize = 0;
    iPackageCnt = 0;

    this->setSocketDescriptor(socketDescriptor);
    connect(this, &TcpSocket::readyRead, this, &TcpSocket::readData);
    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            qDebug() << "disconnect" ;
            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect" ;
}

TcpSocket::~TcpSocket()
{
    if(pFileBuf != 0)
    {
        delete[] pFileBuf;
        pFileBuf = 0;
    }
}



void TcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
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
    QByteArray outMsgBytes;
    outMsgBytes.clear();
    QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QByteArray inMsgBytes;
    inMsgBytes.clear();
    QDataStream in(&inMsgBytes, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_7);

    QString inMsg = "";
    QString outMsg = "";

    //qDebug() << "DataTransferType: " << DataTransferType;

    switch(DataTransferType)
    {
        case None:
        {
            while(bytesAvailable()>0)
            {
                QByteArray datagram;
                datagram.resize(bytesAvailable());
                read(datagram.data(),datagram.size());
                inMsgBytes.append(datagram);
            }
            in >> inMsg;

            if(inMsg == "connect to server")
            {
                out << QString("connect to server");
                this->write(outMsgBytes, outMsgBytes.length());
            }
            else if(inMsg == "download project")
            {
                DataTransferType = DwonloadProject;
                out << QString("download project");
                this->write(outMsgBytes, outMsgBytes.length());
            }
            else if(inMsg == "upload project")
            {
                DataTransferType = None;
                uploadProject();

                outMsgBytes.clear();
                // send complete message
                out << QString("completed");
                write(outMsgBytes, outMsgBytes.length());
            }
        }
        break;
        case DwonloadProject:
        {
            if (fileSize == 0)
            {
                QDataStream stream(this);
                stream.setVersion(QDataStream::Qt_5_7);

                // file.size + "x.tar".length
                if (bytesAvailable() < sizeof(quint32) + 5 )
                    return;

                stream >> fileName >> fileSize;
                if(pFileBuf != 0)
                {
                    delete[] pFileBuf;
                    pFileBuf = 0;
                }
                pFileBuf = new char[fileSize];
                //qDebug() << "filename: " << fileName << "size: " << fileSize;
            }
            //qDebug() << "bytesAvailable: " << bytesAvailable();
            if (fileSize > bytesAvailable())
                return;

            read(pFileBuf, fileSize);
            saveToFile(fileName, pFileBuf, fileSize);
            DataTransferType = None;

            // 解压工程
            unTarProject();

            outMsgBytes.clear();
            out << QString("completed");
            this->write(outMsgBytes, outMsgBytes.length());
        }
        break;
        case UploadProject:
        {

        }
        break;
    }
    if(inMsg == "completed")
    {
        DataTransferType = None;
        outMsgBytes.clear();
        // send complete message
        out << QString("completed");
        write(outMsgBytes, outMsgBytes.length());
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

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}


void TcpSocket::saveToFile(QString filename, char* pBuf, int len)
{
    QString desDir = QCoreApplication::applicationDirPath() + "/Project";
    QDir dirProj(desDir);
    if(!dirProj.exists())
    {
        dirProj.mkpath(desDir);
    }
    QFile file(desDir + "/" + filename);
    file.open(QIODevice::WriteOnly);
    file.write(pBuf, len);
    file.close();
}

/*
* 解压工程
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
