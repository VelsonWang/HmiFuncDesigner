#include <QElapsedTimer>
#include "NetPort.h"
#include "publicfunction.h"
#include <QThread>
#include <QElapsedTimer>

NetPort::NetPort()
{
    //init only required for windows, no-op on *nix
    net::init();
}


NetPort::~NetPort()
{
    net::uninit();
}


/**
 * @brief NetPort::open
 * @param type
 * @param args
 * @return true-成功, false-失败
 */
bool NetPort::open(QString type, QStringList args)
{
    if(type == "" || args.length() != 2)
        return false;

    ip = args.at(0);
    port = args.at(1).toInt();

    return reOpen();
}

/**
 * @brief NetPort::reOpen
 * @return true-成功, false-失败
 */
bool NetPort::reOpen()
{
    if(sock.good()) {
        sock.close();
        sock.shutdown(net::shut::rdwr);
    }
    //inetV4 socket
    sock.init(net::af::inet, net::sock::stream);

    //make sure socket creation and binding did not fail
    if( !sock.good() )	{
        std::cerr << "error creating socket" << std::endl;
        return false;
    }
    sock.set_ipv4_options();

    endpoint.set(ip.toStdString(), port, net::af::inet);

    //int ret = sock.connect(endpoint, 3000);
    int ret = sock.connect(endpoint);
    if(ret == 0) {
        while(sock.available()) {
            char buf;
            sock.recv((char *)&buf, (std::size_t)1);
        }
    }

    return (ret == 0);
}

int NetPort::read(unsigned char *buf, int len, int ms)
{
    QElapsedTimer timer;
    timer.start();

    QByteArray byteArray;
    while(byteArray.size() < len && !timer.hasExpired(ms)) {
        if(sock.available()) {
            char tmp;
            int ret = (int)sock.recv((char *)&tmp, (std::size_t)1);
            if(ret > 0) {
                byteArray.append(tmp);
            }
        } else {
            QThread::msleep(1);
        }
    }
    if(timer.hasExpired(ms)) {
        sock.flush();
    }

    for(int i=0; i<byteArray.size(); i++) {
        buf[i] = byteArray[i];
    }

#if 0
    qDebug()<< "read: " << hexToString(byteArray.data(), len);
#endif

    return len;
}

int NetPort::write(unsigned char *buf, int len, int ms)
{
#if 0
    qDebug()<< "write: " << hexToString((char *)buf, len);
#endif
    Q_UNUSED(ms)
    std::size_t sendLen = sock.send((const char* )buf, (std::size_t)len);
    sock.flush();
    return (int)sendLen;
}


bool NetPort::close()
{
    sock.close();
    return true;
}

TPortType NetPort::getPortType()
{
    return PORT_NET;
}

