#ifndef QPUSHBUTTONEX_H
#define QPUSHBUTTONEX_H

#include <QPushButton>
#include <QPaintEvent>

class QPushButtonEx : public QPushButton
{
    Q_OBJECT
    Q_ENUMS(ShowContent HAlign VAlign)
    Q_PROPERTY(QStringList clickedDownFuncs READ getClickedDownFuncs WRITE setClickedDownFuncs)
    Q_PROPERTY(QStringList clickedUpFuncs READ getClickedUpFuncs WRITE setClickedUpFuncs)
    Q_PROPERTY(QString script READ getScript WRITE setScript)
    Q_PROPERTY(ShowContent showContent READ getShowContent WRITE setShowContent)
    Q_PROPERTY(QString picture READ getFilePicture WRITE setFilePicture)
    Q_PROPERTY(QString text READ getText WRITE setText)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(HAlign hAlign READ getHAlign WRITE setHAlign)
    Q_PROPERTY(VAlign vAlign READ getVAlign WRITE setVAlign)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool transparent READ getTransparent WRITE setTransparent)
    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth)
    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)
    Q_PROPERTY(QFont font READ getFont WRITE setFont)
    Q_PROPERTY(bool enableOnInitial READ getEnableOnInitial WRITE setEnableOnInitial)
    Q_PROPERTY(bool showOnInitial READ getShowOnInitial WRITE setShowOnInitial )

public:
    explicit QPushButtonEx(QWidget *parent = nullptr);

    enum ShowContent {
        Text,
        Image,
    };

    enum HAlign {
        Left,
        HCenter,
        Right,
    };

    enum VAlign {
        Top,
        VCenter,
        Bottom,
    };

    QStringList getClickedDownFuncs() const;
    void setClickedDownFuncs(const QStringList &value);

    QStringList getClickedUpFuncs() const;
    void setClickedUpFuncs(const QStringList &value);

    QString getScript() const;
    void setScript(const QString &value);

    ShowContent getShowContent() const;
    void setShowContent(ShowContent value);

    QString getFilePicture() const;
    void setFilePicture(const QString &value);

    HAlign getHAlign() const;
    void setHAlign(HAlign value);

    VAlign getVAlign() const;
    void setVAlign(VAlign value);

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

private:
    void setPropertyInner();
    void drawPushButton(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    // 按下时执行功能操作
    QStringList clickedDownFuncs;
    // 抬起时执行功能操作
    QStringList clickedUpFuncs;
    // JavaScript脚本
    QString script;
    // 显示内容
    ShowContent showContent;
    // 显示内容
    bool showContentText;
    // 图片名
    QString filePicture;
    // 置位显示文本
    QString text;
    // 文本颜色
    QColor textColor;
    // 水平对齐
    HAlign hAlign;
    // 垂直对齐
    VAlign vAlign;
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

};

#endif // QPUSHBUTTONEX_H
