#include "splitterform.h"

SplitterForm::SplitterForm(Qt::Orientation orientation, QWidget * parent) :
    QSplitter(parent)
{
    this->setOrientation(orientation);
}

