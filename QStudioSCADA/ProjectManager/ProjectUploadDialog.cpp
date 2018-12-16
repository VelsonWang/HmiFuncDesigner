
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QMessageBox>

#include "../Public/Public.h"
#include "ProjectUploadDialog.h"
#include "ui_ProjectUploadDialog.h"

#include <QDebug>

ProjectUploadDialog::ProjectUploadDialog(QWidget *parent, QString projName) :
    QDialog(parent),
    ui(new Ui::ProjectUploadDialog)
{
    ui->setupUi(this);
    this->configProjPath = projName;
    ui->progressBar->setValue(0);
    ui->editAddress->setText(getRuntimeIp());
    ui->editPath->setText(QCoreApplication::applicationDirPath() + "/UploadProjects");
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    port = 6000;
    serverIP = new QHostAddress();
    status = false;
    DataTransferType = None;
    fileName = "";
    fileSize = 0;
    pFileBuf = 0;

    if(!serverIP->setAddress(ui->editAddress->text()))
    {
        qDebug() << "server ip address error!";
        return;
    }
}

ProjectUploadDialog::~ProjectUploadDialog()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->waitForDisconnected();

    if(pFileBuf != 0)
    {
        delete[] pFileBuf;
        pFileBuf = 0;
    }
    delete ui;
    delete serverIP;
    delete tcpSocket;
}


QString ProjectUploadDialog::getRuntimeIp()
{
    QString sIp = "127.0.0.1";

    qDebug() << this->configProjPath;
    QFile loadFile(this->configProjPath);
    if(!loadFile.exists())
        return sIp;
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open load file.");
        return sIp;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(DATA_SAVE_FORMAT == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));

    sIp = loadDoc.object()["sStationAddress"].toString();

    loadFile.close();

    return sIp;
}


void ProjectUploadDialog::slotConnected()
{
    qDebug()<< "slotConnected...";

    QByteArray msgBytes;
    QDataStream out(&msgBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString msg = QString("connect to server");
    out << msg;
    DataTransferType = MsgTransfer;

    tcpSocket->write(msgBytes, msgBytes.length());
}
void ProjectUploadDialog::slotDisconnected()
{
    qDebug() << "slotDisconnected...";

}

void ProjectUploadDialog::dataReceived()
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

    switch(DataTransferType)
    {
        case MsgTransfer:
        {
            while(tcpSocket->bytesAvailable()>0)
            {
                QByteArray datagram;
                datagram.resize(tcpSocket->bytesAvailable());
                tcpSocket->read(datagram.data(),datagram.size());
                inMsgBytes.append(datagram);
            }
            in >> inMsg;
            //qDebug() << inMsg;
            if(inMsg == "connect to server")
            {
                DataTransferType = UploadProject;
                out << QString("upload project");
                tcpSocket->write(outMsgBytes, outMsgBytes.length());
            }
            else if(inMsg == "completed")
            {
                DataTransferType = MsgTransfer;
                outMsgBytes.clear();
                // send complete message
                out << QString("completed");
                tcpSocket->write(outMsgBytes, outMsgBytes.length());

                this->accept();
            }
        }
        break;
        case UploadProject:
        {
            if (fileSize == 0)
            {
                QDataStream stream(tcpSocket);
                stream.setVersion(QDataStream::Qt_5_7);

                // file.size + "x.tar".length
                if (tcpSocket->bytesAvailable() < sizeof(quint32) + 5 )
                    return;

                stream >> fileName >> fileSize;
                //qDebug() << fileName << " " << fileSize;
                // 设置进度条范围
                ui->progressBar->setRange(0, fileSize);
                if(pFileBuf != 0)
                {
                    delete[] pFileBuf;
                    pFileBuf = 0;
                }
                pFileBuf = new char[fileSize];
            }

            //qDebug() << tcpSocket->bytesAvailable();
            // 设置进度条值
            ui->progressBar->setValue(tcpSocket->bytesAvailable());

            if (fileSize > tcpSocket->bytesAvailable())
                return;

            tcpSocket->read(pFileBuf, fileSize);
            saveToFile(fileName, pFileBuf, fileSize);
            DataTransferType = MsgTransfer;

            outMsgBytes.clear();
            out << QString("completed");
            tcpSocket->write(outMsgBytes, outMsgBytes.length());
        }
        break;
    }
    return;
}


QString ProjectUploadDialog::getProjectPath()
{
    QString desDir = QCoreApplication::applicationDirPath() + "/UploadProjects";
    if(ui->editPath->text()!="")
        return ui->editPath->text();
    return desDir;
}

void ProjectUploadDialog::saveToFile(QString filename, char* pBuf, int len)
{
    QString desDir = QCoreApplication::applicationDirPath() + "/UploadProjects";
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


void ProjectUploadDialog::on_btnSelectPath_clicked()
{
    // 创建UploadProjects目录
    QString dirUploadProjects = QCoreApplication::applicationDirPath() + "/UploadProjects";
    QDir dir(dirUploadProjects);
    if(!dir.exists())
    {
        dir.mkpath(dirUploadProjects);
    }

    QString strDir = QFileDialog::getExistingDirectory(this, tr("选择保存工程路径"),
                                                       dirUploadProjects,
                                                       QFileDialog::ShowDirsOnly|
                                                       QFileDialog::DontResolveSymlinks);
    ui->editPath->setText(strDir);
}

void ProjectUploadDialog::on_btnOK_clicked()
{
    if(ui->editPath->text() == "")
    {
        QMessageBox::information(this, "系统提示", "请选择上传工程文件保存路径！");
        return;
    }
    if(ui->editAddress->text() == "")
    {
        QMessageBox::information(this, "系统提示", "请设置RunTime的IP地址！");
        return;
    }

    tcpSocket->connectToHost (*serverIP, port);
    if (!tcpSocket->waitForConnected(1000))
    {
        QMessageBox::information(this, "系统提示", "连接失败！");
    }
}

void ProjectUploadDialog::on_btnCancel_clicked()
{
    this->reject();
}
