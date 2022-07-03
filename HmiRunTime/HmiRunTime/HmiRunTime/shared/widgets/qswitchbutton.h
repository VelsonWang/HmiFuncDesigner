#ifndef QSWITCHBUTTON_H
#define QSWITCHBUTTON_H

#include <QWidget>
#include <QPaintEvent>
#include "realtimedb.h"
#include "iloader.h"

class QSwitchButton : public QWidget, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTagSelected WRITE setTagSelected)
    Q_PROPERTY(QStringList onFuncs READ getOnFuncs WRITE setOnFuncs)
    Q_PROPERTY(QStringList offFuncs READ getOffFuncs WRITE setOffFuncs)
    Q_PROPERTY(bool stateOnInitial READ getStateOnInitial WRITE setStateOnInitial)
    Q_PROPERTY(QString showContent READ getShowContent WRITE setShowContent)
    Q_PROPERTY(QString resetPictureFile READ getResetPictureFile WRITE setResetPictureFile)
    Q_PROPERTY(QString setPictureFile READ getSetPictureFile WRITE setSetPictureFile)
    Q_PROPERTY(bool showNoScale READ getShowNoScale WRITE setShowNoScale)
    Q_PROPERTY(QString resetText READ getResetText WRITE setResetText)
    Q_PROPERTY(QString setText READ getSetText WRITE setSetText)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QString szHAlign READ getSzHAlign WRITE setSzHAlign)
    Q_PROPERTY(QString szVAlign READ getSzVAlign WRITE setSzVAlign)
    Q_PROPERTY(QColor resetBackgroundColor READ getResetBackgroundColor WRITE setResetBackgroundColor)
    Q_PROPERTY(QColor setBackgroundColor READ getSetBackgroundColor WRITE setSetBackgroundColor)
    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth)
    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)
    Q_PROPERTY(bool transparent READ getTransparent WRITE setTransparent)
    Q_PROPERTY(QFont font READ getFont WRITE setFont)
    Q_PROPERTY(bool enableOnInitial READ getEnableOnInitial WRITE setEnableOnInitial)
    Q_PROPERTY(bool showOnInitial READ getShowOnInitial WRITE setShowOnInitial)

public:
    Q_INVOKABLE QSwitchButton(QWidget *parent = nullptr);

    QString getTagSelected() const;
    void setTagSelected(const QString &value);

    QStringList getOnFuncs() const;
    void setOnFuncs(const QStringList &value);

    QStringList getOffFuncs() const;
    void setOffFuncs(const QStringList &value);

    bool getStateOnInitial() const;
    void setStateOnInitial(bool value);

    QString getShowContent() const;
    void setShowContent(const QString &value);

    QString getResetPictureFile() const;
    void setResetPictureFile(const QString &value);

    QString getSetPictureFile() const;
    void setSetPictureFile(const QString &value);

    bool getShowNoScale() const;
    void setShowNoScale(bool value);

    QString getResetText() const;
    void setResetText(const QString &value);

    QString getSetText() const;
    void setSetText(const QString &value);

    QString getSzHAlign() const;
    void setSzHAlign(const QString &value);

    QString getSzVAlign() const;
    void setSzVAlign(const QString &value);

    QColor getResetBackgroundColor() const;
    void setResetBackgroundColor(const QColor &value);

    QColor getSetBackgroundColor() const;
    void setSetBackgroundColor(const QColor &value);

    bool getTransparent() const;
    void setTransparent(bool value);

    QFont getFont() const;
    void setFont(const QFont &value);

    bool getEnableOnInitial() const;
    void setEnableOnInitial(bool value);

    bool getShowOnInitial() const;
    void setShowOnInitial(bool value);

    int getBorderWidth() const;
    void setBorderWidth(int value);

    QColor getBorderColor() const;
    void setBorderColor(const QColor &value);

    QColor getTextColor() const;
    void setTextColor(const QColor &value);

public:
    void fromObject(XMLObject* xml) override;

private:
    void setPropertyInner();
    void drawSwitchButton(QPainter *painter);
    void execFunc(bool bVal);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 关联的变量
    QString tagSelected;
    // 功能操作
    QStringList onFuncs;
    QStringList offFuncs;
    // 初始状态
    bool stateOnInitial;
    // 显示内容
    QString showContent;
    // false-显示图片，true-显示文本
    bool showContentText;
    // 复位图片
    QString resetPictureFile;
    // 置位图片
    QString setPictureFile;
    // 原尺寸显示
    bool showNoScale;

    // 复位显示文本
    QString resetText;
    // 置位显示文本
    QString setText;
    // 文本颜色
    QColor textColor;
    // 水平对齐
    QString szHAlign;
    // 垂直对齐
    QString szVAlign;
    // 复位按钮背景颜色
    QColor resetBackgroundColor;
    // 置位按钮背景颜色
    QColor setBackgroundColor;
    // 透明背景颜色
    bool transparent;
    // 边框宽度
    int borderWidth;
    // 边框颜色
    QColor borderColor;
    // 字体
    QFont font;
    // 初始有效
    bool enableOnInitial;
    // 初始可见
    bool showOnInitial;

    //-------------------------------------------------------------------------

    QImage resetImageObj;
    QImage setImageObj;
    bool selected;
    // 上一次读取的变量值
    bool bLastTagVal = false;
    RunTimeTag *m_tag = NULL;
};

#endif // QSWITCHBUTTON_H
