#ifndef QFORM_H
#define QFORM_H

#include <QWidget>

class QForm : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor background READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QStringList openFuncs READ getOpenFuncs WRITE setOpenFuncs)
    Q_PROPERTY(QStringList closeFuncs READ getCloseFuncs WRITE setCloseFuncs)

public:
    explicit QForm(QWidget *parent = nullptr);

    QColor getBackgroundColor();
    void setBackgroundColor(QColor color);

    QStringList getOpenFuncs() const;
    void setOpenFuncs(const QStringList &value);

    QStringList getCloseFuncs() const;
    void setCloseFuncs(const QStringList &value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    // 画面打开功能操作
    QStringList openFuncs;
    // 画面关闭功能操作
    QStringList closeFuncs;
    // 画面背景颜色
    QColor backgroundColor;
};

#endif // QFORM_H
