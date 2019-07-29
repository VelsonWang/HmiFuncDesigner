#ifndef EDNCRYPT_H
#define EDNCRYPT_H

#include <QString>
#include <qcryptographichash.h>
#include "aes.h"
#include "d3des.h"
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "crc32.h"


enum ED_Type
{
    CRC32,
    SHA1,
    D3DES,
    AES,
    SHA256,
    MD5,
};

class EDncrypt
{
public:
	EDncrypt();
	~EDncrypt();
    static QString Encrypt(const QString &data, int type, const QString &key="");
    static QString Dncrypt(const QString &data, int type, const QString &key="");

private:
    static QString toCRC32(const QString &data);
    static QString toMD5(const QString &data);
    static QString toSHA1(const QString &data);
    static QString toSHA256(const QString &data);
    static QString toD3DES(const QString &data, const QString &key, bool isEncrypt);
    static QString toAES(const QString &data, const QString &key, bool isEncrypt);
};

#endif // EDNCRYPT_H
