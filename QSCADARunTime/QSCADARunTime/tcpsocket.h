#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QDataStream>
#include <QTime>
#include <QFile>
#include "SCADARunTime.h"

enum DataTransferType
{
    None = 0,
    DwonloadProject,
    UploadProject,
};
typedef enum DataTransferType TDataTransferType;


class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(SCADARunTime *pRunTime, qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocket();
    QByteArray handleData(QByteArray data,const QString & ip, qint16 port);//用来处理数据的函数

signals:
    //void readData(const int,const QString &,const quint16,const QByteArray &);
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
    void sentData(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);

protected slots:
    void readData();//接收数据
    void startNext();//处理下一个

protected:
    QFutureWatcher<QByteArray> watcher;
    QQueue<QByteArray> datas;
private:
    qintptr socketID;
    QMetaObject::Connection dis;

    ///////////////////////////////////

private:
    void saveToFile(QString filename, char* pBuf, int len);
    void unTarProject();
    void uploadProject();

private:
    int iPackageCnt;
    int offset;
    char *pFileBuf;
    QString fileName;
    quint32 fileSize;
    TDataTransferType DataTransferType;
    SCADARunTime *m_pRunTime;
};

#endif // TCPSOCKET_H
