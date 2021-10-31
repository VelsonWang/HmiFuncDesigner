#include <QTime>
#include <QDebug>
#include <QThread>
#include "ComPort.h"
#include "publicfunction.h"
#include "SerialPortReMapping.h"

#define TIME_OUT 10

ComPort::ComPort()
{
    serialPortPtr_ = NULL;
    buf_.clear();
}


ComPort::~ComPort()
{
    close();
    if(serialPortPtr_ != NULL) {
        delete serialPortPtr_;
        serialPortPtr_ = NULL;
    }
}


/**
 * @brief ComPort::open
 * @param port "ttyUSBx", "ttySx", "COMx"
 * @param args ["9600","N","8","1"]
 * @return true-成功, false-失败
 */
bool ComPort::open(QString port, QStringList args)
{
    if(port == "" || args.length() != 4) {
        return false;
    }

    QString szSerialPortName = Singleton<SerialPortReMapping>::instance().getSerialPortName(port);

#ifdef Q_OS_LINUX
    serialPortPtr_ = new QextSerialPort(szSerialPortName);
#elif defined (Q_OS_WIN)
    serialPortPtr_ = new QextSerialPort(szSerialPortName);
#endif

    //设置波特率
    QString strBaud = args.at(0);
    serialPortPtr_->setBaudRate((BaudRateType)strBaud.toInt());

    //设置数据位
    QString strDataBits = args.at(1);
    serialPortPtr_->setDataBits((DataBitsType)strDataBits.toInt());

    //设置校验
    QString strParity = args.at(2);
    if(strParity == "N" || strParity == "无校验") {
        serialPortPtr_->setParity(PAR_NONE);
    } else if(strParity == "O" || strParity == "奇校验") {
        serialPortPtr_->setParity(PAR_ODD);
    } else if(strParity == "E" || strParity == "偶校验") {
        serialPortPtr_->setParity(PAR_EVEN);
    } else {
        serialPortPtr_->setParity(PAR_NONE);
    }

    //设置停止位
    QString strStopBits = args.at(3);
    int iStopBits = strStopBits.toFloat() * 10;
    switch(iStopBits) {
        case 10:
            serialPortPtr_->setStopBits(STOP_1);
            break;
        case 15:
#ifdef Q_OS_WIN
            serialPortPtr_->setStopBits(STOP_1_5);
#endif
            break;
        case 20:
            serialPortPtr_->setStopBits(STOP_2);
            break;
        default:
            serialPortPtr_->setStopBits(STOP_1);
            break;
    }

    //设置数据流控制
    serialPortPtr_->setFlowControl(FLOW_OFF);
    //设置延时
    serialPortPtr_->setTimeout(TIME_OUT);

    return serialPortPtr_->open(QIODevice::ReadWrite);
}



/**
 * @brief ComPort::reOpen
 * @return true-成功, false-失败
 */
bool ComPort::reOpen()
{
    if(serialPortPtr_->isOpen()) {
        serialPortPtr_->close();
        return serialPortPtr_->open(QIODevice::ReadWrite);
    }
    return true;
}


int ComPort::read(unsigned char *buf, int len, int ms)
{
    long start;

    QTime time;
    time.start();
    start = time.elapsed();

    while(buf_.size() < len) {
        if(serialPortPtr_->bytesAvailable()) {
            buf_.append(serialPortPtr_->readAll());
        }

        if((time.elapsed() - start) > ms) {
            if(len > buf_.size()) {
                len = buf_.size();
            }
        } else {
            QThread::msleep(20);
        }
    }

    for(int i = 0; i < len; i++) {
        buf[i] = buf_[i];
    }
#if 0
    qDebug() << "read: " << hexToString(buf_.data(), len);
#endif
    buf_.remove(0, len);

    return len;
}

int ComPort::write(unsigned char *buf, int len, int /*ms*/)
{
    int count = 0;
#if 0
    qDebug() << "write: " << hexToString((char *)buf, len);
#endif
    count = serialPortPtr_->write((char*)buf, len);
    serialPortPtr_->flush();
    return count;
}


bool ComPort::close()
{
    buf_.clear();
    if(serialPortPtr_ != NULL) {
        if(serialPortPtr_->isOpen()) {
            serialPortPtr_->close();
            return true;
        }
    }
    return false;
}


TPortType ComPort::getPortType()
{
    return PORT_SERIAL;
}
