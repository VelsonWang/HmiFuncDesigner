#ifndef PROJECTUPLOADDIALOG_H
#define PROJECTUPLOADDIALOG_H

#include "FileTansfer.h"
#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

namespace Ui {
class ProjectUploadDialog;
}

class ProjectUploadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectUploadDialog(QWidget *parent = 0, QString proj = "");
    ~ProjectUploadDialog();
    QString getProjectPath();

private:
    QString getRuntimeIp();
    void saveToFile(QString filename, QByteArray fileBuf);

private:
    int port = 6000;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;
    bool status = false;
    QString configProjPath;

signals:
    void sigUpdateProcessBar(int min, int max, int value);

private slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void sltUpdateProcessBar(int min, int max, int value);
    void on_btnSelectPath_clicked();
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    Ui::ProjectUploadDialog *ui;
    char *pFileBuf;
    QString fileName;
    quint32 fileSize;
    int transferState;
    TDataPackage dataPackage;
    QByteArray fileBuf;
};

#endif // PROJECTUPLOADDIALOG_H
