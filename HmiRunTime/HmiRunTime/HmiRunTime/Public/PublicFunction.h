#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H

#include <QString>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <QByteArray>

inline void BytesSetZero(unsigned char* pbuf, int len)
{
    for(int i = 0; i < len; i ++) {
        pbuf[i] = 0;
    }
}

// len==2,两个字节调换， len==4,前后16bit调换
inline void SwitchHighLow(unsigned char* pdat, int len)
{
    unsigned char i, j;
    unsigned char tmp;

    for(i = 0; i < len; i ++) {
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

    if(len == 0) {
        return;
    }

    for(i = len - 1; i >= 0; i --) {
        unsigned char val = psrc[i];
        unsigned char tmp;

        //处理高4位
        tmp = val >> 4;
        if(tmp <= 9) {
            tmp = tmp + '0';
        } else {
            tmp = tmp - 0x0A + 'A';
        }

        pdst[2 * i] = tmp;

        //处理低4位
        tmp = val & 0x0F;
        if(tmp <= 9) {
            tmp = tmp + '0';
        } else {
            tmp = tmp - 0x0A + 'A';
        }

        pdst[2 * i + 1] = tmp;
    }
}


//将Ascii码转换成16进制
inline void MakeAsiiToCode(unsigned char * psrc, unsigned char * pdst, int len)
{
    int i;

    if(len == 0) {
        return;
    }

    for(i = 0; i < len; i ++) {
        unsigned char val;
        unsigned char tmp;

        //处理高4位
        tmp = psrc[2 * i];
        if(tmp >= 'A') {
            tmp = tmp - 'A' + 0x0A;
        } else {
            tmp = tmp - '0';
        }

        val = tmp << 4;

        //处理低4位
        tmp = psrc[2 * i + 1];
        if(tmp >= 'A') {
            tmp = tmp - 'A' + 0x0A;
        } else {
            tmp = tmp - '0';
        }

        val |= (tmp & 0x0F);

        pdst[i] = val;
    }
}

inline void ShowBufferHex(QString msg, unsigned char * pbuf, int len)
{
    QString strBuf;
    for(int i = 0; i < len; i++) {
        QString s;
        s.sprintf("0x%02x ", pbuf[i]);
        strBuf += s;
    }
    msg += strBuf;
    qDebug() << msg;
}

inline QString hexToString(char *buf, int size)
{
    QString ret = "";
    for(int i = 0; i < size; i++) {
        ret = ret + QString("%1").arg(buf[i] & 0xff, 2, 16, QChar('0')) + " ";
    }
    ret.chop(1);
    return ret.toUpper();
}

inline void delayMs(unsigned int msec)
{
#if 0
    QTime reachTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < reachTime) {
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
    for (i = 0; i < 8; ++i) {
        if (crc & 1) {
            crc = (crc >> 1) ^ 0xA001;
        } else {
            crc = (crc >> 1);
        }
    }

    return crc;
}

inline quint8 byteSwitchHigh4bitLow4bit(quint8 dat)
{
    quint8 tmp = ((dat & 0xF0) >> 4) | ((dat & 0x0F) << 4);
    return tmp;
}

inline QByteArray boolToBytes(bool val)
{
    char ch = val ? 1 : 0;
    QByteArray ba;
    ba.append(ch);
    return ba;
}

inline bool bytesToBool(QByteArray val)
{
    char ch = 0;
    if(val.size() >= 1) {
        ch = val.at(0);
    }
    return (ch != 0);
}

inline QByteArray int8ToBytes(qint8 val)
{
    QByteArray ba;
    ba.append(val);
    return ba;
}

inline qint8 bytesToInt8(QByteArray val)
{
    if(val.size() >= 1) {
        return val.at(0);
    }
    return 0;
}

inline QByteArray uint8ToBytes(quint8 val)
{
    QByteArray ba;
    ba.append((char)val);
    return ba;
}

inline quint8 bytesToUint8(QByteArray val)
{
    if(val.size() >= 1) {
        quint8 v = (quint8)val.at(0);
        return v;
    }
    return 0;
}

inline QByteArray int16ToBytes(qint16 val)
{
    union data {
        qint16 val;
        quint8 byte[2];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);

    return ba;
}

inline qint16 bytesToInt16(QByteArray val)
{
    if(val.size() >= 2) {
        qint16 v = val.at(1) * 2 ^ 8 + val.at(0);
        return v;
    }
    return 0;
}

inline QByteArray uint16ToBytes(quint16 val)
{
    union data {
        quint16 val;
        quint8 byte[2];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);

    return ba;
}

inline quint16 bytesToUint16(QByteArray val)
{
    if(val.size() >= 2) {
        quint16 v = val.at(1) * 2 ^ 8 + val.at(0);
        return v;
    }
    return 0;
}

inline QByteArray int32ToBytes(qint32 val)
{
    union data {
        qint32 val;
        quint8 byte[4];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);
    ba.append((char)dat.byte[2]);
    ba.append((char)dat.byte[3]);

    return ba;
}

inline qint32 bytesToInt32(QByteArray val)
{
    if(val.size() >= 4) {
        qint32 v = val.at(3) * 2 ^ 24 + val.at(2) * 2 ^ 16 + val.at(1) * 2 ^ 8 + val.at(0);
        return v;
    }
    return 0;
}

inline QByteArray uint32ToBytes(quint32 val)
{
    union data {
        quint32 val;
        quint8 byte[4];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);
    ba.append((char)dat.byte[2]);
    ba.append((char)dat.byte[3]);

    return ba;
}

inline quint32 bytesToUint32(QByteArray val)
{
    if(val.size() >= 4) {
        quint32 v = val.at(3) * 2 ^ 24 + val.at(2) * 2 ^ 16 + val.at(1) * 2 ^ 8 + val.at(0);
        return v;
    }
    return 0;
}

inline QByteArray int64ToBytes(qint64 val)
{
    union data {
        qint64 val;
        quint8 byte[8];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);
    ba.append((char)dat.byte[2]);
    ba.append((char)dat.byte[3]);
    ba.append((char)dat.byte[4]);
    ba.append((char)dat.byte[5]);
    ba.append((char)dat.byte[6]);
    ba.append((char)dat.byte[7]);

    return ba;
}

inline qint64 bytesToInt64(QByteArray val)
{
    if(val.size() >= 8) {
        qint64 v = val.at(7) * 2 ^ 56 + val.at(6) * 2 ^ 48 + val.at(5) * 2 ^ 40 + val.at(4) * 2 ^ 32 +
                   val.at(3) * 2 ^ 24 + val.at(2) * 2 ^ 16 + val.at(1) * 2 ^ 8 + val.at(0);
        return v;
    }
    return 0;
}

inline QByteArray uint64ToBytes(quint64 val)
{
    union data {
        quint64 val;
        quint8 byte[8];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);
    ba.append((char)dat.byte[2]);
    ba.append((char)dat.byte[3]);
    ba.append((char)dat.byte[4]);
    ba.append((char)dat.byte[5]);
    ba.append((char)dat.byte[6]);
    ba.append((char)dat.byte[7]);

    return ba;
}

inline quint64 bytesToUint64(QByteArray val)
{
    if(val.size() >= 8) {
        quint64 v = val.at(7) * 2 ^ 56 + val.at(6) * 2 ^ 48 + val.at(5) * 2 ^ 40 + val.at(4) * 2 ^ 32 +
                    val.at(3) * 2 ^ 24 + val.at(2) * 2 ^ 16 + val.at(1) * 2 ^ 8 + val.at(0);
        return v;
    }
    return 0;
}

inline QByteArray float32ToBytes(float val)
{
    union data {
        float val;
        quint8 byte[4];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);
    ba.append((char)dat.byte[2]);
    ba.append((char)dat.byte[3]);

    return ba;
}

inline float bytesToFloat32(QByteArray val)
{
    if(val.size() >= 4) {
        union data {
            float val;
            quint8 byte[4];
        };
        union data dat;
        dat.byte[0] = (quint8)val.at(0);
        dat.byte[1] = (quint8)val.at(1);
        dat.byte[2] = (quint8)val.at(2);
        dat.byte[3] = (quint8)val.at(3);
        return dat.val;
    }
    return 0;
}


inline QByteArray float64ToBytes(double val)
{
    union data {
        double val;
        quint8 byte[8];
    };
    union data dat;

    dat.val = val;

    QByteArray ba;
    ba.append((char)dat.byte[0]);
    ba.append((char)dat.byte[1]);
    ba.append((char)dat.byte[2]);
    ba.append((char)dat.byte[3]);
    ba.append((char)dat.byte[4]);
    ba.append((char)dat.byte[5]);
    ba.append((char)dat.byte[6]);
    ba.append((char)dat.byte[7]);

    return ba;
}

inline double bytesToFloat64(QByteArray val)
{
    if(val.size() >= 8) {
        union data {
            double val;
            quint8 byte[8];
        };
        union data dat;
        dat.byte[0] = (quint8)val.at(0);
        dat.byte[1] = (quint8)val.at(1);
        dat.byte[2] = (quint8)val.at(2);
        dat.byte[3] = (quint8)val.at(3);
        dat.byte[4] = (quint8)val.at(4);
        dat.byte[5] = (quint8)val.at(5);
        dat.byte[6] = (quint8)val.at(6);
        dat.byte[7] = (quint8)val.at(7);
        return dat.val;
    }
    return 0;
}

inline QByteArray bcd8ToBytes(quint8 val)
{
    return uint8ToBytes(val);
}

inline quint8 bytesToBcd8(QByteArray val)
{
    return bytesToUint8(val);
}

inline QByteArray bcd16ToBytes(quint16 val)
{
    return uint16ToBytes(val);
}

inline quint16 bytesToBcd16(QByteArray val)
{
    return bytesToUint16(val);
}

inline QByteArray bcd32ToBytes(quint32 val)
{
    return uint32ToBytes(val);
}

inline quint32 bytesToBcd32(QByteArray val)
{
    return bytesToUint32(val);
}

#endif // PUBLIC_FUNCTION_H
