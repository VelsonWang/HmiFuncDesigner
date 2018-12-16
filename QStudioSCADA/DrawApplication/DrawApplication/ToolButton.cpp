
#include "ToolButton.h"

ToolButton::ToolButton(QWidget *parent) :
    QToolButton(parent)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(Click(bool)));
}


void ToolButton::Click(bool checked)
{
    emit Click(this->text());
}




