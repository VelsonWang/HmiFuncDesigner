#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <cstdlib>
#include <cstring>
#include "../Public/Public.h"
#include "ProjectDownloadDialog.h"
#include "ui_ProjectDownloadDialog.h"
#include "qsoftcore.h"
#include "qprojectcore.h"
#include <QDebug>


ProjectDownloadDialog::ProjectDownloadDialog(QWidget *parent, QString proj) :
    QDialog(parent),
    projFile(proj),
    ui(new Ui::ProjectDownloadDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    fileBuf.clear();
    memset((void *)&dataPackage, 0, sizeof (TDataPackage));
    ui->progressBar->setValue(0);
    ui->labelAddress->setText(getRuntimeIp());

    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(dataReceived()));

    port = 6000;
    serverIP = new QHostAddress();
    status = false;
}

ProjectDownloadDialog::~ProjectDownloadDialog()
{
    socket->disconnectFromHost();
    socket->waitForDisconnected();

    delete ui;
    delete serverIP;
    delete socket;
}

const QString ProjectDownloadDialog::getRuntimeIp()
{
    return QSoftCore::getCore()->getProjectCore()->m_projInfoMgr.getStationAddress();
}

void ProjectDownloadDialog::on_btnOk_clicked()
{  
    if(projFile == "") {
        QMessageBox::warning(this, tr("提示"), tr("未打开工程文件或未新建工程文件！"));
        return;
    }
    fileBuf.clear();
    memset((void *)&dataPackage, 0, sizeof (TDataPackage));

    QFile file(projFile);
    fileSize = file.size();

    dataPackage.total = fileSize / 512;
    if(fileSize % 512) {
        dataPackage.total += 1;
    }

    ui->progressBar->setRange(0, dataPackage.total - 1);

    file.open(QIODevice::ReadOnly);
    fileBuf.resize(dataPackage.total * 512);
    file.read(fileBuf.data(), fileSize);
    file.close();

    transferState = CMD_NONE;
    TMgsHeader mgsHeader;
    mgsHeader.length = sizeof(mgsHeader);
    mgsHeader.cmd = CMD_DOWNLOAD_PROJECT;

    if(!serverIP->setAddress(ui->labelAddress->text())) {
        qDebug() << "server ip address error!";
        QMessageBox::warning(this, tr("提示"), tr("工程文件IP设置错误！"));
        return;
    }

    socket->connectToHost (*serverIP, port);
    qDebug() << serverIP ->toString() << port;
    if (!socket->waitForConnected(1000)) {
        QMessageBox::warning(this, tr("提示"), tr("连接设备失败，请检查工程文件IP设置和网络连接！"));
    } else {
        qDebug("Connected!");
        socket->write((char *)&mgsHeader, sizeof(mgsHeader));
    }
}

void ProjectDownloadDialog::on_btnCancal_clicked()
{
    fileBuf.clear();
    this->reject();
}


void ProjectDownloadDialog::slotConnected()
{
    qDebug() << "tcp socket connected...";

}
void ProjectDownloadDialog::slotDisconnected()
{
    qDebug() << "tcp socket disconnected...";

}

void ProjectDownloadDialog::dataReceived()
{
    switch(transferState)
    {
        case CMD_NONE:
        case CMD_DOWNLOAD_PROJECT_ACK:
        {
            if (socket->bytesAvailable() < sizeof(TMgsHeader)) return;
            TMgsHeader mgsHeader;
            socket->read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length != sizeof(mgsHeader)) return;
            if(mgsHeader.cmd == CMD_DONE)
                transferState = CMD_DOWNLOAD_PROJECT;
            if(mgsHeader.cmd == CMD_NONE)
                transferState = CMD_NONE;
            transferFilePackage();
        }
        break;
        default:
        {
            transferState = CMD_NONE;
        }
        break;
    }
}

void ProjectDownloadDialog::transferFilePackage()
{
    int waitSendLen = sizeof(TDataPackage)+sizeof(TMgsHeader);
    TMgsHeader mgsHeader;
    mgsHeader.length = waitSendLen;
    mgsHeader.cmd = CMD_DOWNLOAD_PROJECT;

    TDataPackage dataPack;
    dataPack.total = dataPackage.total;
    dataPack.index = dataPackage.index;

    ui->progressBar->setValue(dataPack.index);
    int blockSize = sizeof(dataPackage.data) / sizeof(quint8);
    for(int i=0; i<blockSize; i++) {
        dataPack.data[i] = fileBuf[dataPack.index * blockSize + i];
    }
    if(dataPack.index == (dataPackage.total-1)) {
        if(blockSize * dataPackage.total > fileSize) {
            mgsHeader.length = sizeof(TDataPackage) + sizeof(TMgsHeader) - blockSize + (blockSize * dataPackage.total - fileSize);
        } else {
            mgsHeader.length = waitSendLen;
        }
        this->accept();
        fileBuf.clear();
    } else {
        mgsHeader.length = waitSendLen;
    }

    socket->write((char *)&mgsHeader, sizeof(mgsHeader));
    socket->write((char *)&dataPack, sizeof(dataPack));

    dataPackage.index++;
}
