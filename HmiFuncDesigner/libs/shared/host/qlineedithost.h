#ifndef QLINEEDITHOST_H
#define QLINEEDITHOST_H

#include "qwidgethost.h"

class QLineEditHost : public QWidgetHost
{
    Q_OBJECT
public:
    Q_INVOKABLE QLineEditHost(QAbstractHost *parent = 0);

    static QString getShowName();
    static QString getShowIcon();
    static QString getShowGroup();

public slots:
    void setText(const QString text);
    QString text();

    void setAlignment(int alignment);
    int alignment();

    void setFrame(bool frame);
    bool frame();

    void setReadOnly(bool readOnly);
    bool readOnly();

protected slots:
    void textChanged(const QString &text);
    void editFinish();

protected:
    void initProperty() override;

protected:
    void createObject() override;
    // 控件支持的功能事件
    QStringList supportFuncEvents() override;
};


#endif // QLINEEDITHOST_H
