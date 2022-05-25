#ifndef QFORM_H
#define QFORM_H

#include <QWidget>

class QForm : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QStringList openFuncs READ getOpenFuncs WRITE setOpenFuncs)
    Q_PROPERTY(QStringList closeFuncs READ getCloseFuncs WRITE setCloseFuncs)

public:
    explicit QForm(QWidget *parent = nullptr);

    QStringList getOpenFuncs() const;
    void setOpenFuncs(const QStringList &value);

    QStringList getCloseFuncs() const;
    void setCloseFuncs(const QStringList &value);

private:
    // 画面打开功能操作
    QStringList openFuncs;
    // 画面关闭功能操作
    QStringList closeFuncs;

};

#endif // QFORM_H
