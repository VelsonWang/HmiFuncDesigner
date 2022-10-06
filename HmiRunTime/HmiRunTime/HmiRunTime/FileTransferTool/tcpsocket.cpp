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
    socketID(socketDescriptor)
{
    fileBuf.clear();
    memset((void *)&dataPackage, 0, sizeof(TDataPackage));

    this->setSocketDescriptor(socketDescriptor);
    connect(this, &TcpSocket::readyRead, this, &TcpSocket::slotReadData);
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
    if (i == socketID) {
        this->disconnectFromHost();
    } else if (i == -1) { //-1为全部断开
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::slotReadData()
{
    switch(transferState)
    {
        case CMD_NONE:
        {
            if (bytesAvailable() < sizeof(TMgsHeader)) return;
            TMgsHeader mgsHeader;
            read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length != sizeof(mgsHeader)) return;

            transferState = mgsHeader.cmd;
            switch(transferState)
            {
            case CMD_DOWNLOAD_PROJECT:
            {
                fileBuf.clear();
                mgsHeader.cmd = CMD_DOWNLOAD_PROJECT_ACK;
            }
            break;
            case CMD_UPLOAD_PROJECT:
            {
                mgsHeader.cmd = CMD_UPLOAD_PROJECT;

                fileBuf.clear();
                memset((void *)&dataPackage, 0, sizeof(TDataPackage));

                QString projFileName = QCoreApplication::applicationDirPath() + "/RunProject/RunProject.pdt";
                QFile file(projFileName);
                fileSize = file.size();

                dataPackage.total = fileSize / 512;
                if(fileSize % 512) {
                    dataPackage.total += 1;
                }

                file.open(QIODevice::ReadOnly);
                fileBuf.resize(dataPackage.total * 512);
                file.read(fileBuf.data(), fileSize);
                file.close();
                transferState = CMD_UPLOAD_PROJECT_ACK;
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
            TMgsHeader mgsHeader;
            read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length < (sizeof(mgsHeader)+8) || mgsHeader.cmd != CMD_DOWNLOAD_PROJECT)
                return;

            TDataPackage dataPackage;
            int readLen = mgsHeader.length - sizeof(mgsHeader);
            read((char *)&dataPackage, readLen);
            fileBuf.append((char *)dataPackage.data, readLen-8);

            TMgsHeader sendMgsHeader;
            sendMgsHeader.length = sizeof(sendMgsHeader);

            if(dataPackage.total != (dataPackage.index+1)) {
                sendMgsHeader.cmd = CMD_DOWNLOAD_PROJECT_ACK;
            } else {
                saveToFile(fileBuf);
                fileBuf.clear();
                sendMgsHeader.cmd = CMD_NONE;
                transferState = CMD_NONE;
                emit reStartRuntime();
            }
            write((char *)&sendMgsHeader, sizeof(sendMgsHeader));
        }
        break;
        case CMD_UPLOAD_PROJECT_ACK:
        {
            if (bytesAvailable() < sizeof(TMgsHeader)) return;
            TMgsHeader mgsHeader;
            read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length != sizeof(mgsHeader)) return;
            if(mgsHeader.cmd == CMD_NONE)
                transferState = CMD_NONE;
            transferFilePackage();
        }
        break;
    }
}

void TcpSocket::saveToFile(QByteArray fileBuf)
{
    QString desDir = QCoreApplication::applicationDirPath() + "/RunProject";
    QDir dirProj(desDir);
    if(!dirProj.exists()) {
        dirProj.mkpath(desDir);
    }
    QFile file(desDir + "/RunProject.pdt");
    file.open(QIODevice::WriteOnly);
    file.write(fileBuf);
    file.close();
}

void TcpSocket::transferFilePackage()
{
    int waitSendLen = sizeof(TDataPackage)+sizeof(TMgsHeader);
    TMgsHeader mgsHeader;
    mgsHeader.length = waitSendLen;
    mgsHeader.cmd = CMD_UPLOAD_PROJECT;

    TDataPackage dataPack;
    dataPack.total = dataPackage.total;
    dataPack.index = dataPackage.index;

    int blockSize = sizeof(dataPackage.data) / sizeof(quint8);
    for(int i=0; i<blockSize; i++) {
        dataPack.data[i] = fileBuf[dataPack.index*blockSize+i];
    }
    if(dataPack.index == (dataPackage.total-1)) {
        if(blockSize*dataPackage.total>fileSize) {
            mgsHeader.length = sizeof(TDataPackage) + sizeof(TMgsHeader) - blockSize + (blockSize * dataPackage.total - fileSize);
        } else {
            mgsHeader.length = waitSendLen;
        }
        transferState = CMD_NONE;
        fileBuf.clear();
    } else {
        mgsHeader.length = waitSendLen;
    }

    write((char *)&mgsHeader, sizeof(mgsHeader));
    write((char *)&dataPack, sizeof(dataPack));

    dataPackage.index++;
}


