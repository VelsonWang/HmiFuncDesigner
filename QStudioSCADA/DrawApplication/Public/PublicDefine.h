#ifndef PUBLICDEFINE_H
#define PUBLICDEFINE_H

#include <QDomDocument>
#include <QGraphicsItem>
#include <QString>


const int LineItemType = QGraphicsItem::UserType + 1;
const int RectItemType = QGraphicsItem::UserType + 2;
const int EllipseItemType = QGraphicsItem::UserType + 3;
const int ArrowItemType = QGraphicsItem::UserType + 4;
const int TextItemType = QGraphicsItem::UserType + 5;
const int GroupItemType = QGraphicsItem::UserType + 6;

const int PushButtonItemType = QGraphicsItem::UserType + 40;
const int InputEditItemType = QGraphicsItem::UserType + 41;
const int PictureItemType = QGraphicsItem::UserType + 42;
const int IndicationLampItemType = QGraphicsItem::UserType + 43;
const int SwitchButtonItemType = QGraphicsItem::UserType + 44;
const int ClockItemType = QGraphicsItem::UserType + 45;
const int MovingTextItemType = QGraphicsItem::UserType + 46;
const int TagTextListItemType = QGraphicsItem::UserType + 47;
const int ValueStickItemType = QGraphicsItem::UserType + 48;


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
    EL_FUNCTION = 20,
    EL_GRAPHPAGE = 21,
    EL_TAG = 22,
    EL_EVENT = 23,
    EL_H_ALIGN = 24,
    EL_V_ALIGN = 25,
    EL_TRANSPARENT_BACKGROUND = 26,
    EL_FONT = 27,
    EL_HIDE_ON_CLICK = 28,
    EL_SHOW_ON_INITIAL = 29,
    EL_FILL_COLOR = 30,
    EL_IS_FILL_COLOR = 31,
    EL_TAG_COLOR_LIST = 32,
    EL_SHOW_SCALE = 33,
    EL_SHOW_CONTENT = 34,
    EL_ENABLE_ON_INITIAL = 35,
    EL_INPUT_PASSWORD = 36,
    EL_ENABLE_EDIT = 37,
    EL_STATE_ON_INITIAL = 38,
    EL_PICTURE1 = 39,
    EL_PICTURE2 = 40,
    EL_TEXT1 = 41,
    EL_TEXT2 = 42,
    EL_BACKGROUND1 = 43,
    EL_BACKGROUND2 = 44,
    EL_PERIOD = 45,
    EL_SHOW_DATE = 46,
    EL_SHOW_WEEK = 47,
    EL_TAG_TEXT_LIST = 48,
    EL_TEXT_MOVE_DIR = 49,
    EL_TEXT_MOVE_CHAR_NUM = 50,
    EL_SCALE_VALUE_MAX = 51,
    EL_SCALE_VALUE_MIN = 52,
    EL_SCALE_NUM = 53,
    EL_FOREGROUND = 54,
    EL_SCALECOLOR = 55,
    EL_SCALE_DIR = 56,
    EL_SCALE_POS = 57,
    EL_SHOW_RULER = 58,



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

