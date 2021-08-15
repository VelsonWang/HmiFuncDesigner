#include "qimagehost.h"
#include "../property/qabstractproperty.h"
#include "../qpropertyfactory.h"
#include "../qcommonstruct.h"
#include "../widgets/qimagebox.h"

QImageHost::QImageHost(QAbstractHost *parent) : QFrameHost(parent)
{
    setProperty("need_frame", false);
    setProperty("accept_drop", false);
    setProperty("scaledContents", true);
}

QString QImageHost::getShowName()
{
    return tr("Image");
}

QString QImageHost::getShowGroup()
{
    return tr("Display Widgets");
}

QString QImageHost::getShowIcon()
{
    return ":/images/image.png";
}

void QImageHost::createObject()
{
    m_object = new QImageBox();
    m_object->setObjectName("image");
}

void QImageHost::initProperty()
{
    QFrameHost::initProperty();

    QAbstractProperty* pObj = QPropertyFactory::create_property("Image");
    if(pObj != Q_NULLPTR) {
        pObj->setObjectProperty("name", "ImageFile");
        pObj->setAttribute("show_name", tr("选择图片"));
        pObj->setAttribute("filters", "Images (*.png *.xpm *.jpg)");
        pObj->setAttribute("group", "Attributes");
        //pObj->setAttribute(ATTR_CAN_SAME, true);
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Bool");
    if(pObj != Q_NULLPTR)
    {
        pObj->setObjectProperty("name", "NoScale");
        pObj->setAttribute("show_name", tr("原尺寸显示"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Number");
    if(pObj != Q_NULLPTR)
    {
        pObj->setObjectProperty("name", "BoardWidth");
        pObj->setAttribute("show_name", tr("边框宽度"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    pObj = QPropertyFactory::create_property("Color");
    if(pObj != Q_NULLPTR)
    {
        pObj->setObjectProperty("name", "BoardColor");
        pObj->setAttribute("show_name", tr("边框颜色"));
        pObj->setAttribute("group", "Attributes");
        insertProperty(pObj);
    }

    setPropertyValue("geometry", QRect(0, 0, 80, 80));
    removeProperty("frameShape");
    removeProperty("frameShadow");
}
