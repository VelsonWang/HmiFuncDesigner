#ifndef PUBLIC_H
#define PUBLIC_H

////////////////////////////////////////////
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


#endif // PUBLIC_H
