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
const int JarShapeItemType = QGraphicsItem::UserType + 49;
const int ComboBoxItemType = QGraphicsItem::UserType + 50;

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

