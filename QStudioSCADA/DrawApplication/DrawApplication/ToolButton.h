#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
#include <QString>


class ToolButton : public QToolButton
{
    Q_OBJECT
public:
    ToolButton(QWidget *parent = 0);

signals:
    void Click(QString s);

private slots:
    void Click(bool checked = false);

};

#endif // TOOLBUTTON_H
