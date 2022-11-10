#ifndef QFORMWIDGET_H
#define QFORMWIDGET_H

#include <QWidget>
#include "iloader.h"

class QFormWidget : public QWidget, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QColor background READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QStringList openFuncs READ getOpenFuncs WRITE setOpenFuncs)
    Q_PROPERTY(QStringList closeFuncs READ getCloseFuncs WRITE setCloseFuncs)
public:
    Q_INVOKABLE QFormWidget(QWidget *parent = nullptr);

public:
    void fromObject(XMLObject* xml) override;

    QColor getBackgroundColor();
    void setBackgroundColor(QColor color);
    QStringList getOpenFuncs() const;
    void setOpenFuncs(const QStringList &value);
    QStringList getCloseFuncs() const;
    void setCloseFuncs(const QStringList &value);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    // 功能操作
    QStringList openFuncs;
    QStringList closeFuncs;
    // 画面背景颜色
    QColor backgroundColor;
};

#endif // QFORMWIDGET_H
