#ifndef DATA_AES_H
#define DATA_AES_H

#include <cstdio>
#include <cstdint>
#include <string>
#include "projdata_global.h"

#define AES_DEFAULT_KEY "JasonWangEasyHMI"

enum class Mode {
    Encrypt = 0,
    Decrypt,
    Unspecified
};


typedef std::string String;
typedef uint8_t state_t[4][4];

#ifndef CBC
#   define CBC 1
#endif
#ifndef ECB
#   define ECB 1
#endif

#ifndef CTR
#   define CTR 1
#endif

#define AES128 1
//#define AES192 1
//#define AES256 1

#define AES_BLOCKLEN 16 //Block length in bytes AES is 128b block only

#if defined(AES256) && (AES256 == 1)
#   define AES_KEYLEN 32
#   define AES_keyExpSize 240
#elif defined(AES192) && (AES192 == 1)
#   define AES_KEYLEN 24
#   define AES_keyExpSize 208
#else
#   define AES_KEYLEN 16   // Key length in bytes
#   define AES_keyExpSize 176
#endif

struct AesCtx {
    uint8_t RoundKey[AES_keyExpSize];
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
    uint8_t Iv[AES_BLOCKLEN];
#endif
};

class Aes
{
private:
    Aes() = default;
    ~Aes() = default;
private:
    static void KeyExpansion(uint8_t*, const uint8_t*);
public:
    static void InitAesCtx(struct AesCtx*, const uint8_t*);
    static void Cipher(state_t*, const uint8_t*);
    static void InvCipher(state_t*, const uint8_t*);
    static void InvShiftRows(state_t*);
    static void InvSubBytes(state_t*);
    static void SubBytes(state_t*);
    static void AddRoundKey(uint8_t, state_t*, const uint8_t*);
    static void InvMixColumns(state_t*);
    static void MixColumns(state_t*);
    static void ShiftRows(state_t*);
    static void XorWithIv(uint8_t*, const uint8_t*);
#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
    static void AESInitCtxIv(struct AesCtx*, const uint8_t*, const uint8_t*);
    static void AESCtxSetIv(struct AesCtx*, const uint8_t*);
#endif

#if defined(ECB) && (ECB == 1)
    // buffer size is exactly AES_BLOCKLEN bytes;
    // you need only AES_init_ctx as IV is not used in ECB
    // NB: ECB is considered insecure for most uses
    static void AesECBEncrypt(const struct AesCtx*, uint8_t*);
    static void AesECBDecrypt(const struct AesCtx*, uint8_t*);
#endif // #if defined(ECB) && (ECB == !)

#if defined(CBC) && (CBC == 1)
    // buffer size MUST be mutile of AES_BLOCKLEN;
    // Suggest https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
    // NOTES: you need to set IV in ctx via AES_init_ctx_iv() or AES_ctx_set_iv()
    //        no IV should ever be reused with the same key
    static void AesCBCEncryptBuffer(struct AesCtx*, uint8_t*, uint32_t);
    static void AesCBCDecryptBuffer(struct AesCtx*, uint8_t*, uint32_t);

#endif // #if defined(CBC) && (CBC == 1)

#if defined(CTR) && (CTR == 1)

    // Same function for encrypting as for decrypting.
    // IV is incremented for every block, and used after encryption as XOR-compliment for output
    // Suggesting https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7 for padding scheme
    // NOTES: you need to set IV in ctx with AES_init_ctx_iv() or AES_ctx_set_iv()
    //        no IV should ever be reused with the same key
    static void AesCTRXcryptBuffer(struct AesCtx*, uint8_t*, uint32_t);

#endif // #if defined(CTR) && (CTR == 1)
};

//----------------------------------------------------------------------------------------

class PROJDATASHARED_EXPORT DataAES
{
private:
    DataAES() = default;
    ~DataAES() = default;

private:
    static uint32_t Byte2ToInt(uint8_t*);
    static void IntToBytes(uint32_t, uint8_t*);

public:
    static int Encrypt(QByteArray &input, QByteArray &output, const QString &key);
    static int Decrypt(QByteArray &input, QByteArray &output, const QString &key);
};

#endif
