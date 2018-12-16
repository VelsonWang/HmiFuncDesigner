
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>

#include "../Public/Public.h"
#include "ProjectDownloadDialog.h"
#include "ui_ProjectDownloadDialog.h"

#include <QDebug>



ProjectDownloadDialog::ProjectDownloadDialog(QWidget *parent, QString projName) :
    QDialog(parent),
    ui(new Ui::ProjectDownloadDialog)
{
    ui->setupUi(this);
    this->configProjPath = projName;
    ui->progressBar->setValue(0);
    ui->labelAddress->setText(getRuntimeIp());
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(dataReceived()));

    port = 6000;
    serverIP = new QHostAddress();
    status = false;
    projFileName = "";

    if(!serverIP->setAddress(ui->labelAddress->text()))
    {
        qDebug() << "server ip address error!";
        return;
    }
    //qDebug() << *serverIP << port;
    tcpSocket->connectToHost (*serverIP, port);
    if (tcpSocket->waitForConnected(1000))
        qDebug("Connected!");
}

ProjectDownloadDialog::~ProjectDownloadDialog()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->waitForDisconnected();

    delete ui;
    delete serverIP;
    delete tcpSocket;
}

QString ProjectDownloadDialog::getRuntimeIp()
{
    QString sIp = "127.0.0.1";

    //qDebug() << this->configProjPath;

    QFile loadFile(this->configProjPath);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return sIp;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));

    sIp = loadDoc.object()["sStationAddress"].toString();

    loadFile.close();

    return sIp;
}

void ProjectDownloadDialog::setProjFileName(QString name)
{
    this->projFileName = name;
}

void ProjectDownloadDialog::setConfigProjPath(QString name)
{
    this->configProjPath = name;
}

void ProjectDownloadDialog::on_btnOk_clicked()
{  
    QByteArray msgBytes;
    QDataStream out(&msgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString msg = QString("connect to server");
    out << msg;

    tcpSocket->write(msgBytes, msgBytes.length());
}

void ProjectDownloadDialog::on_btnCancal_clicked()
{
    this->reject();
}


void ProjectDownloadDialog::slotConnected()
{
    int length = 0;

    QString msg = "slotConnected...";
    length=tcpSocket->write(msg.toLatin1(),msg.length());
    //tcpSocket->waitForBytesWritten(-1);
    //tcpSocket->waitForReadyRead(-1);
    if(length!=msg.length())
    {
        return ;
    }
}
void ProjectDownloadDialog::slotDisconnected()
{
    qDebug() << "slotDisconnected...";

}

void ProjectDownloadDialog::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        //QString msg = datagram.data();
        //qDebug() << msg.left(datagram.size());
        QDataStream in(&datagram, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_7);
        QString msg = onDataTransfer(in);
        if(msg != "")
            qDebug() << msg;
    }
}


QString ProjectDownloadDialog::onDataTransfer(QDataStream& in)
{
    QByteArray outMsgBytes;
    outMsgBytes.clear();
    QDataStream out(&outMsgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString inMsg = "";
    QString outMsg = "";

    in >> inMsg;
    //qDebug() << "server return: " << inMsg;

    if(inMsg == "connect to server")
    {
        outMsg = QString("download project");
        out << outMsg;
        tcpSocket->write(outMsgBytes, outMsgBytes.length());
    }
    if(inMsg == "download project")
    {
        if(projFileName == "")
            return "project file name empty.";

        // send file name
        QString strTarName = projFileName.right(projFileName.length() - projFileName.lastIndexOf("/") - 1);
        out << strTarName;

        // send file length
        QFile file(projFileName);
        quint32 filesize = file.size();
        //qDebug() << "size: " << filesize;
        out << (quint32)filesize;
        tcpSocket->write(outMsgBytes, outMsgBytes.length());
        outMsgBytes.clear();

        int iPackageCnt = (filesize%1024)?(filesize/1024+1):(filesize/1024);
        ui->progressBar->setRange(0, iPackageCnt-1);

        // send file data
        file.open(QIODevice::ReadOnly);
        char *pFileBuf = new char[iPackageCnt*1024];
        int rCnt = file.read(pFileBuf, filesize);
        file.close();
        if(rCnt != filesize)
        {
            //qDebug() << "read file error, read size: " << rCnt;
            return "read file error.";
        }
        char *pBuf = new char[1024];
        //qDebug() << "iPackageCnt: " << iPackageCnt;
        for(int i=0; i<iPackageCnt;i++)
        {
            ui->progressBar->setValue(i);
            for(int j=0;j<1024;j++)
            {
                pBuf[j] = pFileBuf[i*1024+j];
            }
            if(i==(iPackageCnt-1))
            {
                if(1024*iPackageCnt>filesize)
                {
                    tcpSocket->write((const char*)pBuf, 1024*iPackageCnt-filesize);
                }
                else
                {
                    tcpSocket->write((const char*)pBuf, 1024);
                }
            }
            else
            {
                tcpSocket->write((const char*)pBuf, 1024);
            }
            //qDebug() << i << "/" << iPackageCnt;
        }
        delete[] pBuf;
        delete[] pFileBuf;
    }
    else if(inMsg == "completed")
    {
        outMsgBytes.clear();
        // send complete message
        out << QString("completed");
        tcpSocket->write(outMsgBytes, outMsgBytes.length());

        this->accept();
    }
    return "";
}
