#ifndef PROJECTDOWNLOADDIALOG_H
#define PROJECTDOWNLOADDIALOG_H

#include "FileTansfer.h"
#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

namespace Ui {
class ProjectDownloadDialog;
}

class ProjectDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectDownloadDialog(QWidget *parent = 0, QString projName = "");
    ~ProjectDownloadDialog();
    void setProjFileName(QString name);

private:
    QString getRuntimeIp();
    void transferFilePackage();

private slots:
    void on_btnOk_clicked();
    void on_btnCancal_clicked();

private:
    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;
    bool status;
    QString projFileName;
    int transferState_;
    TDataPackage dataPackage_;
    QByteArray fileBuf_;
    int fileSize_;

private slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();

private:
    Ui::ProjectDownloadDialog *ui;

};

#endif // PROJECTDOWNLOADDIALOG_H
