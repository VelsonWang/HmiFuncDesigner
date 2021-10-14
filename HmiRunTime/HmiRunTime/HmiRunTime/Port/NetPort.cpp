#include <QTime>
#include "NetPort.h"
#include "Public/PublicFunction.h"
#include <QThread>

NetPort::NetPort(QString sIp,int tPort)
{
 	client = NULL;
	port_ = tPort;
	ip_ = sIp;
    if(sIp.toLower() == "localhost") ip_ = "127.0.0.1";
}


NetPort::~NetPort()
{
    if(client) {
		client->close();
		delete client;
		client = NULL;
	}
}

QTcpSocket* NetPort::getSocket()
{
    if(client) {
		client->close();
		delete client;
		client = NULL;
	}
    if(!client) {
		client = new QTcpSocket();
		client->setReadBufferSize(10000);
	}
	return client;
}

/**
 * @brief NetPort::open
 * @param port
 * @param args
 * @return true-成功, false-失败
 */
bool NetPort::open(QString port, QStringList args)
{
    if(port == "" || args.length() != 2) return false;
    ip_ = args.at(0);
    if(args.at(0).toLower() == "localhost") ip_ = "127.0.0.1";
    port_ = args.at(1).toInt();
    // 本函数在主线程调用！！！
    // 为避免跨线程调用QTcpSocket，在这里先不创建对象, 对象的创建放在线程中
    return true;
}

/**
 * @brief NetPort::reOpen
 * @return true-成功, false-失败
 */
bool NetPort::reOpen()
{
	getSocket();
	client->close();
	client->connectToHost(QHostAddress(ip_), port_);
    return client->waitForConnected(1000);
}

int NetPort::read(unsigned char *buf, int len, int ms)
{
    if (!client) reOpen();
    long start;

    QTime time;
    time.start();
    start = time.elapsed();
    QByteArray byteArray;

	int iNeed = len;
	int iAvailable;
	while (iNeed > 0) {
		iAvailable = client->bytesAvailable();
        if(iAvailable) {
			iAvailable = (iAvailable > iNeed) ? iNeed : iAvailable;
			//qDebug() << "read Begin";
			byteArray.append(client->read(iAvailable));
			//qDebug() << "read End";
			iNeed -= iAvailable;
		}
        if ((time.elapsed() - start) > ms) {
            if (byteArray.size() < len) {
				len = byteArray.size();
			}
			break;
		}
		QThread::msleep(1);
	}

	for (int i = 0; i < len; i++) {
		buf[i] = byteArray[i];
	}

#if 0
    if(len) {
		qDebug() << "read: " << hexToString(byteArray.data(), len); 
    } else {
		qDebug() << "read: 0 byes";
	}
#endif

    return len;
}

int NetPort::write(unsigned char *buf, int len, int ms)
{
    if(!client) reOpen();
#if 0
    qDebug()<< "write: " << hexToString((char *)buf, len);
#endif
    Q_UNUSED(ms)
    int iAvailable = client->bytesAvailable();
    if(iAvailable) client->readAll();
	len = client->write((const char*)buf,len);
    if(client->waitForReadyRead(ms)) return len;
	return 0;
}


bool NetPort::close()
{
    if (client) client->close();
    return true;
}

TPortType NetPort::getPortType()
{
    return PORT_NET;
}

