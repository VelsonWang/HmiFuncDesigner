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
    explicit ProjectDownloadDialog(QWidget *parent = 0, QString proj = "");
    ~ProjectDownloadDialog();

private:
    const QString getRuntimeIp();
    void transferFilePackage();

private slots:
    void on_btnOk_clicked();
    void on_btnCancal_clicked();

private:
    quint16 port = 6000;
    QHostAddress *serverIP;
    QTcpSocket *socket;
    bool status = false;
    QString projFile = "";
    int transferState = 0;
    TDataPackage dataPackage;
    QByteArray fileBuf;
    quint32 fileSize = 0;

private slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();

private:
    Ui::ProjectDownloadDialog *ui;
};

#endif // PROJECTDOWNLOADDIALOG_H
