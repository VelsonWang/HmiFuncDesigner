#ifndef INPUTLINEEDIT_H
#define INPUTLINEEDIT_H

#include <QLineEdit>
#include <QPaintEvent>

class InputLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit InputLineEdit(QWidget *parent = nullptr);
    virtual ~InputLineEdit();

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent *event);

};

#endif // INPUTLINEEDIT_H
