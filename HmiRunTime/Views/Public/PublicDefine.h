#ifndef PUBLICDEFINE_H
#define PUBLICDEFINE_H

#include <QDomDocument>
#include <QGraphicsItem>
#include <QString>


const int LineItemType = QGraphicsItem::UserType + 1;
const int RectItemType = QGraphicsItem::UserType + 2;
const int EllipseItemType = QGraphicsItem::UserType + 3;
const int ArrowItemType = QGraphicsItem::UserType + 4;
const int TextItemType = QGraphicsItem::UserType + 14;
const int GroupItemType = QGraphicsItem::UserType + 16;
const int PolygonItemType = QGraphicsItem::UserType + 17;

const int PushButtonItemType = QGraphicsItem::UserType + 40;
const int InputEditItemType = QGraphicsItem::UserType + 41;
const int PictureItemType = QGraphicsItem::UserType + 42;
const int IndicationLampItemType = QGraphicsItem::UserType + 43;
const int SwitchButtonItemType = QGraphicsItem::UserType + 44;
const int ClockItemType = QGraphicsItem::UserType + 45;
const int MovingTextItemType = QGraphicsItem::UserType + 46;
const int TagTextListItemType = QGraphicsItem::UserType + 47;
const int ValueStickItemType = QGraphicsItem::UserType + 48;
const int JarShapeItemType = QGraphicsItem::UserType + 49;
const int ComboBoxItemType = QGraphicsItem::UserType + 50;
const int AnalogClockItemType = QGraphicsItem::UserType + 51;

enum {
    EL_X = 0, //coord X
    EL_Y = 1, //coord Y
    EL_WIDTH = 2,
    EL_HEIGHT = 3,
    EL_TOOLTIP = 4,
    EL_ID = 5, //unique id
    EL_BACKGROUND = 6,
    EL_SIGN_BACKGROUND = 7,
    EL_BORDER_COLOR = 8,
    EL_BORDER_WIDTH = 9,
    EL_BORDER_STYLE = 10,
    EL_ANGLE = 11,
    EL_Z_VALUE = 12,
    EL_LINE_WIDTH = 13,
    EL_TEXT = 14,
    EL_FONT_FAMILY = 15,
    EL_FONT_SIZE = 16,
    EL_FONT_COLOR = 17,
    EL_GRAPHPAGE_LINK = 18, //transition to screen
    EL_FILE = 19,




    EL_PROPERTY_EMPTY = 255
};

enum {
    GRAPHPAGE_ID,
    GRAPHPAGE_BACKGROUND,
    GRAPHPAGE_WIDTH,
    GRAPHPAGE_HEIGHT
};

enum ComponentType {
    NEW_PROJECT,
    NEW_GRAPHPAGE
};

enum SaveGraphPageType {
    SAVE_XML,
    SAVE_BINARY
};



#endif // IELEMENT_H

