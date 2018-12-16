#ifndef PROJECTUPLOADDIALOG_H
#define PROJECTUPLOADDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

enum DataTransferType
{
    None = 0,
    MsgTransfer,
    UploadProject,
};
typedef enum DataTransferType TDataTransferType;

namespace Ui {
class ProjectUploadDialog;
}

class ProjectUploadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectUploadDialog(QWidget *parent = 0, QString projName = "");
    ~ProjectUploadDialog();
    QString getProjectPath();

private:
    QString getRuntimeIp();
    void saveToFile(QString filename, char* pBuf, int len);

private:
    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;
    bool status;
    QString configProjPath;

private slots:
    void slotConnected();
    void slotDisconnected();
    void dataReceived();

    void on_btnSelectPath_clicked();
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    Ui::ProjectUploadDialog *ui;
    TDataTransferType DataTransferType;
    char *pFileBuf;
    QString fileName;
    quint32 fileSize;
};

#endif // PROJECTUPLOADDIALOG_H
