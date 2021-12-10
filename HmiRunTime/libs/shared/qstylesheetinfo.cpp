#include "qstylesheetinfo.h"

sheet SheetState[SSID_NUMBER]=
{
    {"unknow","",false},
    {"active","Active",true},
    {"adjoins-item","Adjoins Item",false},
    {"alternate","Alternate",false},
    {"bottom","Bottom",false},
    {"checked","Checked",false},
    {"closable","Closable",true},
    {"closed","Closed",false},
    {"default","Default",true},
    {"disabled","Disabled",false},
    {"edit-focus","Edit Focus",true},
    {"editable","Editable",true},
    {"enabled","Enabled",false},
    {"exclusive","Exclusive",false},
    {"first","First",false},
    {"flat","Flat",true},
    {"floatable","Floatable",true},
    {"focus","Has Focus",true},
    {"has-children","Has Children",true},
    {"has-siblings","Has Siblings",true},
    {"horizontal","Horizontal",false},
    {"hover","Hover",true},
    {"indeterminate","Indeterminate",false},
    {"last","Last",false},
    {"left","Left",false},
    {"maximized","Maximized",false},
    {"middle","Middle",false},
    {"minimized","Minimized",false},
    {"movable","Movable",true},
    {"next-selected","Next Selected",false},
    {"no-frame","No Frame",false},
    {"non-exclusive","Non Exclusive",false},
    {"off","Off",false},
    {"on","On",false},
    {"only-one","Only One",true},
    {"open","Open",false},
    {"pressed","Pressed",true},
    {"previous-selected","Previous Selected",false},
    {"read-only","Read Only",true},
    {"right","Right",false},
    {"selected","Selected",true},
    {"top","Top",false},
    {"unchecked" ,"Unchecked",false},
    {"vertical","Veritical",false},
    {"window","Window",true}
};

sheet StyleSheetInfo::get_info(int id)
{
    if(id<SSID_NUMBER)
    {
        return SheetState[id];
    }
    else
    {
        return sheet();
    }
}
