#include "qbaseeditorwidget.h"

#include "qbackgrounsheeteditor.h"
#include "qbordersheeteditor.h"
#include "qtextsheeteditor.h"

#include "../../../gradienteditor/qtgradientdialog.h"
#include "../../../shared/property/stylesheetitem/qabstractstylesheetitem.h"

#include <QColorDialog>
#include <QPainter>

QBaseEditorWidget* create_editor_widget(const QString &name)
{
    if(name=="background")
    {
        return new QBackgrounSheetEditor;
    }
    else if(name=="border")
    {
        return new QBorderSheetEditor;
    }
    else if(name=="text_sheet")
    {
        return new QTextSheetEditor;
    }

    return Q_NULLPTR;
}

QBaseEditorWidget::QBaseEditorWidget(QWidget *parent) :
    QWidget(parent)
{
}


void QBaseEditorWidget::set_item(QAbstractStylesheetItem *item)
{
    m_item=item->value().value<tagStylesheetItem>();
}

tagStylesheetItem QBaseEditorWidget::get_value()
{
    return m_item;
}

QGradient QBaseEditorWidget::get_color(const QColor &color)
{
    QColor temp;
    if(color.isValid())
    {
        temp=color;
    }
    else
    {
        temp=QColor(255,255,255);
    }
    QColor c=QColorDialog::getColor(temp,this);
    if(c.isValid())
    {
        QGradient g=QLinearGradient(0,0,1,0);
        g.setColorAt(0,c);
        g.setColorAt(1,c);
        return g;
    }
    else
    {
        return QGradient();
    }
}

QGradient QBaseEditorWidget::get_gradient(const QGradient &g)
{
    QtGradientDialog dlg(this);
    dlg.setGradient(g);
    int res=dlg.exec();
    if (res == QDialog::Accepted)
    {
        return dlg.gradient();
    }
    return QGradient();
}

QIcon QBaseEditorWidget::get_icon(const QGradient &gradient)
{
    QPixmap pixmap=QPixmap(16,16);

    pixmap.fill(Qt::transparent);
    if(gradient.type()!=QGradient::NoGradient)
    {
        QPainter p(&pixmap);
        p.fillRect(QRect(0,0,16,16),QBrush(gradient));
    }

    QIcon icon=QIcon(pixmap);
    icon.addPixmap(pixmap,QIcon::Disabled);
    return icon;
}
