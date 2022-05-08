#ifndef QABSTRACTSPINBOXHOST_H
#define QABSTRACTSPINBOXHOST_H

#include "qwidgethost.h"

class QAbstractSpinBoxHost : public QWidgetHost
{
    Q_OBJECT
public:
    QAbstractSpinBoxHost(QAbstractHost *parent = 0);

protected:
    void initProperty() override;

public slots:
    void setFrame(bool frame);
    bool frame();

    void setWraping(bool wraping);
    bool wraping();

    void setReadOnly(bool readonly);
    bool readonly();

    void setButtonSymbols(int buttonSymbols);
    int buttonSymbols();

    void setAlignment(int alignment);
    int alignment();
};

#endif // QABSTRACTSPINBOXHOST_H
