#include "ElementSwitchButton.h"
#include "PubTool.h"
#include "ProjectData.h"
#include "DrawListUtils.h"
#include "ElementIDHelper.h"
#include "Helper.h"
#include "XMLObject.h"
#include "ProjectData.h"
#include "variantmanager.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include "variantmanager.h"
#include "editbasicpropertydialog.h"

int ElementSwitchButton::iLastIndex_ = 1;

ElementSwitchButton::ElementSwitchButton(const QString &szProjPath,
                                         const QString &szProjName,
                                         QtVariantPropertyManager *propertyMgr)
    : Element(szProjPath, szProjName, propertyMgr)
{
    elementId = QString(tr("SwitchButton_%1").arg(iLastIndex_, 4, 10, QChar('0')));
    iLastIndex_++;
    internalElementType = tr("SwitchButton");
    elementIcon = QIcon(":/images/SwitchButton.png");
    showContent_ = tr("文本");
    bShowContentText_ = true;
    szHAlign_ = tr("居中对齐");
    szVAlign_ = tr("居中对齐");
    font_ = QFont("宋体", 12);
    init();
    elementWidth = 100;
    elementHeight = 40;
    resetBackgroundColor_ = QColor(240, 240, 240);
    setBackgroundColor_ = QColor(240, 240, 240);
    signBackgroundColor = QColor(Qt::black);
    borderWidth = 4;
    borderColor = QColor(112, 112, 112);
    enableOnInitial_ = true;
    showOnInitial_ = true;
    transparent_ = false;
    showNoScale_ = false;
    resetPictureFile_ = "";
    setPictureFile_ = "";
    resetText_ = tr("关");
    setText_ = tr("开");
    DrawListUtils::setProjectPath(szProjectPath_);
    ElementIDHelper::setProjectPath(szProjectPath_);

    if(ProjectData::getInstance()->getDBPath() == "")
        ProjectData::getInstance()->createOrOpenProjectData(szProjectPath_, szProjectName_);

    createPropertyList();
    updatePropertyModel();
}

void ElementSwitchButton::regenerateElementId()
{
    elementId = QString(tr("SwitchButton_%1").arg(iLastIndex_ - 1, 4, 10, QChar('0')));
    this->updatePropertyModel();
}

/**
 * @brief ElementSwitchButton::release
 * @details 释放占用的资源
 */
void ElementSwitchButton::release()
{
    if(resetPictureFile_ != "") {
        PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
        picResMgr_.del(ProjectData::getInstance()->dbData_, resetPictureFile_);
    }
    if(setPictureFile_ != "") {
        PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
        picResMgr_.del(ProjectData::getInstance()->dbData_, setPictureFile_);
    }

    ProjectData::releaseInstance();
}

QRectF ElementSwitchButton::boundingRect() const
{
    qreal extra = 5;
    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra, -extra, extra, extra);
}


void ElementSwitchButton::createPropertyList()
{
    propList.clear();
    clearProperties();

    QtVariantProperty *property = Q_NULLPTR;

    // ID
    property = variantPropertyManager_->addProperty(QVariant::String, tr("ID"));
    property->setAttribute(QLatin1String("readOnly"), true);
    addProperty(property, QLatin1String("id"));

    // 选择变量
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("选择变量"));
    tagNames_.clear();
    ProjectData::getInstance()->getAllTagName(tagNames_);
    if(tagNames_.size() > 0) szTagSelected_ = tagNames_.at(0);
    property->setAttribute(QLatin1String("enumNames"), tagNames_);
    addProperty(property, QLatin1String("tag"));

    // 选择功能
    property = variantPropertyManager_->addProperty(VariantManager::functionTypeId(), tr("功能操作"));
    QStringList listEvents;
    getSupportEvents(listEvents);
    property->setAttribute(QLatin1String("supportevents"), listEvents.join("|"));
    addProperty(property, QLatin1String("functions"));

    // 初始状态
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("初始状态"));
    addProperty(property, QLatin1String("stateOnInitial"));

    // 显示内容
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("显示内容"));
    contents_.clear();
    contents_ << tr("文本") << tr("图片");
    property->setAttribute(QLatin1String("enumNames"), contents_);
    addProperty(property, QLatin1String("showContent"));

    // 复位图片
    property = variantPropertyManager_->addProperty(VariantManager::filePathTypeId(), tr("选择复位图片"));
    property->setAttribute(QLatin1String("filter"), "image files (*.png *.jpg *.jpeg *.bmp)");
    addProperty(property, QLatin1String("resetPicture"), !bShowContentText_);

    // 置位图片
    property = variantPropertyManager_->addProperty(VariantManager::filePathTypeId(), tr("选择置位图片"));
    property->setAttribute(QLatin1String("filter"), "image files (*.png *.jpg *.jpeg *.bmp)");
    addProperty(property, QLatin1String("setPicture"), !bShowContentText_);

    // 原尺寸显示
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("原尺寸显示"));
    addProperty(property, QLatin1String("showNoScale"), !bShowContentText_);

    // 复位显示文本
    property = variantPropertyManager_->addProperty(QVariant::String, tr("关时文本"));
    addProperty(property, QLatin1String("resetText"), bShowContentText_);

    // 置位显示文本
    property = variantPropertyManager_->addProperty(QVariant::String, tr("开时文本"));
    addProperty(property, QLatin1String("setText"), bShowContentText_);

    // 字体
    property = variantPropertyManager_->addProperty(QVariant::Font, tr("字体"));
    addProperty(property, QLatin1String("font"), bShowContentText_);

    // 文本颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("文本颜色"));
    addProperty(property, QLatin1String("textColor"), bShowContentText_);

    // 水平对齐
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("水平对齐"));
    hAlignList_.clear();
    hAlignList_ << tr("左对齐") << tr("居中对齐") << tr("右对齐");
    property->setAttribute(QLatin1String("enumNames"), hAlignList_);
    addProperty(property, QLatin1String("hAlign"), bShowContentText_);

    // 垂直对齐
    property = variantPropertyManager_->addProperty(QtVariantPropertyManager::enumTypeId(), tr("垂直对齐"));
    vAlignList_.clear();
    vAlignList_ << tr("上对齐") << tr("居中对齐") << tr("下对齐");
    property->setAttribute(QLatin1String("enumNames"), vAlignList_);
    addProperty(property, QLatin1String("vAlign"), bShowContentText_);

    // 复位按钮背景颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("关背景颜色"));
    addProperty(property, QLatin1String("resetBackground"), bShowContentText_);

    // 置位按钮背景颜色
    property = variantPropertyManager_->addProperty(QVariant::Color, tr("开背景颜色"));
    addProperty(property, QLatin1String("setBackground"), bShowContentText_);

    // 透明
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("透明显示"));
    addProperty(property, QLatin1String("transparent"));

    // 初始有效性
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("初始有效性"));
    addProperty(property, QLatin1String("enableOnInitial"));

    // 初始可见性
    property = variantPropertyManager_->addProperty(QVariant::Bool, tr("初始可见性"));
    addProperty(property, QLatin1String("showOnInitial"));

    // 坐标 X
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("坐标 X"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("xCoord"));

    // 坐标 Y
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("坐标 Y"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("yCoord"));

    // Z 值
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("Z 值"));
    property->setAttribute(QLatin1String("minimum"), -1000);
    property->setAttribute(QLatin1String("maximum"), 1000);
    addProperty(property, QLatin1String("zValue"));

    // 宽度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("宽度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("width"));

    // 高度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("高度"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 5000);
    addProperty(property, QLatin1String("height"));

    // 旋转角度
    property = variantPropertyManager_->addProperty(QVariant::Int, tr("角度"));
    property->setAttribute(QLatin1String("minimum"), -360);
    property->setAttribute(QLatin1String("maximum"), 360);
    addProperty(property, QLatin1String("angle"));
}

void ElementSwitchButton::updateElementProperty(QtProperty *property, const QVariant &value)
{
    QString id = propertyToId_[property];

    if (id == QLatin1String("id")) {
        elementId = value.toString();
    } else if (id == QLatin1String("tag")) {
        szTagSelected_ = tagNames_.at(value.toInt());
    } else if (id == QLatin1String("functions")) {
        QString szFuncs = value.toString();
        funcs_ = szFuncs.split('|');
    } else if (id == QLatin1String("stateOnInitial")) {
        stateOnInitial_ = value.toBool();
    } else if (id == QLatin1String("showContent")) {
        QString szShowContent = contents_.at(value.toInt());
        if(showContent_ != szShowContent) {
            showContent_ = szShowContent;
            bShowContentText_ = true;
            if(showContent_ == tr("图片")) {
                bShowContentText_ = false;
            }
            updateBoundingElement();
            // 属性集发生改变需要更新属性表
            updatePropertyEditor();
        }
    } else if (id == QLatin1String("resetPicture")) {
        QString szTmpName = value.toString();
        QFileInfo infoSrc(szTmpName);
        if(infoSrc.exists()) {
            QString picturePath = getProjectPath() + "/Pictures";
            QDir dir(picturePath);
            if(!dir.exists())
                dir.mkpath(picturePath);
            QString fileDes = picturePath + "/" + infoSrc.fileName();
            QFileInfo infoDes(fileDes);
            PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
            if(resetPictureFile_ != "" && resetPictureFile_ != infoSrc.fileName()) {
                picResMgr_.del(ProjectData::getInstance()->dbData_, resetPictureFile_);
            }
            if(!infoDes.exists()) {
                QFile::copy(szTmpName, fileDes);
            }
            resetPictureFile_ = infoSrc.fileName();
            picResMgr_.add(ProjectData::getInstance()->dbData_, resetPictureFile_);
            updatePropertyModel();
        }
    } else if (id == QLatin1String("setPicture")) {
        QString szTmpName = value.toString();
        QFileInfo infoSrc(szTmpName);
        if(infoSrc.exists()) {
            QString picturePath = getProjectPath() + "/Pictures";
            QDir dir(picturePath);
            if(!dir.exists())
                dir.mkpath(picturePath);
            QString fileDes = picturePath + "/" + infoSrc.fileName();
            QFileInfo infoDes(fileDes);
            PictureResourceManager &picResMgr_ = ProjectData::getInstance()->pictureResourceMgr_;
            if(setPictureFile_ != "" && setPictureFile_ != infoSrc.fileName()) {
                picResMgr_.del(ProjectData::getInstance()->dbData_, setPictureFile_);
            }
            if(!infoDes.exists()) {
                QFile::copy(szTmpName, fileDes);
            }
            setPictureFile_ = infoSrc.fileName();
            picResMgr_.add(ProjectData::getInstance()->dbData_, setPictureFile_);
            updatePropertyModel();
        }
    } else if (id == QLatin1String("showNoScale")) {
        showNoScale_ = value.toBool();
    } else if (id == QLatin1String("resetText")) {
        resetText_ = value.toString();
    } else if (id == QLatin1String("setText")) {
        setText_ = value.toString();
    } else if (id == QLatin1String("font")) {
        font_ = value.value<QFont>();
    } else if (id == QLatin1String("textColor")) {
        textColor = value.value<QColor>();
    } else if (id == QLatin1String("hAlign")) {
        szHAlign_ = hAlignList_.at(value.toInt());
    } else if (id == QLatin1String("vAlign")) {
        szVAlign_ = vAlignList_.at(value.toInt());
    } else if (id == QLatin1String("resetBackground")) {
        resetBackgroundColor_ = value.value<QColor>();
    } else if (id == QLatin1String("setBackground")) {
        setBackgroundColor_ = value.value<QColor>();
    } else if (id == QLatin1String("transparent")) {
        transparent_ = value.toBool();
    } else if (id == QLatin1String("enableOnInitial")) {
        enableOnInitial_ = value.toBool();
    } else if (id == QLatin1String("showOnInitial")) {
        showOnInitial_ = value.toBool();
    } else if (id == QLatin1String("xCoord")) {
        elementXPos = value.toInt();
        setElementXPos(elementXPos);
    } else if (id == QLatin1String("yCoord")) {
        elementYPos = value.toInt();
        setElementYPos(elementYPos);
    } else if (id == QLatin1String("zValue")) {
        elementZValue = value.toInt();
        setZValue(elementZValue);
    } else if (id == QLatin1String("width")) {
        elementWidth = value.toInt();
        updateBoundingElement();
    } else if (id == QLatin1String("height")) {
        elementHeight = value.toInt();
        updateBoundingElement();
    } else if (id == QLatin1String("angle")) {
        elemAngle = value.toInt();
        setAngle(elemAngle);
    }

    scene()->update();
    update();
}

void ElementSwitchButton::updatePropertyModel()
{
    QtVariantProperty *property = Q_NULLPTR;

    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        property->setValue(elementId);
    }

    property = idToProperty_[QLatin1String("tag")];
    if(property != Q_NULLPTR) {
        property->setValue(tagNames_.indexOf(szTagSelected_));
    }

    property = idToProperty_[QLatin1String("functions")];
    if(property != Q_NULLPTR) {
        property->setValue(funcs_.join('|'));
    }

    property = idToProperty_[QLatin1String("stateOnInitial")];
    if(property != Q_NULLPTR) {
        property->setValue(stateOnInitial_);
    }

    property = idToProperty_[QLatin1String("showContent")];
    if(property != Q_NULLPTR) {
        property->setValue(contents_.indexOf(showContent_));
    }

    property = idToProperty_[QLatin1String("resetPicture")];
    if(property != Q_NULLPTR) {
        property->setValue(resetPictureFile_);
    }

    property = idToProperty_[QLatin1String("setPicture")];
    if(property != Q_NULLPTR) {
        property->setValue(setPictureFile_);
    }

    property = idToProperty_[QLatin1String("showNoScale")];
    if(property != Q_NULLPTR) {
        property->setValue(showNoScale_);
    }

    property = idToProperty_[QLatin1String("resetText")];
    if(property != Q_NULLPTR) {
        property->setValue(resetText_);
    }

    property = idToProperty_[QLatin1String("setText")];
    if(property != Q_NULLPTR) {
        property->setValue(setText_);
    }

    property = idToProperty_[QLatin1String("font")];
    if(property != Q_NULLPTR) {
        property->setValue(font_);
    }

    property = idToProperty_[QLatin1String("textColor")];
    if(property != Q_NULLPTR) {
        property->setValue(textColor);
    }

    property = idToProperty_[QLatin1String("hAlign")];
    if(property != Q_NULLPTR) {
        property->setValue(hAlignList_.indexOf(szHAlign_));
    }

    property = idToProperty_[QLatin1String("vAlign")];
    if(property != Q_NULLPTR) {
        property->setValue(vAlignList_.indexOf(szVAlign_));
    }

    property = idToProperty_[QLatin1String("resetBackground")];
    if(property != Q_NULLPTR) {
        property->setValue(resetBackgroundColor_);
    }

    property = idToProperty_[QLatin1String("setBackground")];
    if(property != Q_NULLPTR) {
        property->setValue(setBackgroundColor_);
    }

    property = idToProperty_[QLatin1String("transparent")];
    if(property != Q_NULLPTR) {
        property->setValue(transparent_);
    }

    property = idToProperty_[QLatin1String("enableOnInitial")];
    if(property != Q_NULLPTR) {
        property->setValue(enableOnInitial_);
    }

    property = idToProperty_[QLatin1String("showOnInitial")];
    if(property != Q_NULLPTR) {
        property->setValue(showOnInitial_);
    }

    property = idToProperty_[QLatin1String("xCoord")];
    if(property != Q_NULLPTR) {
        property->setValue(elementXPos);
    }

    property = idToProperty_[QLatin1String("yCoord")];
    if(property != Q_NULLPTR) {
        property->setValue(elementYPos);
    }

    property = idToProperty_[QLatin1String("zValue")];
    if(property != Q_NULLPTR) {
        property->setValue(elementZValue);
    }

    property = idToProperty_[QLatin1String("width")];
    if(property != Q_NULLPTR) {
        property->setValue(elementWidth);
    }

    property = idToProperty_[QLatin1String("height")];
    if(property != Q_NULLPTR) {
        property->setValue(elementHeight);
    }

    property = idToProperty_[QLatin1String("angle")];
    if(property != Q_NULLPTR) {
        property->setValue(elemAngle);
    }
}

void ElementSwitchButton::reloadPropertyList()
{
    QtVariantProperty *property = Q_NULLPTR;
    propList.clear();

    // ID
    property = idToProperty_[QLatin1String("id")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 选择变量
    property = idToProperty_[QLatin1String("tag")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 选择功能
    property = idToProperty_[QLatin1String("functions")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 初始状态
    property = idToProperty_[QLatin1String("stateOnInitial")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 显示内容
    property = idToProperty_[QLatin1String("showContent")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    if(!bShowContentText_) { // 显示内容图片
        // 复位图片
        property = idToProperty_[QLatin1String("resetPicture")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 置位图片
        property = idToProperty_[QLatin1String("setPicture")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 原尺寸显示
        property = idToProperty_[QLatin1String("showNoScale")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }
    } else { // 显示内容文本
        // 复位显示文本
        property = idToProperty_[QLatin1String("resetText")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 置位显示文本
        property = idToProperty_[QLatin1String("setText")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 字体
        property = idToProperty_[QLatin1String("font")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 文本颜色
        property = idToProperty_[QLatin1String("textColor")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 水平对齐
        property = idToProperty_[QLatin1String("hAlign")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 垂直对齐
        property = idToProperty_[QLatin1String("vAlign")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 关背景颜色
        property = idToProperty_[QLatin1String("resetBackground")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }

        // 开背景颜色
        property = idToProperty_[QLatin1String("setBackground")];
        if(property != Q_NULLPTR) {
            propList.append(property);
        }
    }

    // 透明
    property = idToProperty_[QLatin1String("transparent")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 初始有效性
    property = idToProperty_[QLatin1String("enableOnInitial")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 初始可见性
    property = idToProperty_[QLatin1String("showOnInitial")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 坐标 X
    property = idToProperty_[QLatin1String("xCoord")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    //坐标 Y
    property = idToProperty_[QLatin1String("yCoord")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // Z 值
    property = idToProperty_[QLatin1String("zValue")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 宽度
    property = idToProperty_[QLatin1String("width")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 高度
    property = idToProperty_[QLatin1String("height")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }

    // 旋转角度
    property = idToProperty_[QLatin1String("angle")];
    if(property != Q_NULLPTR) {
        propList.append(property);
    }
}

/**
 * @brief ElementSwitchButton::updatePropertyEditor
 * @details 更新属性表
 */
void ElementSwitchButton::updatePropertyEditor()
{
    VariantManager *pVariantManager = dynamic_cast<VariantManager *>(variantPropertyManager_);
    if(pVariantManager != Q_NULLPTR) {
        QtTreePropertyBrowser *pPropertyEditor = pVariantManager->getPropertyEditor();
        if(pPropertyEditor != Q_NULLPTR) {
            pPropertyEditor->clear();
            this->updatePropertyModel();
            reloadPropertyList();
            QListIterator<QtProperty*> iter(this->getPropertyList());
            while (iter.hasNext()) {
                pPropertyEditor->addProperty(iter.next());
            }
        }
    }
}

void ElementSwitchButton::setClickPosition(QPointF position)
{
    prepareGeometryChange();
    elementXPos = static_cast<int>(position.x());
    elementYPos = static_cast<int>(position.y());
    setX(elementXPos);
    setY(elementYPos);
    elementRect.setRect(0, 0, elementWidth, elementHeight);
    updatePropertyModel();
}

void ElementSwitchButton::updateBoundingElement()
{
    elementRect.setRect(0, 0, elementWidth, elementHeight);
}

void ElementSwitchButton::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    drawSwitchButton(painter);

    // 绘制选中状态
    paintSelected(painter, 1);
}

void ElementSwitchButton::drawSwitchButton(QPainter *painter)
{
    QRect rect(static_cast<int>(elementRect.x()),
               static_cast<int>(elementRect.y()),
               static_cast<int>(elementRect.width()),
               static_cast<int>(elementRect.height()));

    if(transparent_) {
        painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);
    } else { 
        if(bShowContentText_) { // 文本+背景
            for(int i=0; i<borderWidth; i++) {
                PubTool::DrawFrameRect(painter, rect, borderColor);
                if(i<borderWidth/2) rect.adjust(1, 1, -1, -1);
                else rect.adjust(1, 1, 0, 0);
            }

            PubTool::DrawFrameRect(painter, rect, QColor(252, 252, 252));
            rect.adjust(1, 1, -1, -1);
            rect.adjust(-1, -1, 0, 0);

            QString szElementText = QString();
            QColor backgroundColor = QColor();

            if(stateOnInitial_) {
                szElementText = setText_;
                backgroundColor = setBackgroundColor_;
            } else {
                szElementText = resetText_;
                backgroundColor = resetBackgroundColor_;
            }

            PubTool::FillFullRect(painter, rect, backgroundColor);
            painter->setPen(textColor);
            painter->setBrush(Qt::NoBrush);
            painter->setFont(font_);

            int hFlags = Qt::AlignLeft;
            if(szHAlign_ == tr("左对齐")) {
                hFlags = Qt::AlignLeft;
            } else if(szHAlign_ == tr("居中对齐")) {
                hFlags = Qt::AlignHCenter;
            } else if(szHAlign_ == tr("右对齐")) {
                hFlags = Qt::AlignRight;
            }

            int vFlags = Qt::AlignVCenter;
            if(szVAlign_ == tr("上对齐")) {
                vFlags = Qt::AlignTop;
            } else if(szVAlign_ == tr("居中对齐")) {
                vFlags = Qt::AlignVCenter;
            } else if(szVAlign_ == tr("下对齐")) {
                vFlags = Qt::AlignBottom;
            }

            QRectF rect(elementRect.toRect());
            QRectF textRect = rect.normalized().adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth);
            painter->drawText(textRect, hFlags|vFlags, szElementText);
        } else { // 图片
            QString szPictureFile = QString();
            if(stateOnInitial_) {
                szPictureFile = setPictureFile_;
            } else {
                szPictureFile = resetPictureFile_;
            }

            if(szPictureFile != QString()) {
                QString picture = getProjectPath() + "/Pictures/" + szPictureFile;
                if(QFile::exists(picture)) {
                    QImage image(getProjectPath() + "/Pictures/" + szPictureFile);
                    QImage scaleImage;
                    if(showNoScale_) {
                        scaleImage = image;
                    } else {
                        scaleImage = image.scaled(static_cast<int>(elementRect.width()),
                                                  static_cast<int>(elementRect.height()),
                                                  Qt::IgnoreAspectRatio);
                    }
                    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);
                    painter->drawImage(elementRect, scaleImage);
                }
            }

            painter->setPen(QPen(Qt::gray, 1, Qt::DashDotLine));
            painter->drawRect(elementRect);
        }
    }
}


/**
 * @brief ElementSwitchButton::mouseDoubleClickEvent
 * @details 切换按钮控件元素单击时弹出基本属性编辑对话框
 * @param event
 */
void ElementSwitchButton::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    EditBasicPropertyDialog dlg;
    QStringList listEvents;
    getSupportEvents(listEvents);
    dlg.setSupportEvents(listEvents);
    dlg.setFuncs(this->funcs_);
    dlg.setTagSelected(this->szTagSelected_);
    dlg.setStateOnInitial(this->stateOnInitial_);
    dlg.setShowContentText(this->bShowContentText_);
    dlg.setResetPictureFile(this->resetPictureFile_);
    dlg.setSetPictureFile(this->setPictureFile_);
    dlg.setResetText(this->resetText_);
    dlg.setSetText(this->setText_);
    dlg.setHAlign(this->szHAlign_);
    dlg.setVAlign(this->szVAlign_);
    if(dlg.exec() == QDialog::Accepted) {
        this->funcs_ = dlg.funcs();
        this->szTagSelected_ = dlg.tagSelected();
        this->stateOnInitial_ = dlg.stateOnInitial();
        this->bShowContentText_ = dlg.showContentText();
        showContent_ = this->bShowContentText_ ? tr("文本") : tr("图片");
        this->resetPictureFile_ = dlg.resetPictureFile();
        this->setPictureFile_ = dlg.setPictureFile();
        this->resetText_ = dlg.resetText();
        this->setText_ = dlg.setText();
        this->szHAlign_ = dlg.hAlign();
        this->szVAlign_ = dlg.vAlign();
        // 更新属性表
        updatePropertyEditor();
        scene()->update();
        update();
    }
    QGraphicsObject::mouseDoubleClickEvent(event);
}


void ElementSwitchButton::writeAsXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("element");
    writer.writeAttribute("internalType", internalElementType);
    writer.writeAttribute("elementId", elementId);
    writer.writeAttribute("tag", szTagSelected_);
    writer.writeAttribute("stateOnInitial", stateOnInitial_?"true":"false");

    writer.writeAttribute("resetPicture", resetPictureFile_);
    writer.writeAttribute("setPicture", setPictureFile_);
    writer.writeAttribute("showNoScale", showNoScale_?"true":"false");

    writer.writeAttribute("resettext", resetText_);
    writer.writeAttribute("settext", setText_);

    writer.writeAttribute("x", QString::number(x()));
    writer.writeAttribute("y", QString::number(y()));
    writer.writeAttribute("z", QString::number(zValue()));
    writer.writeAttribute("width", QString::number(elementWidth));
    writer.writeAttribute("height", QString::number(elementHeight));
    writer.writeAttribute("showContent", showContent_);
    writer.writeAttribute("halign", getHAlignString(szHAlign_));
    writer.writeAttribute("valign", getVAlignString(szVAlign_));
    writer.writeAttribute("resetBackgroundColor", resetBackgroundColor_.name());
    writer.writeAttribute("setBackgroundColor", setBackgroundColor_.name());
    writer.writeAttribute("transparent", transparent_?"true":"false");
    writer.writeAttribute("font", font_.toString());
    writer.writeAttribute("textcolor", textColor.name());
    writer.writeAttribute("fontsize", QString::number(fontSize));
    writer.writeAttribute("elemAngle", QString::number(elemAngle));
    writer.writeAttribute("enableOnInitial", enableOnInitial_?"true":"false");
    writer.writeAttribute("showOnInitial", showOnInitial_?"true":"false");
    writer.writeAttribute("functions", funcs_.join("|"));
    writer.writeEndElement();
}

void ElementSwitchButton::readFromXml(const QXmlStreamAttributes &attributes)
{
    if (attributes.hasAttribute("elementId")) {
        QString szID = attributes.value("elementId").toString();
        setElementId(szID);
        int index = getIndexFromIDString(szID);
        if(iLastIndex_ < index) {
            iLastIndex_ = index;
        }
    }

    if (attributes.hasAttribute("tag")) {
        szTagSelected_ = attributes.value("tag").toString();
    }

    if (attributes.hasAttribute("stateOnInitial")) {
        QString value = attributes.value("stateOnInitial").toString();
        stateOnInitial_ = false;
        if(value == "true") {
            stateOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("resetPicture")) {
        resetPictureFile_ = attributes.value("resetPicture").toString();
    }

    if (attributes.hasAttribute("setPicture")) {
        setPictureFile_ = attributes.value("setPicture").toString();
    }

    if (attributes.hasAttribute("showNoScale")) {
        QString value = attributes.value("showNoScale").toString();
        showNoScale_ = false;
        if(value == "true") {
            showNoScale_ = true;
        }
    }

    if (attributes.hasAttribute("resettext")) {
        resetText_ = attributes.value("resettext").toString();
    }

    if (attributes.hasAttribute("settext")) {
        setText_ = attributes.value("settext").toString();
    }

    if (attributes.hasAttribute("x")) {
        setElementXPos(attributes.value("x").toString().toInt());
    }

    if (attributes.hasAttribute("y")) {
        setElementYPos(attributes.value("y").toString().toInt());
    }

    if (attributes.hasAttribute("z")) {
        setZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    if (attributes.hasAttribute("showContent")) {
        showContent_ = attributes.value("showContent").toString();
        bShowContentText_ = true;
        if(showContent_ == tr("图片")) {
            bShowContentText_ = false;
        }
    }

    if (attributes.hasAttribute("halign")) {
        QString align = attributes.value("halign").toString();
        this->setHAlignString(align, szHAlign_);
    }

    if (attributes.hasAttribute("valign")) {
        QString align = attributes.value("valign").toString();
        this->setVAlignString(align, szVAlign_);
    }

    if (attributes.hasAttribute("resetBackgroundColor")) {
        resetBackgroundColor_ = QColor(attributes.value("resetBackgroundColor").toString());
    }

    if (attributes.hasAttribute("setBackgroundColor")) {
        setBackgroundColor_ = QColor(attributes.value("setBackgroundColor").toString());
    }

    if (attributes.hasAttribute("transparent")) {
        QString value = attributes.value("transparent").toString();
        transparent_ = false;
        if(value == "true") {
            transparent_ = true;
        }
    }

    if (attributes.hasAttribute("font")) {
        QString szFont = attributes.value("font").toString();
        font_.fromString(szFont);
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("fontsize")) {
        fontSize = attributes.value("fontsize").toString().toInt();
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    if (attributes.hasAttribute("enableOnInitial")) {
        QString value = attributes.value("enableOnInitial").toString();
        enableOnInitial_ = false;
        if(value == "true") {
            enableOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("showOnInitial")) {
        QString value = attributes.value("showOnInitial").toString();
        showOnInitial_ = false;
        if(value == "true") {
            showOnInitial_ = true;
        }
    }

    if (attributes.hasAttribute("functions")) {
        QString listString = attributes.value("functions").toString();
        funcs_ = listString.split('|');
    }

    updateBoundingElement();
    updatePropertyModel();

    reloadPropertyList();
}

void ElementSwitchButton::writeData(QDataStream &out)
{
    out << this->elementId
        << this->szTagSelected_
        << this->stateOnInitial_
        << this->resetPictureFile_
        << this->setPictureFile_
        << this->showNoScale_
        << this->resetText_
        << this->setText_
        << this->x()
        << this->y()
        << this->zValue()
        << this->elementWidth
        << this->elementHeight
        << this->showContent_
        << this->getHAlignString(szHAlign_)
        << this->getVAlignString(szVAlign_)
        << this->resetBackgroundColor_
        << this->setBackgroundColor_
        << this->transparent_
        << this->font_.toString()
        << this->textColor
        << this->fontSize
        << this->elemAngle
        << this->enableOnInitial_
        << this->showOnInitial_
        << this->funcs_;
}

void ElementSwitchButton::readData(QDataStream &in)
{
    QString id;
    QString szTagSelected;
    bool stateOnInitial;
    QString resetPic;
    QString setPic;
    bool showNoScale;
    QString szResetText;
    QString szSetText;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString showContent;
    QString text;
    QString hAlign;
    QString vAlign;
    QColor resetBackgroundColor;
    QColor setBackgroundColor;
    bool transparent;
    QString font;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool enableOnInitial;
    bool showOnInitial;
    QStringList funcs;

    in >> id
       >> szTagSelected
       >> stateOnInitial
       >> resetPic
       >> setPic
       >> showNoScale
       >> szResetText
       >> szSetText
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> showContent
       >> text
       >> hAlign
       >> vAlign
       >> resetBackgroundColor
       >> setBackgroundColor
       >> transparent
       >> font
       >> textColor
       >> fontSize
       >> angle
       >> enableOnInitial
       >> showOnInitial
       >> funcs;

    this->setElementId(id);
    this->szTagSelected_ = szTagSelected;
    this->stateOnInitial_ = stateOnInitial;
    this->resetPictureFile_ = resetPic;
    this->setPictureFile_ = setPic;
    this->showNoScale_ = showNoScale;
    this->resetText_ = szResetText;
    this->setText_ = szSetText;
    int index = getIndexFromIDString(id);
    if(iLastIndex_ < index) {
        iLastIndex_ = index;
    }
    this->setElementXPos(static_cast<int>(xpos));
    this->setElementYPos(static_cast<int>(ypos));
    this->setElementZValue(static_cast<int>(zvalue));
    this->setElementWidth(width);
    this->setElementHeight(height);
    this->showContent_ = showContent;
    this->setHAlignString(hAlign, szHAlign_);
    this->setVAlignString(vAlign, szVAlign_);
    this->resetBackgroundColor_ = resetBackgroundColor;
    this->setBackgroundColor_ = setBackgroundColor;
    this->transparent_ = transparent;
    this->textColor = textColor;
    this->fontSize = fontSize;
    this->setAngle(angle);
    this->enableOnInitial_ = enableOnInitial;
    this->showOnInitial_ = showOnInitial;
    this->funcs_ = funcs;
    this->updateBoundingElement();
    this->updatePropertyModel();
}

void ElementSwitchButton::getSupportEvents(QStringList &listValue)
{
    QString xmlFileName = Helper::AppDir() + "/Config/ElementSupportEvents.xml";

    QFile fileCfg(xmlFileName);
    if(!fileCfg.exists()) {
        QMessageBox::critical(nullptr, tr("提示"), tr("事件配置列表文件不存在！"));
        return;
    }
    if(!fileCfg.open(QFile::ReadOnly)) {
        return;
    }
    QString buffer = fileCfg.readAll();
    fileCfg.close();
    XMLObject xmlFuncSupportList;
    if(!xmlFuncSupportList.load(buffer, nullptr)) {
        return;
    }

    QList<XMLObject*> childrenFuncSupport = xmlFuncSupportList.getChildren();

    foreach(XMLObject* eventGroup, childrenFuncSupport) {
        QString szEventGroupName = eventGroup->getProperty("name");
        if(szEventGroupName == "SwitchButton") {

            QList<XMLObject*> childrenGroup = eventGroup->getChildren();
            if(childrenGroup.size() < 1)
                continue;

            foreach(XMLObject* event, childrenGroup) {
                QString eventName = event->getProperty("name");
                QString eventShowName = event->getProperty("ShowName");
                listValue << QString("%1-%2").arg(eventName).arg(eventShowName);

                QList<XMLObject*> funcDesc = event->getChildren();
                if(funcDesc.size() < 1)
                    continue;
                QString strDesc = event->getCurrentChild("desc")->getText();
            }
        }
    }
}

QDataStream &operator<<(QDataStream &out,const ElementSwitchButton &ele)
{
    out << ele.elementId
        << ele.szTagSelected_
        << ele.stateOnInitial_
        << ele.showNoScale_
        << ele.resetText_
        << ele.setText_
        << ele.x()
        << ele.y()
        << ele.zValue()
        << ele.elementWidth
        << ele.elementHeight
        << ele.showContent_
        << ele.getHAlignString(ele.szHAlign_)
        << ele.getVAlignString(ele.szVAlign_)
        << ele.resetBackgroundColor_
        << ele.setBackgroundColor_
        << ele.transparent_
        << ele.font_
        << ele.textColor
        << ele.fontSize
        << ele.elemAngle
        << ele.enableOnInitial_
        << ele.showOnInitial_
        << ele.funcs_;
    return out;
}

QDataStream &operator>>(QDataStream &in,ElementSwitchButton &ele)
{
    QString id;
    QString szTagSelected;
    bool stateOnInitial;
    bool showNoScale;
    QString szResetText;
    QString szSetText;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString showContent;
    QColor resetBackgroundColor;
    QColor setBackgroundColor;
    bool transparent;
    QString text;
    QString hAlign;
    QString vAlign;
    QString font;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool enableOnInitial;
    bool showOnInitial;
    QStringList funcs;

    in >> id
       >> szTagSelected
       >> stateOnInitial
       >> showNoScale
       >> szResetText
       >> szSetText
       >> xpos
       >> ypos
       >> zvalue
       >> width
       >> height
       >> showContent
       >> text
       >> hAlign
       >> vAlign
       >> resetBackgroundColor
       >> setBackgroundColor
       >> transparent
       >> font
       >> textColor
       >> fontSize
       >> angle
       >> enableOnInitial
       >> showOnInitial
       >> funcs;

    ele.setElementId(id);
    int index = ele.getIndexFromIDString(id);
    if(ele.iLastIndex_ < index) {
        ele.iLastIndex_ = index;
    }
    ele.szTagSelected_ = szTagSelected;
    ele.stateOnInitial_ = stateOnInitial;
    ele.showNoScale_ = showNoScale;
    ele.resetText_ = szResetText;
    ele.setText_ = szSetText;
    ele.setElementXPos(static_cast<int>(xpos));
    ele.setElementYPos(static_cast<int>(ypos));
    ele.setElementZValue(static_cast<int>(zvalue));
    ele.setElementWidth(width);
    ele.setElementHeight(height);
    ele.showContent_ = showContent;
    ele.setHAlignString(hAlign, ele.szHAlign_);
    ele.setVAlignString(vAlign, ele.szVAlign_);
    ele.resetBackgroundColor_ = resetBackgroundColor;
    ele.setBackgroundColor_ = setBackgroundColor;
    ele.transparent_ = transparent;
    ele.font_ = font;
    ele.textColor = textColor;
    ele.fontSize = fontSize;
    ele.setAngle(angle);
    ele.enableOnInitial_ = enableOnInitial;
    ele.showOnInitial_ = showOnInitial;
    ele.funcs_ = funcs;
    ele.updateBoundingElement();
    ele.updatePropertyModel();

    return in;
}

