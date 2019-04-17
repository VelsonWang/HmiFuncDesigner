#include "InputLineEdit.h"

InputLineEdit::InputLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

}

InputLineEdit::~InputLineEdit() {

}

void InputLineEdit::paintEvent(QPaintEvent *event) {

    QLineEdit::paintEvent(event);
}
