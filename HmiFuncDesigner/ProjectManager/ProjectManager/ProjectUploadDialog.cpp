#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QMessageBox>
#include "../Public/Public.h"
#include "ProjectUploadDialog.h"
#include "ui_ProjectUploadDialog.h"
#include "qsoftcore.h"
#include "qprojectcore.h"
#include <QDebug>

ProjectUploadDialog::ProjectUploadDialog(QWidget *parent, QString projName) :
    QDialog(parent),
    ui(new Ui::ProjectUploadDialog),
    fileSize(0)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));
    fileBuf.clear();
    memset((void *)&dataPackage, 0, sizeof (TDataPackage));
    this->configProjPath = projName;
    ui->progressBar->setValue(0);
    ui->editAddress->setText(getRuntimeIp());
    ui->editPath->setText(QCoreApplication::applicationDirPath() + "/UploadProjects");
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(sigUpdateProcessBar(int,int,int)), this, SLOT(sltUpdateProcessBar(int,int,int)));

    port = 6000;
    serverIP = new QHostAddress();
    status = false;
    fileName = "";
    fileSize = 0;
    pFileBuf = 0;
}

ProjectUploadDialog::~ProjectUploadDialog()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->waitForDisconnected();

    if(pFileBuf != 0) {
        delete[] pFileBuf;
        pFileBuf = 0;
    }
    delete ui;
    delete serverIP;
    delete tcpSocket;
}


QString ProjectUploadDialog::getRuntimeIp()
{
    return QSoftCore::getCore()->getProjectCore()->m_projInfoMgr.getStationAddress();
}


void ProjectUploadDialog::slotConnected()
{
    qDebug()<< "slotConnected...";

    transferState = CMD_NONE;
    TMgsHeader mgsHeader;
    mgsHeader.length = sizeof(mgsHeader);
    mgsHeader.cmd = CMD_UPLOAD_PROJECT;
    tcpSocket->write((char *)&mgsHeader, sizeof(mgsHeader));
}

void ProjectUploadDialog::slotDisconnected()
{
    qDebug() << "slotDisconnected...";
}

void ProjectUploadDialog::dataReceived()
{
    switch(transferState)
    {
        case CMD_NONE:
        {
            if (tcpSocket->bytesAvailable() < sizeof(TMgsHeader)) return;
            TMgsHeader mgsHeader;
            tcpSocket->read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length != sizeof(mgsHeader)) return;

            transferState = mgsHeader.cmd;
            switch(transferState)
            {
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
            tcpSocket->write((char *)&mgsHeader, sizeof(mgsHeader));
        }
        break;
        case CMD_UPLOAD_PROJECT:
        {
            int waitRecvLen = sizeof(TMgsHeader);
            if (tcpSocket->bytesAvailable() < waitRecvLen) return;
            TMgsHeader mgsHeader;
            tcpSocket->read((char *)&mgsHeader, sizeof(mgsHeader));

            if(mgsHeader.length < (sizeof(mgsHeader)+8) ||
                    mgsHeader.cmd != CMD_UPLOAD_PROJECT)
                return;

            TDataPackage dataPackage;
            int readLen = mgsHeader.length - sizeof(mgsHeader);
            tcpSocket->read((char *)&dataPackage, readLen);
            fileBuf.append((char *)dataPackage.data, readLen-8);

            TMgsHeader sendMgsHeader;
            sendMgsHeader.length = sizeof(sendMgsHeader);

            if(dataPackage.total != (dataPackage.index+1)) {
                sendMgsHeader.cmd = CMD_UPLOAD_PROJECT_ACK;
            } else {
                saveToFile(getProjectPath() + "/UpLoadProject.pdt", fileBuf);
                sendMgsHeader.cmd = CMD_NONE;
                transferState = CMD_NONE;
                this->accept();
            }
            tcpSocket->write((char *)&sendMgsHeader, sizeof(sendMgsHeader));

            emit sigUpdateProcessBar(0, dataPackage.total - 1, dataPackage.index);
        }
        break;
        default:
        {
        }
        break;
    }
}


QString ProjectUploadDialog::getProjectPath()
{
    QString desDir = QCoreApplication::applicationDirPath() + "/UploadProjects";
    if(ui->editPath->text() != "")
        return ui->editPath->text();
    return desDir;
}

void ProjectUploadDialog::saveToFile(QString filename, QByteArray fileBuf)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(fileBuf);
    file.close();
    fileBuf.clear();
}


void ProjectUploadDialog::on_btnSelectPath_clicked()
{
    // 创建UploadProjects目录
    QString dirUploadProjects = QCoreApplication::applicationDirPath() + "/UploadProjects";
    QDir dir(dirUploadProjects);
    if(!dir.exists()) {
        dir.mkpath(dirUploadProjects);
    }

    QString strDir = QFileDialog::getExistingDirectory(this,
                                                       tr("选择保存工程路径"),
                                                       dirUploadProjects,
                                                       QFileDialog::ShowDirsOnly|
                                                       QFileDialog::DontResolveSymlinks);
    ui->editPath->setText(strDir);
}

void ProjectUploadDialog::on_btnOK_clicked()
{
    if(ui->editPath->text() == "") {
        QMessageBox::information(this, "系统提示", "请选择上传工程文件保存路径！");
        return;
    }
    if(ui->editAddress->text() == "") {
        QMessageBox::information(this, "系统提示", "请设置RunTime的IP地址！");
        return;
    }

    if(!serverIP->setAddress(ui->editAddress->text())) {
        qDebug() << "server ip address error!";
        return;
    }

    tcpSocket->connectToHost (*serverIP, port);
    if (!tcpSocket->waitForConnected(1000)) {
        QMessageBox::information(this, "系统提示", "连接失败！");
    }
}

void ProjectUploadDialog::sltUpdateProcessBar(int min, int max, int value)
{
    ui->progressBar->setRange(min, max);
    ui->progressBar->setValue(value);
}

void ProjectUploadDialog::on_btnCancel_clicked()
{
    fileBuf.clear();
    this->reject();
}
