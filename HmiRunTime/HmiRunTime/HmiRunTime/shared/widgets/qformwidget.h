#ifndef QFORMWIDGET_H
#define QFORMWIDGET_H

#include <QWidget>
#include "iloader.h"

class QFormWidget : public QWidget, public ILoader
{
    Q_PROPERTY(QStringList openFuncs READ getOpenFuncs WRITE setOpenFuncs)
    Q_PROPERTY(QStringList closeFuncs READ getCloseFuncs WRITE setCloseFuncs)
public:
    Q_INVOKABLE QFormWidget(QWidget *parent = nullptr);

public:
    void fromObject(XMLObject* xml) override;

    QStringList getOpenFuncs() const;
    void setOpenFuncs(const QStringList &value);
    QStringList getCloseFuncs() const;
    void setCloseFuncs(const QStringList &value);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    // 功能操作
    QStringList openFuncs;
    QStringList closeFuncs;
};

#endif // QFORMWIDGET_H
