#ifndef QSTYLESHEETINFO_H
#define QSTYLESHEETINFO_H

#include "sharedlibglobal.h"

#include <QString>
#include <QList>
#include <QVariant>

#include <QStringList>

enum enStyleSheetID
{
    SSID_Unknow,
    SSID_Active,
    SSID_Item,
    SSID_Alternate,
    SSID_Bottom,
    SSID_Checked,
    SSID_Closable,
    SSID_Closed,
    SSID_Default,
    SSID_Disabled,
    SSID_EditFocus,
    SSID_Editable,
    SSID_Enabled,
    SSID_Exclusive,
    SSID_First,
    SSID_Flat,
    SSID_Floatable,
    SSID_Focus,
    SSID_Children,
    SSID_Sibling,
    SSID_Horizontal,
    SSID_Hover,
    SSID_Indeterminate,
    SSID_Last,
    SSID_Left,
    SSID_Maximized,
    SSID_Middle,
    SSID_Minimized,
    SSID_Movable,
    SSID_NextSelected,
    SSID_Frameless,
    SSID_NonExclusive,
    SSID_Off,
    SSID_On,
    SSID_OnlyOne,
    SSID_Open,
    SSID_Pressed,
    SSID_PreviousSelected,
    SSID_ReadOnly,
    SSID_Right,
    SSID_Selected,
    SSID_Top,
    SSID_Unchecked,
    SSID_Vertical,
    SSID_Window,
    SSID_NUMBER
};

struct sheet
{
    QString     m_name;
    QString     m_show_name;
    bool        m_not;
};

struct tagSheetItem
{
    enStyleSheetID  m_item_id;
    QStringList     m_un_used;
};

typedef QMap<int,tagSheetItem> SheetItems;
Q_DECLARE_METATYPE(SheetItems)



class SHAREDLIB_EXPORT StyleSheetInfo
{
public:
    static sheet get_info(int id);
};

#endif // QSTYLESHEETINFO_H
