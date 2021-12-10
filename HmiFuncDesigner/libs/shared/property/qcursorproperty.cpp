#include "qcursorproperty.h"
#include "../qcommonstruct.h"
#include <QCursor>
#include <QComboBox>
#include <QIcon>
#include <QCursor>

QCursorProperty::QCursorProperty(QAbstractProperty *parent) :
    QEnumProperty(parent)
{
    setObjectProperty("type", "Cursor");

    ComboItems items;
    tagComboItem item;
    item.m_icon = ":/images/cursors/arrow.png";
    item.m_text = tr("箭头"); // tr("Arrow")
    item.m_value = QCursor(Qt::ArrowCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/uparrow.png";
    item.m_text = tr("向上箭头"); // tr("Up Arrow")
    item.m_value = QCursor(Qt::UpArrowCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/cross.png";
    item.m_text = tr("十字"); // tr("Cross")
    item.m_value = QCursor(Qt::CrossCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/wait.png";
    item.m_text = tr("等待"); // tr("Wait")
    item.m_value = QCursor(Qt::WaitCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/ibeam.png";
    item.m_text = tr("工字"); // tr("IBeam");
    item.m_value = QCursor(Qt::IBeamCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/sizev.png";
    item.m_text = tr("垂直尺寸"); // tr("Size Vertical")
    item.m_value = QCursor(Qt::SizeVerCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/sizeh.png";
    item.m_text = tr("水平尺寸"); // tr("Size Horizontal")
    item.m_value = QCursor(Qt::SizeHorCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/sizef.png";
    item.m_text = tr("大小调整"); // tr("Size Slash")
    item.m_value = QCursor(Qt::SizeFDiagCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/sizeb.png";
    item.m_text = tr("大小反斜杠"); // tr("Size Backslash")
    item.m_value = QCursor(Qt::SizeBDiagCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/sizeall.png";
    item.m_text = tr("所有大小"); // tr("Size All")
    item.m_value = QCursor(Qt::SizeAllCursor);
    items.append(item);

    item.m_icon = "";
    item.m_text = tr("空白"); // tr("Blank")
    item.m_value = QCursor(Qt::BlankCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/vsplit.png";
    item.m_text = tr("垂直分裂"); // tr("Split Vertical")
    item.m_value = QCursor(Qt::SplitVCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/hsplit.png";
    item.m_text = tr("水平分裂"); // tr("Split Horizontal")
    item.m_value = QCursor(Qt::SplitHCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/hand.png";
    item.m_text = tr("指向手"); // tr("Pointing Hand")
    item.m_value = QCursor(Qt::PointingHandCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/no.png";
    item.m_text = tr("禁止"); // tr("Forbidden")
    item.m_value = QCursor(Qt::ForbiddenCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/openhand.png";
    item.m_text = tr("张开手"); // tr("Open Hand")
    item.m_value = QCursor(Qt::OpenHandCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/closedhand.png";
    item.m_text = tr("关闭手"); // tr("Closed Hand")
    item.m_value = QCursor(Qt::ClosedHandCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/whatsthis.png";
    item.m_text = tr("这是什么"); // tr("What's This")
    item.m_value = QCursor(Qt::WhatsThisCursor);
    items.append(item);

    item.m_icon = ":/images/cursors/busy.png";
    item.m_text = tr("忙"); // tr("Busy")
    item.m_value = QCursor(Qt::BusyCursor);
    items.append(item);

    QVariant v;
    v.setValue<ComboItems>(items);
    setAttribute("items", v);
}

void QCursorProperty::makeValue()
{
    QCursor c((Qt::CursorShape)m_propertys.value("value").toInt());
    m_value.setValue<QCursor>(c);
}

void QCursorProperty::writeValue()
{
    QCursor c = m_value.value<QCursor>();
    m_propertys.insert("value", int(c.shape()));
}

