#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>

#include "../Public/Public.h"
#include "ProjectDownloadDialog.h"
#include "ui_ProjectDownloadDialog.h"
#include "ProjectData.h"

#include <QDebug>


ProjectDownloadDialog::ProjectDownloadDialog(QWidget *parent, QString projName) :
    QDialog(parent),
    ui(new Ui::ProjectDownloadDialog),
    transferState_(CMD_NONE),
    dataPackage_({0}),
    fileSize_(0)
{
    ui->setupUi(this);
    fileBuf_.clear();
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
    ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
    projInfoMgr.load(ProjectData::getInstance()->dbData_);
    sIp = projInfoMgr.getStationAddress();
    return sIp;
}

void ProjectDownloadDialog::setProjFileName(QString name)
{
    this->projFileName = name;
}

void ProjectDownloadDialog::on_btnOk_clicked()
{  
    if(projFileName == "") return;
    fileBuf_.clear();
    dataPackage_ = {0};

    // send file length
    QFile file(projFileName);
    fileSize_ = file.size();

    dataPackage_.total = (fileSize_%512)?(fileSize_/512+1):(fileSize_/512);
    ui->progressBar->setRange(0, dataPackage_.total-1);

    file.open(QIODevice::ReadOnly);
    fileBuf_.resize(dataPackage_.total*512);
    file.read(fileBuf_.data(), fileSize_);
    file.close();

    transferState_ = CMD_NONE;
    TMgsHeader mgsHeader = {0};
    mgsHeader.length = sizeof(mgsHeader);
    mgsHeader.cmd = CMD_DOWNLOAD_PROJECT;
    tcpSocket->write((char *)&mgsHeader, sizeof(mgsHeader));
}

void ProjectDownloadDialog::on_btnCancal_clicked()
{
    fileBuf_.clear();
    this->reject();
}


void ProjectDownloadDialog::slotConnected()
{
    QString msg = "slotConnected...";

}
void ProjectDownloadDialog::slotDisconnected()
{
    qDebug() << "slotDisconnected...";

}

void ProjectDownloadDialog::dataReceived()
{
    switch(transferState_)
    {
        case CMD_NONE:
        case CMD_DOWNLOAD_PROJECT_ACK:
        {
            if (tcpSocket->bytesAvailable() < sizeof(TMgsHeader)) return;
            TMgsHeader mgsHeader = {0};
            tcpSocket->read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length != sizeof(mgsHeader)) return;
            if(mgsHeader.cmd == CMD_DONE)
                transferState_ = CMD_DOWNLOAD_PROJECT;
            if(mgsHeader.cmd == CMD_NONE)
                transferState_ = CMD_NONE;
            transferFilePackage();
        }
        break;
        default:
        {
            transferState_ = CMD_NONE;
        }
        break;
    }
}

void ProjectDownloadDialog::transferFilePackage()
{
    int waitSendLen = sizeof(TDataPackage)+sizeof(TMgsHeader);
    TMgsHeader mgsHeader = {0};
    mgsHeader.length = waitSendLen;
    mgsHeader.cmd = CMD_DOWNLOAD_PROJECT;

    TDataPackage dataPack = {0};
    dataPack.total = dataPackage_.total;
    dataPack.index = dataPackage_.index;

    ui->progressBar->setValue(dataPack.index);
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
        this->accept();
        fileBuf_.clear();
    }
    else
    {
        mgsHeader.length = waitSendLen;
    }

    tcpSocket->write((char *)&mgsHeader, sizeof(mgsHeader));
    tcpSocket->write((char *)&dataPack, sizeof(dataPack));

    dataPackage_.index++;

}
