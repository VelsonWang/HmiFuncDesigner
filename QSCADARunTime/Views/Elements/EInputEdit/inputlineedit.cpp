#include "InputLineEdit.h"

InputLineEdit::InputLineEdit(QWidget *parent) :
    QLineEdit(parent) {
    this->setStyleSheet("background: transparent;");
}

InputLineEdit::~InputLineEdit() {

}

void InputLineEdit::paintEvent(QPaintEvent *event) {
    QLineEdit::paintEvent(event);
}
