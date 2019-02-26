#include "coloreditor.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QColorDialog>

ColorEditor::ColorEditor(QWidget *parent) :
    QWidget(parent)
{
    toolBtn = new QToolButton;
    toolBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Ignored);
    toolBtn->setText(trUtf8("..."));
    toolBtn->setFixedWidth(20);
    connect(toolBtn,SIGNAL(clicked()),SLOT(showColorDialog()));

    lblColorInfo = new QLabel(this);
    lblColorInfo->setText(getColorString(color));

    lblPic = new QLabel(this);
    lblPic->setPixmap(getColorPixmap(color));

    spacer = new QSpacerItem(1,0,QSizePolicy::Expanding,QSizePolicy::Ignored);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->addItem(spacer);
    hlayout->addWidget(toolBtn);
    setLayout(hlayout);

    setFocusProxy(toolBtn);
    setFocusPolicy(toolBtn->focusPolicy());

    lblPic->hide();
    lblColorInfo->hide();
}

void ColorEditor::setColor(const QColor& color_)
{
    if (color != color_)
    {
        color = color_;
        lblPic->setPixmap(getColorPixmap(color));
        lblColorInfo->setText(getColorString(color));
    }
}

QPixmap ColorEditor::getColorPixmap(const QColor& color, unsigned int size)
{
    QImage tmpImgage(size, size, QImage::Format_ARGB32_Premultiplied);
    tmpImgage.fill(static_cast<unsigned int>(color.rgb()));
    return QPixmap::fromImage(tmpImgage);
}

QString ColorEditor::getColorString(const QColor& color)
{
    return QApplication::translate("QtPropertyExplorer", "[%1, %2, %3] (%4)", "Colors as string")
                                 .arg(QString::number(color.red()))
                                 .arg(QString::number(color.green()))
                                 .arg(QString::number(color.blue()))
                                 .arg(QString::number(color.alpha()));

}

void ColorEditor::showColorDialog() {

    bool ok = false;
    QRgb rgb = color.rgba();
    QRgb newRgb = QColorDialog::getRgba(rgb,&ok,this);

    if (ok && newRgb != rgb) {
        setColor(QColor::fromRgba(newRgb));
    }
}

QColor ColorEditor::getColor() const {
    return color;
}
