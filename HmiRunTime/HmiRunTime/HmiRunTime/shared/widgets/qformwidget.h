#ifndef QFORMWIDGET_H
#define QFORMWIDGET_H

#include <QWidget>
#include "iloader.h"

class QFormWidget : public QWidget, public ILoader
{
public:
    Q_INVOKABLE QFormWidget(QWidget *parent = nullptr);

public:
    void fromObject(XMLObject* xml) override;
};

#endif // QFORMWIDGET_H
