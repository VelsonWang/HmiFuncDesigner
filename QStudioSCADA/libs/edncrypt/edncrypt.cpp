#include "edncrypt.h"

#define KEY24 "Cg4GSmdam5WmtSGGjcAHkbXB"

EDncrypt::EDncrypt()
{

}

EDncrypt::~EDncrypt()
{

}


/**
 * @brief EDncrypt::Encrypt
 * @details 加密函数
 * @param data 要加密的明文
 * @param type 加密类型:MD5,SHA1,SHA256,D3DES,AES
 * @param key 秘钥
 * @return 加密后的密文
 */
QString EDncrypt::Encrypt(const QString &data, int type, const QString &key)
{
	QString ret = "";
	switch(type)
	{
	    case CRC32:
		    ret = toCRC32(data);
		    break;
	    case MD5:
			ret = toMD5(data);
			break;
		case SHA1:
			ret = toSHA1(data);
			break;
		case SHA256:
			ret = toSHA256(data);
			break;
		case D3DES:
            ret = toD3DES(data, key, true);
			break;
		case AES:
            ret = toAES(data, key, true);
			break;
	}
	return ret;
}


/**
 * @brief EDncrypt::Dncrypt
 * @details 解密函数
 * @param data 要解密的密文
 * @param type 解密类型:D3DES,AES
 * @param key 秘钥
 * @return 解密后的明文
 */
QString EDncrypt::Dncrypt(const QString &data, int type, const QString &key)
{
	QString ret = "";
	switch(type)
	{
		case D3DES:
            ret = toD3DES(data, key, false);
			break;
		case AES:
            ret = toAES(data, key, false);
			break;
	}
	return ret;
}


/**
 * @brief CharToQByteArray
 * @details 数据类型转换，char*转QByteArray
 * @param src 源数据
 * @param len 源数据长度
 * @return 转换后的QByteArray值
 */
QByteArray CharToQByteArray(char * src, int len)
{
	QByteArray ba;
	ba.resize(len);
    for(int i=0;i<len;i++) {
		ba[i] = src[i];
	}
	return ba;
}


/**
 * @brief EDncrypt::toCRC32
 * @details CRC32校验函数
 * @param data 要校验的数据
 * @return 校验码
 */
QString EDncrypt::toCRC32(const QString &data)
{
	unsigned int temp;
	QByteArray ba;
	crc32 crc;

    try {
		ba = data.toLatin1();
		char *str = ba.data();
		temp = crc.CRC((unsigned char *)str,ba.size());
    } catch (...) {
		return "0";
	}
	return QString::number(temp);
}


/**
 * @brief EDncrypt::toMD5
 * @details MD5加密函数
 * @param data 要加密的数据
 * @return 加密后的密文
 */
QString EDncrypt::toMD5(const QString &data)
{
	char temp[32]={0};
	QByteArray ba;
	md5 md;

    try {
		ba = data.toLatin1();
		char *str = ba.data();
		md.Encrypt((const unsigned char*)str,strlen(str),(unsigned char*)temp);
        ba = CharToQByteArray(temp, 16);
    } catch (...) {
		return "";
	}
	return ba.toHex();
}


/**
 * @brief EDncrypt::toSHA1
 * @details SHA1加密函数
 * @param data 要加密的数据
 * @return 加密后的密文
 */
QString EDncrypt::toSHA1(const QString &data)
{
	char temp[32]={0};
	QByteArray ba;
	sha1 sha;

    try {
		ba = data.toLatin1();
		char *str = ba.data();
		sha.Encrypt((const unsigned char*)str,strlen(str),(unsigned char*)temp);
        ba = CharToQByteArray(temp,20);
    } catch(...) {
		return "";
	}
	return ba.toHex();
}


/**
 * @brief EDncrypt::toSHA256
 * @details SHA256加密函数
 * @param data 要加密的数据
 * @return 加密后的密文
 */
QString EDncrypt::toSHA256(const QString &data)
{
	char temp[32]={0};
	QByteArray ba;
	sha256 sha;
    try {
		ba = data.toLatin1();
		char *str = ba.data();
		sha.Encrypt((const unsigned char*)str,strlen(str),(unsigned char*)temp);
        ba = CharToQByteArray(temp,32);
    } catch (...) {
		return "";
	}
	return ba.toHex();
}


/**
 * @brief EDncrypt::toAES
 * @details AES加解密函数
 * @param data 要加密或解密的数据
 * @param inkey 秘钥
 * @param isEncrypt 是否为加密 true为加密，false为解密
 * @return 加解密后的数据
 */
QString EDncrypt::toAES(const QString &data, const QString &inkey, bool isEncrypt)
{
	char temp[1024]={0};
	QByteArray ba;
	QString ret="";
    QString key = inkey;

	if(data.length() == 0 || data.length() > 1000)
		return "";

	if(key.length() < 16)
		key = KEY24;

    try {
		ba = key.toLatin1();
		aes aess(16,(unsigned char*)ba.data());

        if(isEncrypt) {
			ba = data.toLatin1();
			char *str = ba.data();
			int len = (strlen(str)+15)/16*16;
            aess.OnAesEncrypt(str, strlen(str), temp);
            ba = CharToQByteArray(temp, len);
			ret= ba.toHex();
        } else {
			if(data.length()%16 != 0)
				return "";

			ba = QByteArray::fromHex(data.toLatin1());
			char *str = ba.data();
            aess.OnAesUncrypt(str, ba.size(), temp);
			ret = QString(QLatin1String(temp));
		}
    } catch (...) {
		return "";
	}
	return ret;
}


/**
 * @brief EDncrypt::toD3DES
 * @details 加解密函数
 * @param data 要加密或解密的数据
 * @param inkey 秘钥
 * @param isEncrypt 是否为加密 true为加密，false为解密
 * @return 加解密后的数据
 */
QString EDncrypt::toD3DES(const QString &data, const QString &inkey, bool isEncrypt)
{
	char temp[1024]={0};
	QByteArray ba;
	QString ret="";
    QString key = inkey;

	if(data.length() == 0 || data.length() > 1000)
		return "";
    if(key.length() < 24) key = KEY24;

    try {
		ba = key.toLatin1();
		d3des des(24,(unsigned char*)ba.data());

        if(isEncrypt) {
			ba = data.toLatin1();
			char *str = ba.data();
			int len = (strlen(str)+8)/8*8;
            des.Encrypt_Field(str, temp, strlen(str));
            ba = CharToQByteArray(temp, len);
			ret= ba.toHex();
        } else {
			if(data.length()%8 != 0)
				return "";
			ba = QByteArray::fromHex(data.toLatin1());
			char *str = ba.data();
            des.Decrypt_Field(str, temp, ba.size());
			ret = QString(QLatin1String(temp));
		}
    } catch (...) {
		return "";
	}
	return ret;
}
