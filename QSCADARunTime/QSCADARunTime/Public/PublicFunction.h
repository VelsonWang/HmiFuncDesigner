#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H

#include <QString>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>

inline void BytesSetZero(unsigned char* pbuf, int len)
{
    for(int i = 0; i < len; i ++)
    {
        pbuf[i] = 0;
    }
}

// len==2,两个字节调换， len==4,前后16bit调换
inline void SwitchHighLow(unsigned char* pdat, int len)
{
    unsigned char i,j;
    unsigned char tmp;

    for(i = 0; i < len; i ++)
    {
        j = len + i;
        tmp = pdat[i];
        pdat[i] = pdat[j];
        pdat[j] = tmp;
    }
}


//将16进制转换成Ascii码
inline void MakeCodeToAsii(unsigned char * psrc, unsigned char * pdst, int len)
{
    int i;

    if(len == 0)
        return;

    for(i = len - 1; i>= 0; i --)
    {
        unsigned char val = psrc[i];
        unsigned char tmp;

        //处理高4位
        tmp = val >> 4;
        if(tmp <= 9)
            tmp = tmp + '0';
        else
            tmp = tmp - 0x0A + 'A';

        pdst[2 * i] = tmp;

        //处理低4位
        tmp = val & 0x0F;
        if(tmp <= 9)
            tmp = tmp + '0';
        else
            tmp = tmp - 0x0A + 'A';

        pdst[2 * i + 1] = tmp;
    }
}


//将Ascii码转换成16进制
inline void MakeAsiiToCode(unsigned char * psrc, unsigned char * pdst, int len)
{
    int i;

    if(len == 0)
        return;

    for(i = 0; i < len; i ++)
    {
        unsigned char val;
        unsigned char tmp;

        //处理高4位
        tmp = psrc[2 * i];
        if(tmp >= 'A')
            tmp = tmp - 'A' + 0x0A;
        else
            tmp = tmp - '0';

        val = tmp << 4;

        //处理低4位
        tmp = psrc[2 * i + 1];
        if(tmp >= 'A')
            tmp = tmp - 'A' + 0x0A;
        else
            tmp = tmp - '0';

        val |= (tmp & 0x0F);

        pdst[i] = val;
    }
}

inline void ShowBufferHex(QString msg, unsigned char * pbuf, int len)
{
    QString strBuf;
    for(int i=0; i<len; i++)
    {
        QString s;
        s.sprintf("0x%02x ", pbuf[i]);
        strBuf += s;
    }
    msg += strBuf;
    qDebug()<< msg;
}

inline QString hexToString(char *buf, int size)
{
    QString ret = "";
    for(int i=0; i<size; i++){
        ret = ret + QString("%1").arg(buf[i]&0xff, 2, 16, QChar('0')) + " ";
    }
    ret.chop(1);
    return ret.toUpper();
}

inline void delayMs(unsigned int msec)
{
#if 0
    QTime reachTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < reachTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
#else
    QEventLoop eventloop;
    QTimer::singleShot(msec, &eventloop, SLOT(quit()));
    eventloop.exec();
#endif
}


/**
 * @brief lowWord
 * @details Return low word of a 32-bit integer.
 * @param ww quint32 ww (0x00000000..0xFFFFFFFF)
 * @return low word of input (0x0000..0xFFFF)
 */
inline quint16 lowWord(quint32 ww)
{
    return (quint16) ((ww) & 0xFFFF);
}


/**
 * @brief highWord
 * @details Return high word of a 32-bit integer.
 * @param ww quint32 ww (0x00000000..0xFFFFFFFF)
 * @return high word of input (0x0000..0xFFFF)
 */
inline quint16 highWord(quint32 ww)
{
    return (quint16) ((ww) >> 16);
}


/**
 * @brief crc16
 * @details Processor-independent CRC-16 calculation.

    Polynomial: x^16 + x^15 + x^2 + 1 (0xA001)<br>
    Initial value: 0xFFFF

    This CRC is normally used in disk-drive controllers.
 * @param crc quint16 crc (0x0000..0xFFFF)
 * @param a quint8 a (0x00..0xFF)
 * @return calculated CRC (0x0000..0xFFFF)
 */
inline quint16 crc16(quint16 crc, quint8 a)
{
    int i;

    crc ^= a;
    for (i = 0; i < 8; ++i)
    {
        if (crc & 1)
            crc = (crc >> 1) ^ 0xA001;
        else
            crc = (crc >> 1);
    }

    return crc;
}


#endif // PUBLIC_FUNCTION_H
