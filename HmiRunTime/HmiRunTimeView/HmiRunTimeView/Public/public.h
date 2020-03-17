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
