#include "qresourceshowwidget.h"
#include "ui_qresourceshowwidget.h"

QResourceShowWidget::QResourceShowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QResourceShowWidget)
{
    ui->setupUi(this);
}

QResourceShowWidget::~QResourceShowWidget()
{
    delete ui;
}

void QResourceShowWidget::on_okbtn_clicked()
{
    emit ok();
}

void QResourceShowWidget::on_cancelbtn_clicked()
{
    emit cancel();
}

void QResourceShowWidget::set_image(const QByteArray &data)
{
    QImage image=QImage::fromData(data);

    QSize sz=image.size();

    QSize dsz=ui->show_widget->size();
    dsz.rheight()-=4;
    dsz.rwidth()-=4;

    if(sz.width()>dsz.width() || sz.height()>dsz.height())
    {
        image=image.scaled(dsz,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    }
    QPixmap map;
    map.convertFromImage(image);
    ui->show_widget->setPixmap(map);
}
