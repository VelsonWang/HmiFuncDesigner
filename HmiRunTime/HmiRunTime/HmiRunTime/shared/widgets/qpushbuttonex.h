#ifndef QPUSHBUTTONEX_H
#define QPUSHBUTTONEX_H

#include <QPushButton>
#include <QPaintEvent>
#include "iloader.h"

class QPushButtonEx : public QPushButton, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QStringList clickedDownFuncs READ getClickedDownFuncs WRITE setClickedDownFuncs)
    Q_PROPERTY(QStringList clickedUpFuncs READ getClickedUpFuncs WRITE setClickedUpFuncs)
    Q_PROPERTY(QString script READ getScript WRITE setScript)
    Q_PROPERTY(QString showContent READ getShowContent WRITE setShowContent)
    Q_PROPERTY(QString picture READ getFilePicture WRITE setFilePicture)
    Q_PROPERTY(QString text READ getText WRITE setText)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QString hAlign READ getHAlign WRITE setHAlign)
    Q_PROPERTY(QString vAlign READ getVAlign WRITE setVAlign)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool transparent READ getTransparent WRITE setTransparent)
    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth)
    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)
    Q_PROPERTY(QFont font READ getFont WRITE setFont)
    Q_PROPERTY(bool enableOnInitial READ getEnableOnInitial WRITE setEnableOnInitial)
    Q_PROPERTY(bool showOnInitial READ getShowOnInitial WRITE setShowOnInitial )

public:
    Q_INVOKABLE QPushButtonEx(QWidget *parent = nullptr);

    QStringList getClickedDownFuncs() const;
    void setClickedDownFuncs(const QStringList &value);

    QStringList getClickedUpFuncs() const;
    void setClickedUpFuncs(const QStringList &value);

    QString getScript() const;
    void setScript(const QString &value);

    QString getShowContent() const;
    void setShowContent(const QString &value);

    QString getFilePicture() const;
    void setFilePicture(const QString &value);

    QString getHAlign() const;
    void setHAlign(const QString &value);

    QString getVAlign() const;
    void setVAlign(const QString &value);

    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor &value);

    bool getTransparent() const;
    void setTransparent(bool value);

    QFont getFont() const;
    void setFont(const QFont &value);

    bool getEnableOnInitial() const;
    void setEnableOnInitial(bool value);

    bool getShowOnInitial() const;
    void setShowOnInitial(bool value);

    QString getText() const;
    void setText(const QString &value);

    QColor getTextColor() const;
    void setTextColor(const QColor &value);

    int getBorderWidth() const;
    void setBorderWidth(int value);

    QColor getBorderColor() const;
    void setBorderColor(const QColor &value);

public:
    void fromObject(XMLObject* xml) override;

private:
    void setPropertyInner();
    void drawPushButton(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 功能操作
    QStringList clickedDownFuncs;
    QStringList clickedUpFuncs;
    // JavaScript脚本
    QString script;
    // 显示内容
    QString showContent;
    // 显示内容
    bool showContentText;
    // 图片名
    QString filePicture;
    // 置位显示文本
    QString text;
    // 文本颜色
    QColor textColor;
    // 水平对齐
    QString hAlign;
    // 垂直对齐
    QString vAlign;
    // 按钮背景颜色
    QColor backgroundColor;
    // 透明背景颜色
    bool transparent;
    // 边框宽度
    int borderWidth;
    // 边框颜色
    QColor borderColor;
    // 字体
    QFont font;
    // 初始有效性
    bool enableOnInitial;
    // 初始可见性
    bool showOnInitial;

private:
    QImage imageObj;
    bool selected;
};

#endif // QPUSHBUTTONEX_H
