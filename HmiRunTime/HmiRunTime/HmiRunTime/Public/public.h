#ifndef PUBLIC_H
#define PUBLIC_H

//////////////////////////////////////////
/// \brief The SaveFormat enum
///
enum SaveFormat {
    Json = 0,
    Binary
};

#define DATA_SAVE_FORMAT    (Json)

//////////////////////////////////////////
/// \brief The eTagType enum
///
enum eTagDataType
{
    TYPE_VARIANT = 0x00,
    TYPE_BOOL = 0x01,
    TYPE_INT8 = 0x02,
    TYPE_UINT8 = 0x03,
    TYPE_INT16 = 0x04,
    TYPE_UINT16 = 0x05,
    TYPE_INT32 = 0x06,
    TYPE_UINT32 = 0x07,
    TYPE_INT64 = 0x08,
    TYPE_UINT64 = 0x09,

    TYPE_FLOAT32 = 0x0A,
    TYPE_FLOAT64 = 0x0B,

    TYPE_BCD8 = 0x0C,
    TYPE_BCD16 = 0x0D,
    TYPE_BCD32 = 0x0E,

    TYPE_ASCII2CHAR = 0x0F,
    TYPE_STRING = 0x10,

    TYPE_BYTES = 0x64,
    TYPE_RESERVED = 255 // 预留
};
typedef enum eTagDataType TTagDataType;

///////////////////////////////////////////
/// \brief The eTagType enum
///
enum eTagType
{
    TYPE_IO = 0,
    TYPE_TMP,
    TYPE_SYSTEM
};
typedef enum eTagType TTagType;

//////////////////////////////////////////
/// \brief The ePermissionType enum
///
enum ePermissionType
{
    READ_WRIE = 0,
    READ,
    WRIE
};
typedef enum ePermissionType TPermissionType;


///////////////////////////////////////////
/// \brief The eAlarmType enum
///
enum eAlarmType
{
    TYPE_ALARM_AI = 0,
    TYPE_ALARM_DI
};
typedef enum eAlarmType TAlarmType;


//////////////////////////////////////////
/// \brief The eArchiveFileMode enum
///
enum eArchiveFileMode
{
    MODE_PERIOD = 0,
    MODE_BIT_VALUE_CHANGED,
    MODE_EQUAL_VALUE
};
typedef enum eArchiveFileMode TArchiveFileMode;

//////////////////////////////////////////
/// \brief The Result enum
///
typedef enum eResult
{
    FAIL = 0,
    SUCCESS
}ResultType;
typedef enum eResult TResult;

#endif // PUBLIC_H
